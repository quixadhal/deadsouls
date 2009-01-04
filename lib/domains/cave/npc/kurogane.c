#include <lib.h>

inherit LIB_SENTIENT;
int throwing;

int CheckOrc(mixed val){
    if(!val) return 0;
    if(!objectp(val)) return 0;
    if(val->GetRace() == "orc" || val->GetRace() == "rodent") return 0;
    eventForce("growl at "+val->GetKeyName());
    eventForce("say murdering, genocidal humanoids! Leave us alone!");
    return 1;
}

static void create() {
    ::create();
    SetKeyName("orc");
    SetId(({"orc","commander","kurogane"}));
    SetAdjectives(({"armored","professional","orc","commander"}));
    SetShort("an orc commander");
    SetLong("This is a very large adult orc. He seems to have a professional "
            "bearing, and a sense of purpose.");
    SetInventory(([
                "/domains/cave/weap/battleaxe" : "wield axe",
                "/domains/cave/armor/chainmail" : "wear chainmail",
                "/domains/cave/armor/orc_tallboot_l" : "wear left boot",
                "/domains/cave/armor/orc_tallboot_r" : "wear right boot",
                ]));
    SetRace("orc");
    SetLevel(10);
    SetClass("fighter");
    SetGender("male");
    SetEncounter(  (: CheckOrc :) );
    SetGuard("south","The commander bars your way!");
}

void init(){
    object env = environment();
    ::init();
    if(env && base_name(env) == "/domains/cave/room/cave"){
        eventForce("go east");
    }
}

void heart_beat(){
    object env = environment();
    ::heart_beat();
    if(env && base_name(env) == "/domains/cave/room/cave"){
        eventForce("go east");
    }
}
