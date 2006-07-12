#include <lib.h>
#include "../customdefs.h";
inherit LIB_STORAGE;

void create() {
    ::create();
    SetKeyName("rack");
    SetId(({"wrack"}));
    SetAdjectives(({"wooden","large","weapon","weapons"}));
    SetShort("a wooden weapons rack");
    SetLong("This is a large wooden rack designed to contain "+
      "weapons. It is set into the wall.");
    SetMass(200);
    SetDollarCost(50);
    SetMaxCarry(5000);

    SetInventory(([
	MY_WEAP "/dagger" : 5,
	MY_WEAP "/sword" : 2,
	MY_WEAP "/sharpsword" : 1,
	MY_WEAP "/staff" : 1,
      ]));
    SetCanClose(1);
    SetClosed(0);

}
mixed CanGet(object ob) { return "The rack does not budge.";}
