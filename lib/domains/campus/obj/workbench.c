#include <lib.h>

inherit LIB_TABLE;

void create() {
    ::create();
    SetKeyName("workbench");
    SetAdjectives( ({"work", "simple", "flat"}) );
    SetId( ({"table", "workbench", "bench"}) );
    SetShort("a workbench");
    SetLong("A simple flat surface for analyzing materials.");
    SetInventory(([
	"/domains/default/armor/wristcomp" : 1,
	"/domains/default/obj/module_cloak" : 1,
      ]));
    SetBaseCost("silver",1);
    SetMaxCarry(500);

}
void init(){
    ::init();
}
