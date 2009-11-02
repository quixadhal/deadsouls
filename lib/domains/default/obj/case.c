#include <lib.h>

inherit LIB_STORAGE;

void create() {
    ::create();
    SetKeyName("case");
    SetId( ({ "case" }) );
    SetAdjectives( ({ "mounted","wall","glass", "display" }) );
    SetShort("a glass display case");
    SetLong("This is a glass case designed to contain and display firearms.");
    SetInventory(([
                "/domains/default/weap/m16rifle" : 1,
                "/domains/default/weap/9mil" : 1,
                "/domains/default/weap/357pistol" : 1,
                "/domains/default/weap/prifle" : 1,
                ]));
    SetOpacity(0);
    SetCanClose(1);
    SetClosed(1);
    SetMass(274);
    SetBaseCost("silver",1);
    SetMaxCarry(1500);
}
void init(){
    ::init();
}

int CanReceive(object ob) {
    if(!inherits(LIB_FIREARM,ob)) {
        write("This is a case for firearms only.");
        return 0;
    }
    else return 1;
}

mixed CanGet(object ob) { return "The case does not budge.";}
