#include <lib.h>

inherit "/domains/cave/etc/cave_orc";

int throwing;

static void create() {
    ::create();
    SetKeyName("orc");
    SetId(({"orc","guard"}));
    SetAdjectives(({"armored","professional","guard","orc"}));
    SetShort("an orc guard");
    SetLong("This is an adult orc. He seems to have a professional "
            "bearing, and a sense of purpose.");
    SetInventory(([
                "/domains/cave/weap/sword" : "wield sword",
                "/domains/cave/weap/javelin" : 3,
                "/domains/cave/armor/leather_armor" : "wear leather armor",
                "/domains/cave/armor/orc_boot_l" : "wear left boot",
                "/domains/cave/armor/orc_boot_r" : "wear right boot",
                ]));
    SetLevel(3);
    SetRace("orc");
    SetClass("fighter");
    SetWanderSpeed(1);
    SetSkill("projectile attack", 30);
    SetSkill("projectile defense", 10);
    SetGender("male");
    SetEncounter(  (: CheckOrc :) );
}

void init(){
    ::init();
}

void heart_beat(){
    object env = room_environment(this_object());
    if(throwing) throwing--;
    if(GetInCombat()){
        object targ;
        object jav = present("javelin",this_object());
        object *targs = filter(get_livings(env), 
                (: member_array($1, GetEnemies()) != -1 :) );
        if(sizeof(targs)) targ = targs[random(sizeof(targs)-1)];
        if(present("javelin", env)){
            eventForce("get a javelin");
            jav = present("javelin",this_object());
        }
        if(jav && targ && !throwing){
            eventForce("throw a javelin at "+targ->GetKeyName());
            throwing = 10;
        }
    }
    ::heart_beat();
}
