#include <lib.h>

inherit LIB_NPC;

static void create() {
    npc::create();
    SetKeyName("orc");
    SetId(({"orc"}));
    SetAdjectives(({"dirty"}));
    SetShort("a dirty orc");
    SetLong("This orc is typical of its breed: nasty, brutish, and short. It appears "
            "to be a juvenile or adolescent, making it somewhat less "
            "dangerous but more hostile. ");
    SetLevel(1);
    SetRace("orc");
    SetGender("male");
    SetMaxHealthPoints(100);
    SetEncounter(0);
    SetInventory(([
                "/domains/default/weap/axe":"wield axe",
                ]) );

}
void init(){
    ::init();
}
