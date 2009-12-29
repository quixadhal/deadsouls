#include <lib.h>

inherit LIB_STORAGE;

void create() {
    ::create();
    SetKeyName("locker");
    SetId(({"locker"}));
    SetAdjectives(({"metal","tall","school"}));
    SetShort("a tall metal locker");
    SetLong("This is a large metal locker.");
    SetMass(3000);
    SetDollarCost(50);
    SetMaxCarry(500);
    SetInventory(([
                "/domains/campus/armor/bandanna": 1,
                "/domains/campus/armor/workboot_l": 1,
                "/domains/campus/armor/workboot_r": 1,
                "/domains/campus/armor/workglove_l": 1,
                "/domains/campus/armor/workglove_r": 1,
                "/domains/campus/armor/overalls": 1,
                "/domains/campus/armor/cap": 1,
                "/domains/campus/weap/crowbar": 1,
                ]));
    SetCanClose(1);
    SetClosed(0);
    SetCanLock(1);
    SetLocked(0);
    SetKey("locker_key_1");
}
mixed CanGet(object ob) { return "The locker does not budge.";}
void init(){
    ::init();
}
