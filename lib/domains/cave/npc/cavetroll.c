#include <lib.h>
#include <damage_types.h>
#include <size_types.h>

inherit LIB_NPC;

static void create() {
    npc::create();
    SetKeyName("troll");
    SetId(({"cavetroll"}));
    SetAdjectives(({"dirty","cave"}));
    SetShort("a cave troll");
    SetLong("Unlike its more civilized brethren, the cavetroll is a "+
            "huge and monstrously powerful creature with an even more "+
            "vicious temperament. Its rocklike skin makes it difficult to "+
            "injure, and its gargantuan muscles make it difficult to "+
            "survive in combat against it.");
    SetInventory(([
                "/domains/cave/armor/loincloth" : "wear loincloth",
                ]));
    SetLevel(13);
    SetRace("troll");
    SetClass("fighter");
    SetGender("male");
    SetMelee(1);
    SetSkill("melee attack",50,50,10);
    SetStat("strength",90,1);
    SetStat("agility",50,1);
    SetResistance(BLADE,"high");
    SetResistance(KNIFE,"high");
    SetResistance(PIERCE,"high");
    SetResistance(BITE,"high");
    SetEncounter(100);
    SetSize(S_LARGE);
}

void init(){
    ::init();
}
