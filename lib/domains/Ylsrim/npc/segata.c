#include <lib.h>
#include <objects.h>

inherit  LIB_TRAINER;
void create(){
    trainer::create();
    SetKeyName("segata");
    SetId(({"segata", "sanshiro", "segata sanshiro"}));
    SetAdjectives(({"stocky","built","strong","strongly built"}));
    SetGender("male");
    SetRace("human");
    SetShort("Segata Sanshiro");
    SetLong("Stocky and strongly built, this legendary master of "
            "martial arts helps train fighters to become the very best "
            "they can be, so that they may fulfill their duty. Segata "
            "Sanshiro specializes in melee (unarmed) combat. For armed "
            "combat training, guildmaster Roshd is still the trainer.");
    SetClass("fighter");
    SetLevel(40);
    AddTrainingSkills( ({ "melee attack", "melee defense" }) );
    SetSkill("melee attack", 100);
    SetSkill("melee defense", 100);
    SetPolyglot(1);
    SetLanguage("Common", 100);
    SetDefaultLanguage("Common");
    SetGuard("/domains/Ylsrim/obj/cask","Segata Sanshiro prevents your theft.");
    SetUnique(1);
}

void init() {
    trainer::init();
}
