#include <lib.h>

inherit LIB_SENTIENT;

static void create() {
    sentient::create();
    SetKeyName("ingrid");
    SetId(({"npc","mob","character","mobile"}));
    SetAdjectives(({"non-player", "non player"}));
    SetShort("Ingrid");
    SetLong("Ingrid is rather slight of build, but has a non-nonsense way about her that makes her seem much tougher than she physically looks.");
    SetInventory(([
                "/domains/town/armor/hshirt" : "wear shirt",
                "/domains/town/armor/workboot_l" : "wear left boot",
                "/domains/town/armor/bandanna" : "wear bandanna",
                "/domains/town/armor/workboot_r" : "wear right boot",
                "/domains/town/armor/workglove_r" : "wear right glove",
                "/domains/town/armor/jeans" : "wear jeans",
                "/domains/town/armor/workglove_l" : "wear left glove",
                "/domains/town/armor/lvest" : "wear vest",
                ]));
    SetLevel(1);
    SetMelee(1);
    SetRace("human");
    SetGender("female");
}
void init(){
    ::init();
}
