#include <lib.h>
#include <vendor_types.h>
inherit LIB_ITEM;
void create(){
    ::create();
    SetKeyName("slug");
    SetId( ({"slug","round","bullet","projectile"}) );
    SetAdjectives( ({"spent","lead"}) );
    SetShort("a spent firearms slug");
    SetLong("This small, twisted chunk of metal appears to be a spent "+
            "slug: the part of a bullet that is fired from a gun and enters the target. "+
            "Its shape has been distorted by its impact with whatever it hit.");
    SetMass(1);
    SetBaseCost("silver",0);
    SetVendorType(VT_TREASURE);
}
