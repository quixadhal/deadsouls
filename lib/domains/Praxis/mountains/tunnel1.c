#include <lib.h>

inherit LIB_ROOM;

void create() {
    ::create();
    SetProperty("light", 0);
    SetProperty("indoors", 1);
    SetProperty("no teleport", 1);
    SetShort( "A dark tunnel under the mountains");
    SetLong(
            "There is a weak light coming from up this descending tunnel, "
            "but it is not enough to illuminate this passage.  Chambers "
            "branch off of the main tunnel east and west.");
    SetItems(
            (["light" : "What light?",
             "tunnel" : "It is very dark.",
             "chamber" : "You cannot see it very well.",
             "chambers" : "There is one east, and one west, but you "
             "cannot make out anything in them."]) );
    SetExits( 
            (["east" : "/domains/Praxis/mountains/chamber1",
             "west" : "/domains/Praxis/mountains/chamber2",
             "down" : "/domains/Praxis/mountains/tunnel2",
             "up" : "/domains/Praxis/mountains/entrance"]) );
}

void reset() {
    object arm, mon;

    ::reset();
    if(!present("goblin")) {
        mon = new(LIB_NPC);
        mon->SetKeyName("goblin");
        mon->SetId( ({ "guard", "goblin", "goblin guard" }) );
        mon->SetLevel(4);
        mon->SetShort( "Goblin guard");
        mon->SetLong( "An ugly monster who has likely never seen "
                "the light of day.");
        mon->SetRace( "goblin");
        mon->set_stats("strength", 5);
        mon->SetAggressive( 10);
        mon->set_languages( ({ "goeblesque" }) );
        mon->set_speech(10, "goeblesque",
                ({ "Get the hell out of here, asshole!", "You will never get that "
                 "damn crystal ball!", "I will freaking kill you!"}), 0);
        mon->set_speech(20, "goeblesque", ({ "You are dead!", 
                    "Shouldn't have picked a fight with me!", "I will body slam you!",
                    "%&$*!head!" }), 1);
        mon->SetMorality(-120);
        mon->SetHealthPoints(400);
        mon->SetGender("male");
        mon->SetRace("human");
        mon->move(this_object());

        arm = new(LIB_ARMOR);
        arm->SetKeyName("chainmail");
        arm->SetId( ({ "chainmail", "suit of chainmail", "suit" }) );
        arm->SetShort( "A suit of chainmail");
        arm->SetLong( "A rusty old suit of chainmail.");
        arm->set_type("body armour");
        arm->set_limbs( ({ "torso", "left arm", "left leg", "right arm", "right leg" }) );
        arm->set_ac(5);
        arm->set_mass(900);
        arm->set_value(100);
        arm->move(mon);
        mon->force_me("wear suit");
    }
}

int CanReceive() {
    if(previous_object()->query_level() > 10) {
        message("my_action", "A magic force prevents you from going "
                "further into the mountain.", this_player());
        return 0;
    }
    return 1;
}
