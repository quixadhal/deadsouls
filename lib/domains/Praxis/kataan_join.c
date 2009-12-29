#include <lib.h>
int go_down();

inherit LIB_ROOM;

void init() {
    ::init();
    add_action("become", "become");
    add_action("preview","preview");
}

void create() {
    ::create();
    SetProperties( (["no castle":1,"light":2,"indoors":1,"no castle":1]));
    SetShort( "cavern of the kataans");
    SetLong(
            "You are in a dark underground cavern just north of Praxis. "
            "Deep, intricate murals and symbols are carved into the walls of the "
            "cave.  Down deeper in the cavern is a passage protected "
            "by a shimmering %^BLUE%^blue%^RESET%^ light.  "
            "<preview> will tell you about becoming a Kataan. ");
    SetItems(
            (["mural" : "It is very old and depicts gruesome cult practices.",
             "murals" : "They are quite ancient and are made up of "
             "pictures depicting bizarre rituals.",
             "light" : "You are certain that it is a field which allows "
             "only kataans to pass.",
             "cavern" : "The cavern almost pulses with hidden evil."]) );
    SetExits( 
            (["up" : "/domains/Praxis/forest3"]) );
    AddExit("down","/domains/Praxis/kataan_hall",(:go_down:));
}

int preview() {
    if(this_player()->query_class() != "explorer") {
        write("This is not for you.");
        return 1;
    }
    say(this_player()->query_cap_name()+" seeks to learn about "
            "kataans.", this_player());
    write("Welcome, explorer! ");
    write("Kataans are an ancient coven of evil, magical warriors skilled "
            "in the use of projectiles and stealth.  They are careful to do "
            "no acts which might be considered good.  Instead, the use the "
            "powers given to them by their demon masters to terrorize good "
            "beings wherever they may be.\n"
            "To become a kataan, type <become kataan>\n");
    return 1;
}

int become(string str) {
    if(!str) {
        notify_fail("Become what?\n");
        return 0;
    }
    if(str != "kataan") {
        notify_fail("You cannot become that here.\n");
        return 0;
    }
    if(this_player()->query_class() != "explorer") {
        write("You are much too old to start learning our ways now!");
        return 1;
    }
    write("The Master Kataan initiates you into the class of kataans.");
    say(this_player()->query_cap_name()+" becomes a kataan.", this_player());
    this_player()->SetClass("kataan");
    this_player()->setenv("TITLE", "$N the pebble tosser");
    this_player()->init_skills("kataan");
    this_player()->setenv("start", "/domains/Praxis/kataan_hall");
    return 1;
}

int go_down() {
    if(this_player()->query_class() != "kataan") {
        write("You cannot penetrate the force field that blocks the passage.");
        say(this_player()->query_cap_name()+" tries to get into the kataan's sanctuary, but fails.", this_player());
        return 0;
    }
    return 1;
}


