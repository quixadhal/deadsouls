#include <lib.h>
#include <position.h>

inherit LIB_BARKEEP;

int CheckOrc(mixed val){
    if(!val) return 0;
    if(!objectp(val)) return 0;
    if(val->GetRace() == "orc" || val->GetRace() == "rodent") return 0;
    else eventForce("growl at "+val->GetKeyName());
    return 1;
}

static void create() {
    barkeep::create();
    SetKeyName("orc cook");
    SetAdjectives( ({ "orc" }) );
    SetId( ({ "rybak","cook" }) );
    SetShort("the orc cook");
    SetRace("orc");
    SetClass("fighter");
    SetLevel(15);
    SetLong("Bizarrely large, fit, and alert, this orc cook would "+
            "seem more in place in a uniform than serving food in a "+
            "commissary. You can: ask cook for menu");
    SetMelee(1);
    SetInventory(([
                "/domains/cave/armor/foodsmock" : "wear smock",
                ]));
    SetMenuItems(([
                ({ "roast beast", "beast" }) : "/domains/cave/meals/bread",
                ({ "black bread", "bread" }) : "/domains/cave/meals/beast",
                ]));
    SetGender("male");
    AddCurrency("electrum", random(200));
    SetSkill("bargaining", 1);
    SetProperty("no bump", 1);
    SetLocalCurrency("silver");
    SetEncounter(  (: CheckOrc :) );
}

void init(){
    ::init();
}

int eventList(object who, string cmd, string args){
    int ret = ::eventList(who, cmd, args);
    if(ret){
        eventForce("speak Help yourself to the grog and water. They're "+
                "free of charge.");
    }
    return ret;
}
