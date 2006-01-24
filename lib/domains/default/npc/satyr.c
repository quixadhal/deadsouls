#include <lib.h>

inherit LIB_SENTIENT;

static void create() {
    sentient::create();
    SetKeyName("satyr");
    SetAdjectives( ({"wild", "ferocious", "looking", "male", "humanoid", "creature"}) );
    SetId( ({"male", "humanoid", "creature"}) );
    SetShort("a satyr");
    SetLong("This is a wild and ferocious looking humanoid male with the upper body of a human, but the lower body is goatish, woolly, and his legs are those of a hooved animal. Horns grow from beneath the wild mane of hair on his head. However human his face might be, his expression and demeanor make it clear this is not a domesticated person, but an untamed forest creature.");
    SetLevel(1);
    SetMelee(1);
    SetRace("satyr");
    SetGender("male");
}
void init(){
    ::init();
}
