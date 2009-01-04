#include <lib.h>
inherit LIB_ITEM;

create() {
    ::create();
    SetId(({"sword","dull sword","a dull sword"}));
    SetKeyName("sword");
    SetShort("a dull sword");
    SetLong( "This sword has seen better days. It appears to have "
            "been sitting in the sewers a while and as a consequence it "
            "is rusted almost to the point of uselessness.");
    SetMass(500);   SetValue( 50);

    SetClass(7);
    SetWeaponType("blade");
    SetWield("A disgusting slime covers your hand as you "
            "wield the sword.");
}
