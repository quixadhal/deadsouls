#include <lib.h>

#include <armor_types.h>
#include <damage_types.h>
inherit LIB_WORN_STORAGE;

static void create() {
    ::create();
    SetKeyName("backpack");
    SetId(({"bag","pack"}));
    SetAdjectives(({"sturdy","green","olive","olive-green","od green","od-green"}));
    SetShort("a sturdy, olive-green backpack");
    SetLong("This is a large, sturdy backpack made of some sort of "
            "strong, canvas-like material. It appears suitable "+
            "for just about any adventure you'd find yourself in.");
    SetInventory(([
                "/domains/campus/obj/match" : 3,
                "/domains/campus/weap/sharpsword" : 1,
                "/domains/campus/armor/chainmail" : 1,
                "/domains/campus/meals/burger" : 3,
                "/domains/campus/armor/helmet" : 1,
                "/domains/campus/armor/shield" : 1,
                "/domains/campus/obj/maglite" : 1,
                "/domains/campus/armor/collar" : 1,
                "/domains/campus/meals/wimp_ale" : 3,
                "/domains/campus/obj/torch" : 1,
                ]));
    SetMass(100);
    SetBaseCost("silver", 40);
    SetCanClose(1);
    SetClosed(0);
    SetMaxCarry(500);
    SetDamagePoints(100);
    SetArmorType(A_VEST);

}
void init(){
    ::init();
}
