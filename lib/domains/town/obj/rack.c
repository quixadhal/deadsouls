#include <lib.h>

inherit LIB_SURFACE;
object to;


void create() {
    ::create();
    SetKeyName("rack");
    SetId(({"rack"}));
    SetAdjectives(({"knife","metal","fancy","steel","stainless","stainless-steel"}));
    SetShort("a knife rack");
    SetLong("This is a fancy stainless steel knife rack, "+
            "for keeping expensive super-sharp knives.");
    SetMass(274);
    SetBaseCost("silver",50);
    SetMaxCarry(5);
    SetInventory(([
                //"/domains/town/weap/paring_knife" : 1,
                "/domains/town/weap/butcher_knife" : 1,
                "/domains/town/weap/carving_knife" : 1,
                ]));
    SetCanClose(0);
}
mixed CanGet(object ob) { return "The rack does not budge.";}
int CanReceive(object ob) {
    if(!ob->GetFancy()) {
        write("This is a rack for fine, expensive kitchen knives only.");
        return 0;
    }
    else return 1;
}
void init(){
    ::init();
}
