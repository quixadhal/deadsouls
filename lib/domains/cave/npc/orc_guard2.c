#include <lib.h>

inherit "/domains/cave/etc/cave_orc";

static void create() {
    ::create();
    SetKeyName("orc");
    SetId(({"orc"}));
    SetAdjectives(({"armored","professional"}));
    SetShort("an armored orc");
    SetLong("This is an adult orc. He seems to have a professional "
            "bearing, and a sense of purpose.");
    SetInventory(([
                "/domains/cave/weap/sword" : "wield sword",
                "/domains/cave/armor/leather_armor" : "wear leather armor",
                "/domains/cave/armor/orc_boot_l" : "wear left boot",
                "/domains/cave/armor/orc_boot_r" : "wear right boot",
                ]));
    SetRace("orc");
    SetLevel(2);
    SetClass("fighter");
    SetGender("male");
    SetEncounter(  (: CheckOrc :) );
}

void init(){
    ::init();
}
