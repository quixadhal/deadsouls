#include <lib.h>

inherit "/domains/cave/etc/cave_orc";

int GetEnemyChaser() { return 0; }

static void create() {
    ::create();
    SetKeyName("orc");
    SetId(({"orc"}));
    SetAdjectives(({"armored","professional"}));
    SetShort("an armored orc");
    SetLong("This is a large adult orc. He seems to have a professional "
            "bearing, and a sense of purpose.");
    SetInventory(([
                "/domains/cave/weap/axe" : "wield axe",
                "/domains/cave/armor/leather_armor" : "wear leather armor",
                "/domains/cave/armor/orc_boot_l" : "wear left boot",
                "/domains/cave/armor/orc_boot_r" : "wear right boot",
                ]));
    SetRace("orc");
    SetLevel(3);
    SetClass("fighter");
    SetGender("male");
    this_object()->SetGuard("northeast","The orc bars your way!");
}

void init(){
    ::init();
}
