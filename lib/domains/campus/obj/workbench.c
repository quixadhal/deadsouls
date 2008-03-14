#include <lib.h>

inherit LIB_TABLE;

void create() {
    ::create();
    SetKeyName("workbench");
    SetAdjectives( ({"work", "simple", "flat"}) );
    SetId( ({"table", "workbench", "bench"}) );
    SetShort("a workbench");
    SetLong("A simple flat surface for analyzing materials.");
    SetPreventGet("The table does not budge.");
    SetInventory(([
        "/domains/town/armor/rocketpack" : 1,
        "/domains/default/obj/module_cloak" : 1,
        "/domains/default/armor/wristcomp" : 1,
        "/domains/town/armor/breather" : 1,
      ]));
    SetBaseCost("silver",1);
    SetMaxCarry(5000);

}
void init(){
    ::init();
}
