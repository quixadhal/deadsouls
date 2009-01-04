#include <lib.h>

inherit LIB_STORAGE;

void create() {
    ::create();
    SetKeyName("safe");
    SetId(({"safe"}));
    SetAdjectives(({"metal","small"}));
    SetShort("a metal safe");
    SetLong("This is a small safe, about 2 feet high, where "+
            "one may store valuables. It looks quite "+
            "strong.");
    SetMaxCarry(1000);
    SetInventory(([
                "/domains/campus/weap/grenade" : 1,
                "/domains/town/weap/9mil" : 1,
                "/domains/town/obj/slip_heal" : 4,
                ]));
    SetMass(1000);
    SetBaseCost("silver",50);
    SetCanClose(1);
    SetClosed(1);
    SetCanLock(1);
    SetLocked(1);
    SetLockStrength(80);
    SetKey("mansion safe key");
    AddMoney("silver",1000);
}

mixed CanGet(object ob) { return "The safe is bolted to the floor and does not move.";}
void init(){
    ::init();
}
