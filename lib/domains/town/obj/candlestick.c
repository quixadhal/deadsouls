#include <lib.h>
#include <vendor_types.h>
inherit LIB_ITEM;
void create(){
    ::create();
    SetKeyName("silver candlestick");
    SetId( ({"candlestick","candelabra","candelabrum"}) );
    SetAdjectives( ({"small","ornate","silver"}) );
    SetShort("a silver candlestick");
    SetLong("This is an small, ornate silver candlestick.");
    SetMass(20);
    SetBaseCost("silver",50);
    SetVendorType(VT_TREASURE);
}

void init(){
    ::init();
}
