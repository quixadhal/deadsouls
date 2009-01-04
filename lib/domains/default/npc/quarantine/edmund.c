#include <lib.h>

inherit LIB_NPC;


static void create() {
    npc::create();
    SetKeyName("Edmund");
    SetId(({"edmund"}));
    SetShort("Edmund");
    SetLong("Edmund is a foreign exchange student from the United Kingdom.");
    SetLevel(5);
    SetRace("human");
    SetGender("male");
    SetNoClean(1);
}
void init(){
    ::init();
}
