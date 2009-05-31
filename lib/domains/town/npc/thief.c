#include <lib.h>

inherit LIB_NPC;

static void create() {
    npc::create();
    SetKeyName("thief");
    SetId(({"thief in a bathtowel","thief in a towel","thief"}));
    SetAdjectives(({"wet"}));
    SetShort("a thief in a bathtowel");
    SetLong("From his furtive appearance and demeanor, this "+
            "is obviously a thief who has snuck into the "+
            "mansion and taken up residence. You seem "+
            "to have caught him just after his shower.");
    SetLevel(1);
    SetRace("human");
    SetClass("thief");
    SetGender("male");
    SetEncounter(100);
    SetMaxHealthPoints(25);
    SetInventory(([
                "/domains/town/armor/towel":"wear towel",
                "/domains/town/weap/brush":"wield brush",
                "/domains/town/obj/safe_key":1,
                ]) );
    SetHealthPoints(50);
}
void init(){
    ::init();
}
