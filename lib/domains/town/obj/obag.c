#include <lib.h>

#include <armor_types.h>
#include <damage_types.h>
inherit "/lib/std/storage";

static void create() {
    ::create();
    SetKeyName("overnight");
    SetId(({"bag"}));
    SetAdjectives(({"overnight","canvas"}));
    SetShort("an overnight bag");
    SetLong("This is a medium-sized bag made of soft "
            "canvas, made for holding useful items.");
    SetMaxCarry(450);
    SetInventory(([
                "/domains/town/armor/mhelmet" : 1,
                "/domains/town/armor/tshirt" : 1,
                "/domains/town/armor/pants" : 1,
                "/domains/town/armor/glove_r" : 1,
                "/domains/town/obj/lockpick" : 1,
                "/domains/town/armor/jacket" : 1,
                "/domains/town/armor/glove_l" : 1,
                "/domains/town/obj/needle_trap" : 1,
                ]));
    SetMass(100);
    SetBaseCost("silver",30);
    SetCanClose(1);
    SetClosed(1);
}

void init(){
    ::init();
}
