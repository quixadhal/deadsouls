#include <lib.h>
#include <objects.h>

inherit LIB_TRAINER;
void create(){
    trainer::create();
    SetKeyName("radagast");
    SetId("radagast the brown","wizard","mage","trainer");
    SetAdjectives(({"friendly","tall","thin","old"}));
    SetGender("male");
    SetRace("human");
    SetNoSpells(1);
    SetShort("Radagast the Brown");
    SetLong("This is a tall, thin old man. Not much is known of "+
            "this mysterious wizard, other than his legendary kindness "+
            "to animals and his controversial reputation as a seditious "+
            "rebel. He can train you in the arts of magic defense, magic "+
            "attack, and conjuring. You may, for example, \"ask radagast "+
            "to train conjuring\". If you lack training points, then "+
            "do some adventuring and earn a promotion from Dirk. You will "+
            "then be awarded training points."); 
    SetClass("mage");
    SetLevel(40);
    AddTrainingSkills( ({ "magic defense", "magic attack", "conjuring" }) );
    SetSpellBook( ([ "buffer" : 100, "meditate" : 100, "missile" : 100, "fireball" : 100 ]) );
    SetPolyglot(1);
    SetCustomXP(10);
}

void init() {
    trainer::init();
    SetSmell(([  "default" : "A rather odd, musty smell."]));
    SetListen(([  "default" : "Radagast seems to be constantly "+
                "humming a quiet tune to himself."]));
}

int eventHelp(object who, string unused){
    ::eventHelp(who);
    this_object()->eventForce("speak Please note I only understand "+
            "English. If you're speaking to me in another language, I will "+
            "not understand!");
    return 1;
}
