#include <lib.h>
#include <vendor_types.h>
inherit LIB_FLASHLIGHT;

void create(){
    ::create();
    SetKeyName("maglite");
    SetId( ({"fl","flashlight","light","flashlite","Maglite"}) );
    SetAdjectives( ({"powerful","heavy","large","Maglite"}) );
    SetShort( "a large flashlight" );
    SetLong("This is a powerful, heavy, Maglite brand flashlight.");
    SetMass(250);
    SetBaseCost("silver",40);
    SetLightLevel(20);
    SetMinCells(4);
    SetMaxCells(4);
    SetCellType("D");
    Lit=0;
    SetVendorType(VT_TREASURE);
    SetInventory(([
                "/domains/town/obj/dcell_good":4
                ]));

}

void init(){
    ::init();
}
