#include <lib.h>

inherit LIB_STORAGE;

void create() {
    ::create();
    SetKeyName("chest");
    SetId(({"chest","wooden chest"}));
    SetShort("a wooden chest");
    SetLong("This is a sturdy wooden chest used to store valuable items.");
    SetMass(2000);
    SetBaseCost(50);
    SetMaxCarry(500);
    SetPreventPut("You cannot put this in there!");
    SetInventory(([
	"/secure/obj/memo" : 1,
	"/secure/obj/medtric" : 1,
	"/secure/obj/control" : 1,
	"/secure/obj/roommaker" : 1,
	"/secure/obj/staff" : 1,
	"/secure/obj/machine" : 1,
	"/domains/default/armor/badge" : 1,
	"/domains/default/obj/manual" : 1,
      ]));
    SetCanClose(1);
    SetClosed(1);
}
mixed CanGet(object ob) { return "The chest does not budge.";}
