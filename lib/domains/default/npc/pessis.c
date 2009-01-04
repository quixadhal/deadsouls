#include <lib.h>

inherit LIB_SENTIENT;

static void create() {
    sentient::create();
    SetKeyName("pessis");
    SetId( ({"half-orc", "half orc", "half human", "half-human"}) );
    SetAdjectives(({"non-player", "non player"}));
    SetShort("Pessis the half-orc");
    SetLong("Too rough and brutish looking to be a handsome human, too delicate to be a proper orc, Pessis has lived through great indignity and rejection his entire existence. He's understandably a bit defensive about it, so perhaps it's best not to bring up the subject.");
    SetInventory(([
                "/domains/default/armor/horc_shirt" : "wear tshirt",
                ]));
    SetLevel(1);
    SetMelee(1);
    SetRace("half-orc");
    SetGender("male");
}
void init(){
    ::init();
}
