#include <lib.h>

inherit LIB_STORAGE;

void create() {
    ::create();
    SetKeyName("chest");
    SetId(({"chest","wooden chest"}));
    SetShort("a wooden chest");
    SetLong("This is a sturdy wooden chest used to store valuable items.");
    SetInventory(([
                "/domains/town/armor/collar" : 1,
                "/domains/default/obj/bguide" : 1,
                "/secure/obj/machine" : 1,
                "/domains/default/obj/meter" : 1,
                "/secure/obj/control" : 1,
                "/secure/obj/memo" : 1,
                "/secure/obj/staff" : 1,
                "/domains/default/obj/pinger" : 1,
                "/secure/obj/medtric" : 1,
                ]));
    SetMass(2000);
    SetBaseCost(50);
    SetMaxCarry(10000);
    SetPreventPut("You cannot put this in there!");
    SetPreventGet("The chest does not budge.");
    SetCanClose(1);
    SetClosed(1);
}

void init(){
    ::init();
}
