#include <lib.h>

inherit LIB_STORAGE;

void create() {
    storage::create();
    SetKeyName("cardboard case");
    SetAdjectives( ({"small", "cardboard", "357", "ammo"}) );
    SetId( ({ "case" }) );
    SetShort("a small .357 ammo case");
    SetLong("A small cardboard case designed to carry .357 caliber amunition.");
    SetMass(10);
    SetDollarCost(1);
    SetMaxCarry(12);
}

mixed CanReceive(object ob){
    if(ob->GetKeyName() != ".357 round") {
        tell_object(this_player(),"This cardboard case is for .357 ammunition only.");
        return 0;
    }
    return storage::CanReceive(ob);
}

void init(){
    storage::init();
}
