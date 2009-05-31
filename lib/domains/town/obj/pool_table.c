#include <lib.h>

inherit LIB_BED;

void create() {
    ::create();
    SetKeyName("pool table");
    SetId( ({ "table" ,"billiards table"}) );
    SetAdjectives( ({ "billiard", "wood","wooden", "pool", "billiards" "large" }) );
    SetShort("a billiards table");
    SetLong("It is a large table made of dark, heavy wood. Its surface is covered in green felt and there are six holes in the corners and sides, through which balls may fit.");
    SetMass(10000);
    SetBaseCost("silver",1200);
    SetMaxCarry(5000);
    inventory_visible();
    inventory_accessible();
    SetInventory( ([
                "/domains/town/weap/cue" : 2,
                "/domains/town/obj/pool_ball" : 16,
                ]) );
}
void init(){
    ::init();
}
