#include <lib.h>

inherit LIB_SENTIENT;

static void create() {
    sentient::create();
    SetKeyName("zafo");
    SetId(({"dude"}));
    SetAdjectives(({"punk rock", "punkrock", "rock", "cool"}));
    SetShort("Zafo");
    SetLong("Zafo is a cool dude, and totally punk rock.");
    SetPolyglot(1);
    SetLevel(10);
    SetRace("human");
    SetClass("fighter");
    SetGender("male");
    SetInventory( ([
        "/domains/town/armor/jacket" : "wear jacket",
        "/domains/town/armor/pants" : "wear pants",
        "/domains/town/armor/boot_l" : "wear left boot",
        "/domains/town/armor/boot_r" : "wear right boot",
      ]) );
    SetTalkResponses( ([  
        "s up" : "Nothin'.",
        "re you rebelling against" : "Waddaya got?",
      ]) );
}
void init(){
    sentient::init();
}
