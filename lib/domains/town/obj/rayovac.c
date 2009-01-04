#include <lib.h>
#include <vendor_types.h>
inherit "/lib/flashlight";

void create(){
    ::create();
    SetKeyName("flashlight");
    SetId( ({"fl","flashlight","light","torch","flashlite", "rayovac"}) );
    SetAdjectives( ({"plastic","small","cheap","rayovac","Rayovac"}) );
    SetShort("a small plastic flashlight");
    SetLong("This is a cheap Rayovac brand flashlight.");
    SetMass(10);
    SetBaseCost("silver",10);
    SetLightLevel(16);
    SetMinCells(2);
    SetMaxCells(2);
    SetCellType("D");
    Lit=0;
    SetVendorType(VT_TREASURE);
    SetInventory(([
                "/domains/town/obj/dcell_crappy":2
                ]));

}
void init(){
    ::init();
}
