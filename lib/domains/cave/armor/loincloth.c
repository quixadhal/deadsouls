#include <lib.h>
#include <size_types.h>
#include <armor_types.h>
#include <damage_types.h>
inherit LIB_WORN_STORAGE;

static void create() {
    ::create();
    SetKeyName("loincloth");
    SetId(({"cloth"}));
    SetAdjectives(({"loin","filthy"}));
    SetShort("a filthy loincloth");
    SetLong("This is a very large cloth used by someone really gross as "+
            "a form of clothing and storage.");
    SetMass(100);
    SetCanClose(0);
    SetClosed(0);
    SetMaxCarry(100);
    SetArmorType(A_PANTS);
    SetSmell( ([
                "default" : "P.U.",
                ]) );
    SetMoney( ([
                "platinum" : random(5)+1,
                ]) );
    SetInventory(([
                "/domains/default/obj/vial_orange" : 1,
                "/domains/town/obj/cavetroll_key" : 1,
                ]));
    SetSize(S_LARGE);
}

void init(){
    ::init();
}
