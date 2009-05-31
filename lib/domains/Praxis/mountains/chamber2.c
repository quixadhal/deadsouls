#include <lib.h>

inherit LIB_ROOM;

void create() {
    ::create();
    SetProperty("light", -1);
    SetProperty("indoors", 1);
    SetProperty("no teleport", 1);
    SetProperty("no castle", 1);
    SetSmell("default", "The stench in here is foul and trapped.");
    SetShort( "A dark chamber under the mountains");
    SetLong(
            "You are in a very dark chamber, illuminated only by your "
            "light source.");
    SetItems(
            (["light" : "What light?",
             "tunnel" : "It is very dark.",
             "chamber" : "You cannot see it very well."]) );
    SetExits( 
            (["east" : "/domains/Praxis/mountains/tunnel1"]) );
}

void reset() {
    object arm, mon;

    ::reset();
    if(!present("goblin")) {
        mon = new(LIB_NPC);
        mon->SetKeyName("goblin");
        mon->SetId( ({ "soldier", "goblin", "goblin soldier" }) );
        mon->SetShort( "Goblin soldier");
        mon->SetLong( "An ugly monster who has likely never seen "
                "the light of day.");
        mon->SetLevel(3);
        mon->SetRace( "goblin");
        mon->SetAggressive( 10);
        mon->SetMorality(-140);
        mon->set_languages( ({ "goeblesque" }) );
        mon->set_speech(10, "goeblesque", ({ "Piss off.", "Go away or die!" }),
                1);
        mon->SetHealthPoints(240);
        mon->SetGender("female");
        mon->SetRace("human");
        mon->move(this_object());
        arm = new(LIB_ARMOR);
        arm->SetKeyName("boot");
        arm->SetId( ({ "left boot", "boot", "goblin boot" }) );
        arm->SetShort( "Left goblin boot");
        arm->SetLong( "A boot for left foot made from the "
                "rotting leather goblins can only find.");
        arm->set_type("boot");
        arm->set_limbs( ({ "left foot" }) );
        arm->set_ac(3);
        arm->set_mass(120);
        arm->set_value(50);
        arm->move(mon);
        mon->force_me("wear boot");
    }
}

