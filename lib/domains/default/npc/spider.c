#include <lib.h>
#include <size_types.h>

inherit LIB_SENTIENT;

static void create() {
    sentient::create();
    SetKeyName("spider");
    SetAdjectives( ({"huge", "hairy"}) );
    SetId(({"npc","mob","character","mobile"}));
    SetShort("a huge spider");
    SetLong("This is a humongous hairy spider about the size of a housecat. It looks absolutely vile and it has some kind of viscous fluid is dripping from its mouthparts.");
    SetWimpy(80);
    SetMelee(1);
    SetLevel(1);
    SetRace("arachnid");
    SetClass("explorer");
    SetGender("male");
}
void init(){
    ::init();
}
