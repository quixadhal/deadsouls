#include <lib.h>
#include <vendor_types.h>
inherit LIB_DONATE;
void create(){
    ::create();
    SetKeyName("donation box");
    SetId( ({"box","donation box"}) );
    SetAdjectives( ({"small","wood","wooden"}) );
    SetShort("a donation box");
    SetLong("This is a small donation box, made out of wood. "+
      "It is used to donate money, which is then used "+
      "to feed the hungry, clothe the naked, et cetera ad "+
      "nauseam. ");
    SetMass(20);
    SetBaseCost("silver",10);
    SetVendorType(VT_TREASURE);
    SetLocalCurrency("silver");
    SetOwner("Seventh Day Atheist");
}
