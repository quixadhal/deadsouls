#include <lib.h>
#include "../customdefs.h"

inherit LIB_SENTIENT;
inherit MY_AREA "/etc/call_cops";

static void create() {
    sentient::create();
    SetKeyName("horse");
    SetId(({"horse","large horse"}));
    SetShort("a large horse");
    SetLong("A large, hooved quadruped, taller and more muscular than you.");
    SetLevel(5);
    SetRace("horse");
    SetGender("male");
    SetMelee(1);
    SetAction(5, ({ 
	"The horse snorts.", "You hear the horse whinny.", 
	"The horse flips its tail in the air.",
	"The horse looks around."}));
}
