#include <lib.h>

inherit LIB_STORAGE;

void create() {
    ::create();
    SetKeyName("chest");
    SetId(({"chest","wooden chest"}));
    SetShort("a wooden chest");
    SetLong("This is a sturdy wooden chest used to store valuable items.\n"+
            "It is an example of an opaque container.");
    SetInventory(([
                "/domains/town/armor/collar" : 1,
                "/domains/default/armor/breather" : 1,
                "/domains/default/armor/badge" : 1,
                "/domains/default/armor/wizard_hat" : 1,
                "/domains/default/obj/manual" : 1,
                "/secure/obj/machine" : 1,
                "/domains/default/obj/gps" : 1,
                "/domains/default/armor/robe" : 1,
                "/domains/default/obj/meter" : 1,
                "/secure/obj/control" : 1,
                "/secure/obj/memo" : 1,
                "/secure/obj/staff" : 1,
                "/domains/default/obj/pinger" : 1,
                "/secure/obj/medtric" : 1,
                "/secure/obj/glasses" : 1,
                "/domains/default/armor/jade_ring" : 1,
                "/domains/default/armor/gray_amulet" : 1,
                ]));
    SetMass(2000);
    SetBaseCost(50);
    SetMaxCarry(10000);
    SetCanClose(1);
    SetClosed(1);
    SetPreventGet("The chest does not budge.");
}

void init(){
    ::init();
}
