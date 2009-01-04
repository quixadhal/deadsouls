#include <lib.h>

inherit LIB_ROOM;

void create() {
    ::create();
    SetProperty("light", -1);
    SetProperty("indoors", 1);
    SetProperty("no castle", 1);
    SetProperty("no teleport", 1);
    SetShort( "A chamber underneath the Daroq Mountains");
    SetLong(
            "You are in a very dark chamber, illuminated only by your "
            "light source.");
    SetSmell("default", "The stench of decay is all around.");
    SetItems(
            (["light" : "What light?",
             "tunnel" : "It is very dark.",
             "chamber" : "You cannot see it very well."]) );
    SetExits( (["west" : "/domains/Praxis/mountains/tunnel2"]) );
}

void reset() {
    object arm, mon;

    ::reset();
    if(!present("goblin")) {
        mon = new(LIB_NPC);
        mon->SetKeyName("goblin");
        mon->SetId( ({ "colonel", "goblin", "goblin colonel" }) );
        mon->SetShort( "Goblin colonel");
        mon->SetLong( "An ugly monster who has likely never seen "
                "the light of day.");
        mon->SetLevel(4);
        mon->SetRace( "goblin");
        mon->SetAggressive( 10);
        mon->SetMorality(-180);
        mon->SetHealthPoints(275);
        mon->SetGender("female");
        mon->SetRace("human");
        mon->move(this_object());

        arm = new(LIB_ARMOR);
        arm->SetKeyName("visor");
        arm->SetId( ({ "visor", "goblin visor" }) );
        arm->SetShort( "Goblin visor");
        arm->SetLong( "A metallic visor with opening just "
                "large enough to see through.");
        arm->set_type("visor");
        arm->set_limbs( ({ "head" }) );
        arm->set_ac(1);
        arm->set_mass(100);
        arm->set_value(100);
        arm->move(mon);
        mon->force_me("wear visor");
    }
}

