#include <lib.h>

inherit LIB_STORAGE;

void create() {
    ::create();
    SetKeyName("locker");
    SetId(({"locker","object"}));
    SetAdjectives(({"metal","tall","large"}));
    SetShort("a tall metal locker");
    SetLong("This large object is designed to hold "+
            "armor and related accessories.");
    SetInventory(([
                "/domains/default/armor/pcannon" : 1,
                "/domains/default/armor/scoutsuit" : 1,
                "/domains/default/armor/chainmail" : 5,
                ]));
    SetMass(2000);
    SetBaseCost("silver",150);
    SetMaxCarry(10000);
    SetCanClose(1);
    SetClosed(0);
    SetPreventGet("The locker does not budge.");
}
void init(){
    ::init();
}
