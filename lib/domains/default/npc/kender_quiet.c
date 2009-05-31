#include <lib.h>

inherit LIB_SENTIENT;

int StealAttempt(){
    object *players = get_livings(environment(this_object()),1);
    object *stuff;
    object target;
    string name, thing;
    int which, whether;

    if(sizeof(players)){
        which = random(sizeof(players));
        target = players[which];
        name = target->GetKeyName();
        stuff = all_inventory(target);
        if(!sizeof(stuff) || random(100) > 50) whether = 1;
        if(whether) this_object()->eventForce("steal money from "+name);
        else {
            thing = stuff[random(sizeof(stuff))]->GetKeyName();
            this_object()->eventForce("steal "+thing+" from "+name);
        }
    }
    whether = 0;
    return 1;
}

static void create() {
    sentient::create();
    SetKeyName("kender");
    SetId(({"npc","mob","character","mobile"}));
    SetAdjectives(({"non-player", "non player"}));
    SetShort("a kender");
    SetLong("Slight of build and rather attractive, kenders are another race related to hobbits, with all of the playfulness and a little extra disregard for other people's privacy. Kenders are infamous for their extraordinary skill in relieving others of their property, and it is said they are taught to steal before being taught to read. Others dispute this, arguing kenders don't read at all.");
    SetClass("thief");
    SetLevel(5);
    SetMelee(1);
    SetRace("kender");
    SetGender("female");
    SetAction(25, ({
                (: StealAttempt :),
                }) );
}
void init(){
    ::init();
}
