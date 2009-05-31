#include <lib.h>

inherit LIB_ROOM;

void create() {
    ::create();
    SetProperty("light", -1);
    SetProperty("indoors", 1);
    SetProperty("no teleport", 1);
    SetProperty("no castle", 1);
    SetShort( "Deep under the Daroq Mountains");
    SetLong(
            "The tunnel is pitch black, save for your source of light.  "
            "A pair of chambers spread off east and west.");
    SetItems(
            (["light" : "What light?",
             "tunnel" : "It is very dark.",
             "chamber" : "You cannot see it very well.",
             "chambers" : "There is one east, and one west, but you "
             "cannot make out anything in them."]) );
    SetExits( 
            (["east" : "/domains/Praxis/mountains/chamber3",
             "west" : "/domains/Praxis/mountains/chamber4",
             "down" : "/domains/Praxis/mountains/tunnel3",
             "up" : "/domains/Praxis/mountains/tunnel1"]) );
}

void reset() {
    object arm, mon;

    ::reset();
    if(!present("goblin")) {
        mon = new(LIB_NPC);
        mon->SetKeyName("goblin");
        mon->SetId( ({ "guard", "goblin", "goblin guard" }) );
        mon->SetShort( "Goblin guard");
        mon->SetLong( "An ugly monster who has likely never seen "
                "the light of day.");
        mon->SetLevel(4);
        mon->SetRace( "goblin");
        mon->SetAggressive( 10);
        mon->set_languages( ({ "goeblesque" }) );
        mon->set_speech(10, "goeblesque", ({ "I can get you in just one shot.",
                    "You do not stand a chance!", "Get out quick or die!" }), 1);
        mon->SetMorality(-120);
        mon->SetHealthPoints(200);
        mon->SetGender("male");
        mon->SetRace("human");
        mon->move(this_object());

        arm = new(LIB_ARMOR);
        arm->SetKeyName("ring");
        arm->SetId( ({ "ring", "ring of darkness" }) );
        arm->SetShort( "Ring of darkness");
        arm->SetLong( "A featureless ring made from black obsidian.");
        arm->set_type("ring");
        arm->set_limbs( ({ ({ "right hand", "left hand", "first hand", "second hand", "third hand", "fourth hand" }) }) );
        arm->set_ac(1);
        arm->set_mass(60);
        arm->set_value(100);
        arm->move(mon);
        mon->force_me("wear ring on right hand");
    }
}


