#include <lib.h>

inherit LIB_BED;

void create() {
    ::create();
    SetKeyName("dining table");
    SetId( ({ "table" }) );
    SetAdjectives( ({ "very","long","dining","room","huge" }) );
    SetShort("a very long dining table");
    SetLong("This is a very long dining table, suitable for accommodating a few dozen guests. It is made of fine, polished wood.");
    SetMass(2000);
    SetBaseCost("silver",5000);
    SetMaxCarry(5000);
    inventory_visible();
    inventory_accessible();
    SetInventory( ([
                "/domains/town/obj/candlestick" : 2
                ]) );
}

mixed CanGet(object ob) { return "This table is much too large and heavy to go anywhere with you."; }

void init(){
    ::init();
}
