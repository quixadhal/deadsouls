#include <lib.h>

inherit LIB_SENTIENT;

static void create() {
    sentient::create();
    SetKeyName("unicorn");
    SetId(({"unicorn","large unicorn"}));
    SetShort("a beautiful white unicorn");
    SetLong("This white unicorn is a great, proud horse with a single ivory horn growing from its head. Long a symbol of masculine virility, many people still fail to see this large, muscular beast with a long, hard horn as the powerful sexual icon it is.");
    SetLevel(5);
    SetRace("unicorn");
    SetGender("male");
    SetMelee(1);
    SetAction(5, ({ 
                "The unicorn snorts.", "You hear the unicorn whinny.", 
                "The unicorn flips its tail in the air.",
                "The unicorn looks around."}));
}
void init(){
    ::init();
}
