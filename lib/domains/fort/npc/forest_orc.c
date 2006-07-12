#include <lib.h>

inherit LIB_SENTIENT;

static void create() {
    sentient::create();
    SetKeyName("orc");
    SetId(({"orc"}));
    SetAdjectives(({"armored","professional"}));
    SetShort("an armored orc");
    SetLong("This is a large adult orc. He seems to have a professional "
      "bearing, and a sense of purpose, unlike the ragged, chaotic "
      "orcs you've heard talk about. He almost appears to be on a mission.");
    SetPermitLoad(1);
    SetLevel(5);
    SetRace("orc");
    SetClass("explorer");
    SetGender("male");
    SetWanderSpeed(1);
    SetMelee(1);
    SetEncounter( 100 );
}
void init(){
    ::init();
}

varargs int eventDie(object agent){
    new(base_name(this_object()))->eventMove(environment(this_object()));
    new(base_name(this_object()))->eventMove(environment(this_object()));
    ::eventDie(agent);
}
