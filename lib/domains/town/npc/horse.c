#include <lib.h>

inherit LIB_SENTIENT;

static void create() {
    sentient::create();
    SetKeyName("horse");
    SetId(({"horse","large horse","hands","quadruped","ungulate"}));
    SetAdjectives(({"large","hooved","mr","mister"}));
    SetShort("a large horse");
    SetLong("A large, hooved quadruped, taller and more muscular than you. Perhaps if you befriend him, he'll allow you to mount him.");
    SetNoClean(1);
    SetWimpy(50);
    SetLevel(5);
    SetRace("horse");
    SetGender("male");
    SetMelee(1);
    //SetAction(5, ({ 
    //	"The horse snorts.", "You hear the horse whinny.", 
    //	"The horse flips its tail in the air.",
    //	"The horse looks around."}));
    SetMount(1);
    SetCanBefriend(1);
}
void init(){
    ::init();
}
