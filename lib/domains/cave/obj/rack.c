#include <lib.h>

inherit LIB_SURFACE;
object to;


void create() {
    ::create();
    SetKeyName("rack");
    SetId(({"rack"}));
    SetAdjectives(({"knife","metal","fancy","steel","stainless","stainless-steel"}));
    SetShort("a mug rack");
    SetLong("This is a rack with pegs for cups and mugs.");
    SetMass(274);
    SetBaseCost("silver",50);
    SetMaxCarry(100);
    SetInventory(([
                "/domains/cave/obj/mug" : 4,
                "/domains/cave/obj/stein" : 1,
                ]));
    SetCanClose(0);
}
mixed CanGet(object ob) { return "The rack does not budge.";}
int CanReceive(object ob) {
    if(!inherits(LIB_FLASK,ob)){
        write("This is a rack for drinking utensils only.");
        return 0;
    }
    else return 1;
}
void init(){
    ::init();
}
