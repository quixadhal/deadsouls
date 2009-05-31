/*    /lib/pile.c
 *    from the Dead Souls Object Library
 *    basically a pile of precious coinses
 *    created by Descartes of Borg 951029
 *    Version: @(#) pile.c 1.2@(#)
 *    Last modified: 96/11/12
 */

#include <lib.h>

inherit LIB_ITEM;

string PileType   = 0;
int PileAmount = 0;

static void create(){
    string *saveds;
    item::create();
    SetNoCondition(1);
    saveds = item::GetSave();
    saveds += ({ "PileType", "PileAmount" });
    AddSave( saveds );
    SetKeyName("pile");
}

string array GetId(){
    string array id;
    string tender = "coins";
    if(PileType == "dollars") tender = "bills";

    id = item::GetId();
    if( PileType ){
        id += ({ PileType, PileAmount + " " + PileType, "pile of "+PileType });
    }
    return ({ id..., tender, "money", "pile", "pile of "+ PileAmount + " " + PileType });
}

int GetMass(){
    return currency_mass(PileAmount, PileType);
}

void SetPile(string str, int amt){
    PileType = str;
    PileAmount = amt;
    if(!PileAmount || PileAmount < 1 ){ 
        SetLong("a pile of money");
        call_out( (: eventDestruct :), 1);
    }
    else SetLong("It is a pile of " + PileAmount + " " + PileType + ".");
    parse_refresh();
}

void SetCurrency(string str, int amt){
    SetPile(str,  amt);
}

string GetPileType(){ return PileType; }

int GetPileAmount(){ return PileAmount; }

string GetShort(){
    string str = item::GetShort();

    if(!PileAmount || PileAmount < 1 ){
        call_out( (: eventDestruct :), 1);
        return "a pile of money";
    }
    if( str ){
        return str;
    }
    else {
        return "a pile of " + PileAmount + " " + PileType;
    }
}

mixed eventGetMoney(object who, int amount, string curr){
    if( who->AddCurrency(curr, amount) == -1 ){
        who->eventPrint("You had a problem getting the money.");
        return 1;
    }
    send_messages("get", "$agent_name $agent_verb " + amount + " " +
            curr + " from " + GetShort() + ".", who, 0, environment(who));
    PileAmount -= amount;
    if( PileAmount < 1 ){
        call_out((: Destruct :), 0);
        return 1;
    }
    SetPile(PileType, PileAmount); /* This refreshes the parser */
    return 1;
}

int eventMove(mixed dest){
    int x;

    x = item::eventMove(dest);
    if( environment() && !living(environment()) ){
        return x;
    }

    if(environment() && living(environment())){
        environment()->AddCurrency(PileType, PileAmount);
        environment()->AddCarriedMass(-this_object()->GetMass());
        SetShort("a pile of " + PileAmount + " " + PileType);
        PileAmount = 0;
        call_out((: Destruct :), 0);
        return x;
    }
}

mixed direct_get_wrd_wrd_out_of_obj(string num, string curr){
    int amt;

    if( environment() != environment(this_player()) ){
        return "#You cannot reach the pile!";
    }
    if( num[0] < '0' || num[0] > '9' ){
        return 0;
    }
    if( (amt = to_int(num)) < 1 ){
        return "That's a totally bogus amount.";
    }
    if( curr != PileType ){
        return "#The pile has no " + curr + " in it, only " + PileType + ".";
    }
    if( amt > PileAmount ){
        return "#There is not that much in the pile.";
    }
    if( !this_player()->CanCarry(currency_mass(amt, curr)) ){
        return "It is too heavy for you!";
    }
    return 1;    
}

mixed direct_get_wrd_wrd_from_obj(string amt, string curr){
    return direct_get_wrd_wrd_out_of_obj(amt, curr);
}

void init(){
    ::init();
    if(!PileAmount || PileAmount < 1 ){
        SetLong("some money");
        call_out( (: eventDestruct :), 1);
    }
}
