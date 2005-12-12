#include <lib.h>

#include <armor_types.h>
#include <damage_types.h>
inherit LIB_STORAGE;
inherit LIB_ARMOR;

static void create() {
    ::create();
    SetKeyName("backpack");
    SetId(({"pack"}));
    SetAdjectives(({"leather","soft","brown"}));
    SetShort("a soft, brown leather backpack");
    SetLong("This is a medium-sized backpack made of soft brown leather. "+
      "It seems suitable for carrying books around, for the busy college student. "+
      "It features wide, comfortable straps for convenient wear.");
    SetMass(100);
    SetBaseCost("silver",30);
    SetCanClose(1);
    SetClosed(1);
    SetMaxCarry(500);
    SetDamagePoints(100);
    SetArmorType(A_CLOAK);
}
