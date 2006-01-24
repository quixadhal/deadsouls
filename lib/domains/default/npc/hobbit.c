#include <lib.h>

inherit LIB_SENTIENT;

static void create() {
    sentient::create();
    SetKeyName("hobbit");
    SetAdjectives( ({"saucy", "saucy looking", "stout", "somewhat stout", "capable"}) );
    SetId( ({"lass", "adventurer"}) );
    SetShort("a hobbit");
    SetLong("About three feet tall, somewhat stout, and with a jolly twinkle in her eye, this saucy looking hobbit lass looks just as capable as any adventurer.");
    SetLevel(1);
    SetMelee(1);
    SetRace("hobbit");
    SetGender("female");
}
void init(){
    ::init();
}
