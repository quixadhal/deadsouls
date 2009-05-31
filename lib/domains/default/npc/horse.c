#include <lib.h>

inherit LIB_SENTIENT;
inherit LIB_MOUNT;
inherit LIB_DOMESTICATE;

static void create() {
    sentient::create();
    SetKeyName("horse");
    SetId(({"horse","large horse"}));
    SetShort("a large horse");
    SetLong("A large, hooved quadruped, taller and more muscular than you.");
    SetWimpy(50);
    SetLevel(5);
    SetRace("horse");
    SetGender("male");
    SetMelee(1);
    SetAction(5, ({ 
                "The horse snorts.", "You hear the horse whinny.", 
                "The horse flips its tail in the air.",
                "The horse looks around."}));
    SetNoClean(1);
}
void init(){
    ::init();
}
