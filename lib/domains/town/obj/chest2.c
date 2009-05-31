#include <lib.h>

inherit LIB_STORAGE;

void create() {
    ::create();
    SetKeyName("chest");
    SetId(({"chest","wooden chest"}));
    SetShort("a wooden chest");
    SetLong("This is a sturdy wooden chest used to store valuable items.");
    SetMass(500);
    SetBaseCost("silver",50);
    SetMaxCarry(500);
    SetPreventPut("You cannot put this in there!");
    SetInventory(([
                "/domains/town/obj/gcoinbag" : 1,
                "/domains/town/obj/maglite" : 1,
                "/domains/town/obj/m_key" : 1,
                ]));
    SetCanClose(1);
    SetClosed(1);
}

void init(){
    ::init();
}

mixed CanGet(object ob) { return "The chest does not budge.";}
