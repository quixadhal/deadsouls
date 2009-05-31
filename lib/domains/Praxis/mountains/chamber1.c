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
            "light source.  The stench in here is foul, and trapped.");
    SetItems(
            (["light" : "What light?",
             "tunnel" : "It is very dark.",
             "chamber" : "You cannot see it very well."]) );
    SetExits( 
            (["west" : "/domains/Praxis/mountains/tunnel1"]) );
}

void reset() {
    object arm, mon;

    ::reset();
    if(!present("goblin")) {
        mon = new(LIB_NPC);
        mon->SetKeyName("goblin");
        mon->SetId( ({ "soldier", "goblin", "goblin soldier" }) );
        mon->SetLevel(3);
        mon->SetShort( "Goblin soldier");
        mon->SetLong( "An ugly monster who has likely never "
                "seen the light of day.");
        mon->SetRace( "goblin");
        mon->SetAggressive( 10);
        mon->SetMorality(-140);
        mon->SetHealthPoints(240);
        mon->set_languages( ({ "goeblesque" }) );
        mon->set_speech(10, "goeblesque", ({ "Piss off!", "Die, asshole!" }),
                1);
        mon->SetGender("female");
        mon->SetRace("human");
        mon->move(this_object());
        arm = new(LIB_ARMOR);
        arm->SetKeyName("boot");
        arm->SetId( ({ "boot", "boot", "goblin boot" }) );
        arm->SetShort( "Right goblin boot");
        arm->SetLong( "A boot for right foot made from the rotting leather goblins can only find.");
        arm->set_type("boot");
        arm->set_limbs( ({ "right foot" }) );
        arm->set_ac(3);
        arm->set_mass(120);
        arm->set_value(50);
        arm->move(mon);
        mon->force_me("wear boot");
    }
}

