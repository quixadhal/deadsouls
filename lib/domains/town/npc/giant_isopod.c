#include <lib.h>

inherit LIB_SENTIENT;

static void create() {
    sentient::create();
    SetKeyName("isopod");
    SetAdjectives( ({"huge", "giant"}) );
    SetId(({"arthropod","creature"}));
    SetShort("a giant isopod");
    SetLong("A two meter long arthropod, this huge creature sports claws, multiple limbs, and a mandibled head grotesque to most humanoids.");
    SetMelee(1);
    SetLevel(1);
    SetRace("isopod");
    SetClass("explorer");
    SetGender("female");
    SetEncounter(100);
    SetGuard("up", "The giant isopod blocks your path!");
}
void init(){
    ::init();
}
