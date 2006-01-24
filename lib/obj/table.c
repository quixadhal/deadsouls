#include <lib.h>

inherit LIB_TABLE;


void create() {
    ::create();
    SetKeyName("generic table");
    SetId( ({ "table","surface" }) );
    SetAdjectives( ({ "generic"}) );
    SetShort("a generic table");
    SetLong("It is a surface that can hold things.");
    SetBaseCost("silver",1);
    SetMaxCarry(500);

}
void init(){
    ::init();
}
