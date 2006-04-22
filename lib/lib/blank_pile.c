#include <lib.h>

inherit LIB_ITEM;

private string PileType   = 0;
int    PileAmount = 0;

static void create() {
    string *saveds;
    item::create();
    saveds = item::GetSave();
    saveds += ({ "PileType", "PileAmount" });
    AddSave( saveds );
    SetKeyName("money");
}

string array GetId() {
    string array id;

    id = item::GetId();
    if( PileType ) {
	if(PileType != "dollars") {
	    id += ({ PileType, PileAmount + " " + PileType +" coins"});
	    return ({ id..., "money", "pile","coins","coin" });
	}
	else {
	    id += ({ PileType, PileAmount + " " + PileType +" dollars"});
	    return ({ id..., "money", "pile","bill","bills" });
	}
    }
}

varargs string GetLong(string str) {
    if( !PileAmount ) {
	return 0;
    }
    if(PileType != "dollars") {
	return PileAmount + " " + PileType + " coins.";
    }
    else return PileAmount + " " + PileType + ".";
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
    string sum;
    if(!PileAmount) sum = "some";
    else sum = cardinal(PileAmount);
    if(PileType != "dollars") {
	return sum + " " + PileType + " coins";
    }
    else return sum + " " + PileType ;
}

mixed eventGetCurrency(object who, int amount, string curr) {
    string sum;
    if(!amount) amount = PileAmount;
    if( who->AddCurrency(curr, amount) == -1 ) {
	who->eventPrint("You had a problem getting the money.");
	return 1;
    }
    if(!amount) sum = "some";
    else sum = cardinal(amount);
    send_messages("get", "$agent_name $agent_verb " + sum + " " +
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
    if( !environment() || !living(environment()) ) {
	return x;
    }
    if(environment() && living(environment()) ) {
	environment()->AddCurrency(PileType, PileAmount);
	environment()->AddCarriedMass(-this_object()->GetMass());
	SetShort(PileAmount + " " + PileType);
	//PileAmount = 0;
	call_out((: Destruct :), 0);
	return x;
    }
}

mixed direct_get_wrd_wrd_out_of_obj(string num, string curr) {
    int amt;

    if( environment() != environment(this_player()) ) {
	return "#You cannot reach the money!";
    }
    if( num[0] < '0' || num[0] > '9' ) {
	return 0;
    }
    if( (amt = to_int(num)) < 1 ) {
	return "That's a totally bogus amount.";
    }
    if( curr != PileType ) {
	return "#There are no " + curr + " there, only " + PileType + ".";
    }
    if( amt > PileAmount ) {
	return "#There is not that much there.";
    }
    if( !this_player()->CanCarry(currency_mass(amt, curr)) ) {
	return "It is too heavy for you!";
    }
    return 1;    
}

mixed direct_get_wrd_wrd_from_obj(string amt, string curr) {
    return direct_get_wrd_wrd_out_of_obj(amt, curr);
}
