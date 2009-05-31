#include <lib.h>
#include "../customdefs.h"

inherit LIB_TABLE;


void create() {
    ::create();
    SetKeyName("sample table");
    SetId( ({ "table" ,"sample table"}) );
    SetAdjectives( ({ "simple","sample" }) );
    SetShort("a simple table");
    SetLong("It is a simple sample table. It is an example of a \"surface\" "+
            "type container.");
    SetBaseCost(1200);
    SetMaxCarry(5000);
    SetInventory( ([
                MY_OBJ "/key" : 1,
                MY_OBJ "/pitcher" : 1,
                MY_OBJ "/cup" : 1,
                ]) );
}
void init(){
    ::init();
}
