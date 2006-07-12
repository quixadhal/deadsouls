#include <lib.h>
#include "../customdefs.h"

inherit LIB_SENTIENT;

static void create() {
    ::create();
    SetKeyName("alley cat");
    SetId(({"jones","cat","alley cat"}));
    SetAdjectives(({"jones the","dirty","alley"}));
    SetShort("an alley cat");
    SetLong("A rather dirty-looking cat, on the prowl for food.");
    SetInventory(([
      ]));
    SetLevel(1);
    SetMaxHealthPoints(5);
    SetRace("cat");
    SetGender("male");
    SetWanderSpeed(5);
    SetMessage("come","$N slinks in.");
    SetMessage("leave","$N wanders $D.");
    SetAction(5, ({ 
	"The cat meows.", "The cat yowls!", 
	"A dirty alley cat brushes against your leg.",
	"You hear tiny licking sounds from the cat."}));
}
void init(){
    ::init();
}
