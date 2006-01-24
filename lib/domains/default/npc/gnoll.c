#include <lib.h>

inherit LIB_SENTIENT;

static void create() {
    sentient::create();
    SetKeyName("gnoll");
    SetAdjectives( ({"filthy", "horrendous"}) );
    SetId( ({"dogman", "dog-man", "dog man"}) );
    SetShort("a large, filthy dogman");
    SetLong("This appears to be a horrendous cross between a humanoid and a hyena. Its matted fur is streaked with blood and feces, and its slavering jaws seem never to stop dripping saliva.");
    SetLevel(1);
    SetMelee(1);
    SetRace("gnoll");
    SetGender("male");
}
void init(){
    ::init();
}
