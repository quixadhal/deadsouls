#include <lib.h>
inherit LIB_NPC;
static void create(){
    npc::create();
    SetKeyName("human champion");
    SetId( ({ "champion", "human","npc","warrior"}) );
    SetShort("a human champion");
    SetLong("This is a massive human warrior. His pectoral muscles "+
            "are clearly visible even through his shaggy fur and armor. His face is an angry "+
            "snarl of rage and hatred.");
    SetLevel(10);
    SetRace("human");
    SetClass("fighter");
    SetGender("male");
    SetInventory(([
                "/domains/campus/armor/chainmail.c" : "wear chainmail",
                //"/domains/campus/armor/shield.c" : "wear shield",
                "/domains/campus/weap/sharpsword.c" : "wield sword"
                ]));
    //SetMaxHealthPoints(550);
}
