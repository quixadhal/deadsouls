inherit "/lib/std/room";

void create() {
    ::create();
    SetProperty("light", -1);
    SetProperty("indoors", 1);
    SetShort( "Inside a dark chamber");
    SetLong(
            "You are in a very dark chamber, illuminated only by your "
            "light source.  The stench in here is foul, and trapped.");
    SetItems(
            (["light" : "What light?",
             "tunnel" : "It is very dark.",
             "chamber" : "You cannot see it very well."]) );
    SetExits( (["west" : "/domains/Praxis/mountains/tunnel1"]) );
}

void reset() {
    object arm, mon;

    ::reset();
    if(!present("goblin")) {
        mon = new("/lib/npc");
        mon->SetKeyName("goblin");
        mon->SetId( ({ "soldier", "goblin", "goblin soldier"}) );
        mon->SetLevel(3);
        mon->SetShort( "Goblin soldier");
        mon->SetLong( "An ugly monster who has likely never "
                "seen the light of day.");
        mon->SetRace( "goblin");
        mon->SetAggressive( 10);
        mon->SetMorality(-140);
        mon->SetHealthPoints(240);
        mon->SetGender("female");
        mon->SetRace("human");
        mon->move(this_object());

        arm = new("/lib/std/armor");
        arm->SetKeyName("boot");
        arm->SetId( ({ "boot", "boot", "goblin boot" }) );
        arm->SetShort( "Right goblin boot");
        arm->SetLong( "A boot for right foot made from the rotting "
                "leather goblins can only find.");
        arm->set_type("boot");
        arm->set_limbs( ({ "right foot" }) );
        arm->set_ac(3);
        arm->set_mass(100);
        arm->set_value(100);
        arm->move(mon);
        mon->force_me("wear boot");
    }
}

