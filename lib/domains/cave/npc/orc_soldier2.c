#include <lib.h>

inherit "/domains/cave/etc/cave_orc";

int GetEnemyChaser() { return 1; }

static void create() {
    ::create();
    SetKeyName("orc");
    SetId(({"orc","soldier"}));
    SetAdjectives(({"orc","armored","professional","soldier"}));
    SetShort("an orc soldier");
    SetLong("This is a large orc soldier.");
    SetInventory(([
                "/domains/cave/weap/longsword" : "wield sword",
                "/domains/cave/armor/chainmail" : "wear chainmail",
                "/domains/cave/armor/orc_boot_l" : "wear left boot",
                "/domains/cave/armor/orc_boot_r" : "wear right boot",
                ]));
    SetRace("orc");
    SetLevel(7);
    SetClass("fighter");
    SetGender("male");
    SetEncounter(  (: CheckOrc :) );
    SetWanderSpeed(5);
}

void init(){
    ::init();
}
