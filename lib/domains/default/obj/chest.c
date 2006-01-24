#include <lib.h>

inherit LIB_STORAGE;

void create() {
    ::create();
    SetKeyName("chest");
    SetId(({"chest","wooden chest"}));
    SetShort("a wooden chest");
    SetLong("This is a sturdy wooden chest used to store valuable items.");
    SetInventory(([
	"/secure/obj/control" : 1,
	"/domains/default/armor/wizard_hat" : 1,
	"/domains/default/armor/robe" : 1,
	"/domains/town/armor/collar" : 1,
	"/secure/obj/machine" : 1,
	"/domains/default/armor/badge" : 1,
	"/domains/default/obj/manual" : 1,
	"/secure/obj/memo" : 1,
	"/domains/default/obj/meter" : 1,
	"/domains/town/obj/pack" : 1,
	"/secure/obj/medtric" : 1,
	"/secure/obj/staff" : 1,
      ]));
    SetMass(2000);
    SetBaseCost(50);
    SetMaxCarry(500);
    SetPreventPut("You cannot put this in there!");
    SetCanClose(1);
    SetClosed(1);
}
mixed CanGet(object ob) { return "The chest does not budge.";}
void init(){
    ::init();
}
