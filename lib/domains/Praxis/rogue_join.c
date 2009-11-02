inherit "/lib/std/room";
int go_down();

void init() {
    ::init();
    add_action("become", "become");
    add_action("preview","preview");
}

void create() {
    ::create();
    SetProperty("light", 1);
    SetProperty("indoors", 1);
    SetShort( "Rogue Hut");
    SetLong( 
            "You have stumbled upon the hideout of the Nightmare rogues. "
            "What appears to be a worn down shack on the outside is actually "
            "a well put together fortress on the inside.  A magic "
            "%^BLUE%^blue%^RESET%^ light "
            "guards the entrance to the heart of the fortress down a flight "
            "of stairs.  The Grand Master Rogue waits here to initiate new "
            "slime into the vile and contemptable class of rogues. "
            "<preview> will tell you about becoming a rogue.");
    SetItems(
            (["hideout" : "Rogues come here to plot wicked acts.",
             "shack" : "It is not really a shack, but a fortress.",
             "fortress" : "The rogues make this place their hideout.",
             "light" : "Only rogues can pass beyond it.",
             "stairs" : "They lead down into the heart of the rogue hideout.",
             ({"rogue", "master", "grand master rogue"}) : "He initiates "
             "new rogues."]) );
    SetExits( ([
                "west" : "/domains/Praxis/rain_forest.c",
                ]) );
    AddExit("down","/domains/Praxis/rogue_hall",(:go_down:));
}

int preview() {
    if(this_player()->query_class() != "explorer") {
        write("This is not for you.");
        return 1;
    }
    say(this_player()->query_cap_name()+" seeks to learn about rogues.", this_player());
    write("Welcome, explorer!\n");
    write("Rogues are people who make their life and pleasure from the misery "
            "of others.  They keep their hideout here in the Outlands so as to "
            "keep a low profile in the eyes of the civilized world which is only "
            "interested in hunting them down and bringing them to justice. "
            "Rogues are made up of many different types of scum.  Some are murderers, "
            "while others are thieves and bandits.  But they all love it. "
            "Keep in mind that once you choose to become a rogue, you will be "
            "hated by people from all walks of life.  You will never be able to "
            "move about safely.  Unless you avoid targeting players, that is. "
            "Type <become rogue> to become a rogue.");
    return 1;
}

int become(string str) {
    if(!str) {
        notify_fail("Become what?\n");
        return 0;
    }
    if(str != "rogue") {
        notify_fail("You cannot become that here.\n");
        return 0;
    }
    if(this_player()->query_class() != "explorer") {
        write("You are much too old to start learning our ways now!");
        return 1;
    }
    write("The Grand Master Rogue initiates you into the class of rogues.");
    say(this_player()->query_cap_name()+" becomes a rogue.", this_player());
    this_player()->SetClass("rogue");
    this_player()->setenv("TITLE", "$N the novice rogue");
    this_player()->init_skills("rogue");
    this_player()->setenv("start", "/domains/Praxis/rogue_hall");
    return 1;
}

int go_down() {
    if(this_player()->query_class() != "rogue") {
        write("You cannot penetrate the force field that blocks the passage.");
        say(this_player()->query_cap_name()+" tries to get into the heart of the fortress, but fails.", this_player());
        return 0;
    }
    return 1;
}
