#include <lib.h>

inherit LIB_ROOM;

void create() {
    ::create();
    SetProperty("light", -1);
    SetProperty("indoors", 1);
    SetProperty("no teleport", 1);
    SetProperty("no castle", 1);
    SetShort( "A dark underground chamber");
    SetLong(
            "You are in a very dark chamber, illuminated only by your "
            "light source.");
    SetSmell("default", "The stench in here is foul and trapped.");
    SetItems(
            (["light" : "What light?",
             "tunnel" : "It is very dark.",
             "chamber" : "You cannot see it very well."]) );
    SetExits( (["east" : "/domains/Praxis/mountains/tunnel2"]) );
}

void reset() {
    object arm, mon;

    ::reset();
    if(!present("goblin")) {
        mon = new(LIB_NPC);
        mon->SetKeyName("goblin");
        mon->SetId( ({ "general", "goblin", "goblin general" }) );
        mon->SetShort( "Goblin general");
        mon->SetLong( "An ugly monster who has likely never "
                "seen the light of day.");
        mon->SetLevel(5);
        mon->SetRace( "goblin");
        mon->SetAggressive( 10);
        mon->SetMorality(-200);
        mon->SetHealthPoints(300);
        mon->SetGender("male");
        mon->SetRace("human");
        mon->move(this_object());
        arm = new(LIB_ARMOR);
        arm->SetKeyName("shield");
        arm->SetId( ({ "shield", "goblin shield" }) );
        arm->SetShort( "Goblin shield");
        arm->SetLong( "A huge, heavy shield whose metal was mined "
                "from the Darow Mountains.");
        arm->set_type("shield");
        arm->set_limbs( ({ "left hand", "left arm", "torso" }) );
        arm->set_ac(3);
        arm->set_mass(500);
        arm->set_value(150);
        arm->move(mon);
        mon->force_me("wear shield");
    }
}

