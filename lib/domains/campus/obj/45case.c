#include <lib.h>

inherit LIB_STORAGE;

void create() {
    storage::create();
    SetKeyName("cardboard case");
    SetAdjectives( ({"small", "cardboard", "45", "ammo", "ammunition"}) );
    SetId( ({ "case" }) );
    SetShort("a small .45 ammo case");
    SetLong("A small cardboard case designed to carry .45 caliber acp amunition.");
    SetMass(10);
    SetDollarCost(1);
    SetMaxCarry(12);
    SetInventory( ([
                "/domains/campus/obj/45round" : 12,
                ]) );
}

mixed CanReceive(object ob){
    if(ob->GetKeyName() != ".45 round") {
        write("This cardboard case is for .45 acp ammunition only.");
        return 0;
    }
    return storage::CanReceive(ob);
}

void init(){
    storage::init();
}
