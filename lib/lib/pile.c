/*    /lib/pile.c
 *    from the Dead Soulsr2 Object Library
 *    basically a pile of precious coinses
 *    created by Descartes of Borg 951029
 *    Version: @(#) pile.c 1.2@(#)
 *    Last modified: 96/11/12
 */

#include <lib.h>

inherit LIB_ITEM;

private string PileType   = 0;
private int    PileAmount = 0;

static void create() {
    item::create();
    SetKeyName("pile");
}

string array GetId() {
    string array id;

    id = item::GetId();
    if( PileType ) {
	id += ({ PileType, PileAmount + " " + PileType });
    }
    return ({ id..., "money", "pile" });
}

varargs string GetLong(string str) {
    if( !PileAmount ) {
	return 0;
    }
    return "It is a pile of " + PileAmount + " " + PileType + ".";
}

int GetMass() {
    return currency_mass(PileAmount, PileType);
}

void SetPile(string str, int amt) {
    PileType = str;
    PileAmount = amt;
    parse_refresh();
}

string GetPileType() { return PileType; }

int GetPileAmount() { return PileAmount; }

string GetShort() {
    string str = item::GetShort();

    if( str ) {
	return str;
    }
    else {
	return "a pile of " + PileAmount + " " + PileType;
    }
}

mixed eventGetMoney(object who, int amount, string curr) {
    if( who->AddCurrency(curr, amount) == -1 ) {
	who->eventPrint("You had a problem getting the money.");
	return 1;
    }
    send_messages("get", "$agent_name $agent_verb " + amount + " " +
		  curr + " " + GetShort() + ".", who, 0, environment(who));
    PileAmount -= amount;
    if( PileAmount < 1 ) {
	call_out((: Destruct :), 0);
	return 1;
    }
    SetPile(PileType, PileAmount); /* This refreshes the parser */
    return 1;
}

int eventMove(mixed dest) {
    int x;

    x = item::eventMove(dest);
    if( !living(environment()) ) {
	return x;
    }
    environment()->AddCurrency(PileType, PileAmount);
    SetShort("a pile of " + PileAmount + " " + PileType);
    PileAmount = 0;
    call_out((: Destruct :), 0);
    return x;
}

mixed direct_get_wrd_wrd_out_of_obj(string num, string curr) {
    int amt;
    
    if( environment() != environment(this_player()) ) {
	return "#You cannot reach the pile!";
    }
    if( num[0] < '0' || num[0] > '9' ) {
	return 0;
    }
    if( (amt = to_int(num)) < 1 ) {
	return "That's a totally bogus amount.";
    }
    if( curr != PileType ) {
	return "#The pile has no " + curr + " in it, only " + PileType + ".";
    }
    if( amt > PileAmount ) {
	return "#There is not that much in the pile.";
    }
    if( !this_player()->CanCarry(currency_mass(amt, curr)) ) {
	return "It is too heavy for you!";
    }
    return 1;    
}

mixed direct_get_wrd_wrd_from_obj(string amt, string curr) {
    return direct_get_wrd_wrd_out_of_obj(amt, curr);
}
