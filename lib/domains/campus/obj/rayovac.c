#include <lib.h>
#include <vendor_types.h>
inherit "/lib/flashlight";


void create(){
    ::create();
    SetKeyName("flashlight");
    SetId( ({"fl","flashlight","light","torch","flashlite"}) );
    SetAdjectives( ({"plastic","small","cheap","rayovac","Rayovac"}) );
    SetShort( "a small, plastic flashlight" );
    SetLong("This is a cheap Rayovac brand flashlight.");
    SetMass(20);
    SetDollarCost(10);
    SetLightLevel(6);
    SetMinCells(2);
    SetMaxCells(2);
    SetCellType("D");
    Lit=0;
    SetVendorType(VT_TREASURE);
    SetInventory(([
                "/domains/campus/obj/dcell_crappy":2
                ]));

}
void init(){
    ::init();
}
