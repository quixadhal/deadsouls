#include <lib.h>
#include <vendor_types.h>

inherit LIB_VENDOR;

int throwing;

int CheckOrc(mixed val){
    if(!val) return 0;
    if(!objectp(val)) return 0;
    if(val->GetRace() == "orc" || val->GetRace() == "rodent") return 0;
    eventForce("growl at "+val->GetKeyName());
    return 1;
}

static void create() {
    ::create();
    SetKeyName("orc");
    SetId(({"gorm","orc","shopkeep","keeper","vendor"}));
    SetAdjectives(({"adult","male","shop"}));
    SetShort("an orc shopkeep");
    SetLong("This is the manager of the local Post Exchange. You "
            "can sell things to him or buy things from him.");
    SetInventory(([
                ]));
    SetLevel(2);
    SetMelee(1);
    SetRace("orc");
    SetClass("fighter");
    SetGender("male");
    SetEncounter(  (: CheckOrc :) );
    AddCurrency("copper", random(100));
    SetVendorType(VT_ALL);
    SetAttackable(0);
    SetSkill("bargaining", 1);
    SetProperty("no bump", 1);
    SetLocalCurrency("copper");
    SetStorageRoom("/domains/cave/room/shop2");
}

void init(){
    object env = environment();
    ::init();
    if(env && base_name(env) == "/domains/cave/room/cave"){
        eventForce("go east");
    }
}

void heart_beat(){
    object env = room_environment(this_object());
    if(env && base_name(env) == "/domains/cave/room/cave"){
        eventForce("go east");
    }
    ::heart_beat();
}
