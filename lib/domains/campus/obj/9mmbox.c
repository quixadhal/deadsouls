#include <lib.h>

inherit LIB_STORAGE;

void create() {
    storage::create();
    SetKeyName("cardboard case");
    SetId(({"case","box"}));
    SetAdjectives(({"small", "cardboard", "9mm", "9 millimeter", "ammo"}));
    SetShort("a small 9mm ammo case");
    SetLong("A small cardboard case designed to carry 9mm ammunition.");
    SetMass(10);
    SetDollarCost(1);
    SetMaxCarry(36);
    SetCanClose(1);
    SetClosed(0);
    SetInventory( ([
                "/domains/campus/obj/9mmround": 30
                ]) );
}

mixed CanReceive(object ob){
    if(ob->GetKeyName() != "9mm round") {
        tell_object(this_player(),"This cardboard case is for 9mm ammunition only.");
        return 0;
    }
    return storage::CanReceive(ob);
}

void init(){
    storage::init();
}
