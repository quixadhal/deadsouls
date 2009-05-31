#include <lib.h>

inherit LIB_STORAGE;

void create() {
    ::create();
    SetKeyName("chest");
    SetAdjectives(({"wooden","heavy"}));
    SetId(({"chest"}));
    SetShort("a wooden chest");
    SetLong("This is a heavy wooden chest used to store items.");
    SetMaxCarry(9000);
    SetInventory(([
                "/domains/cave/obj/omni" : 1,
                "/domains/cave/armor/boobytrap_ring" : 1,
                "/domains/cave/armor/necklace" : 1,
                ]));
    SetMoney( ([
                //"dollars" : random(50)+50,
                "gold" : random(50)+50,
                "silver" : random(500)+500,
                "copper" : random(9000)+9000,
                "electrum" : random(100)+100,
                "platinum" : random(20)+20,
                ]) );
    SetMass(2000);
    SetBaseCost("silver",50);
    SetCanClose(1);
    SetClosed(1);
    SetPreventGet("The chest does not budge.");
}

void init(){
    ::init();
}
