#include <lib.h>
#include "../../customdefs.h"

inherit LIB_TABLE;


void create() {
    ::create();
    SetKeyName("sample table");
    SetId( ({ "table" ,"sample table"}) );
    SetAdjectives( ({ "simple","sample" }) );
    SetShort("a simple table");
    SetLong("It is a simple sample table.");
    SetBaseCost(1200);
    SetMaxCarry(5000);
    SetInventory( ([
	MY_OBJ "/key" : 1,
      ]) );
}
void init(){
::init();
}