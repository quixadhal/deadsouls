#include <lib.h>

inherit LIB_SENTIENT;

static void create() {
    sentient::create();
    SetKeyName("gnome");
    SetAdjectives( ({"cute", "cuddly", "endearing"}) );
    SetId(({"npc","mob","character","mobile"}));
    SetShort("a cute little gnome");
    SetLong("This cuddly creature is rather like a dwarf, only much smaller and better-disposed. His long white beard, infectious laughter and twinkling eyes remind you of someone you knew and loved long ago. His cone-shaped hat and old-fashioned, awkward manner of dress make him all the more endearing. You just want to hug him and put him in your garden.");
    SetLevel(1);
    SetMelee(1);
    SetRace("gnome");
    SetGender("male");
}
void init(){
    ::init();
}
