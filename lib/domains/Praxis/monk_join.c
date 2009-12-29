#include <lib.h>
int go_east();

inherit LIB_ROOM;

void init() {
    ::init();
    add_action("become", "become");
    add_action("preview","preview");
}

void create() {
    ::create();
    SetProperty("light", 2);
    SetProperty("indoors", 1);
    SetProperty("no castle", 1);
    SetShort( "Entry to the Hall of Monks");
    SetLong(
            "Candles illuminate the chambers of the monastery.  "
            "The monks who run the monastery keep home here "
            "and initiate new believers into way of monks.  "
            "A passage to the east is filled with a shimmering light.  "
            "<preview> will tell you about becoming a monk.");
    SetItems(
            (["candles" : "They are all over the monastery.",
             "candle" : "It burns brightly and helps light the room.",
             "monk" : "She is wandering about peacefully.",
             "passage" : "You sense that only monks may pass that way."]) );
    SetExits( 
            (["down" : "/domains/Praxis/stairs"]) );
    AddExit("east","/domains/Praxis/monk_hall",(:go_east:));
}

int preview() {
    if(this_player()->query_class() != "explorer") {
        message("info", "This is not for you.", this_player());
        return 1;
    }
    say(this_player()->query_cap_name()+" seeks to learn about the monks.", 
            this_player());
    message("info", "Welcome, explorer!", this_player());
    message("info", "Monks make up people of many different beliefs of "
            "great faith, but they are all united in their belief in a single "
            "deity of Goodness and will devote themselves to Holy War to fight "
            "evil wherever it might be.  They are nearly fanatical about their "
            "beliefs.  To become a monk, type <become monk>,", this_player());
    return 1;
}

int become(string str) {
    if(!str) {
        notify_fail("Become what?\n");
        return 0;
    }
    if(str != "monk") {
        notify_fail("You cannot become that here.\n");
        return 0;
    }
    if(this_player()->query_class() != "explorer") {
        message("my_action", "You are much too old to start learning our ways now!", this_player());
        return 1;
    }
    message("my_action", "The Grand Lord High Priestess initiates you into the class of monks.", this_player());
    message("other_action", this_player()->query_cap_name()+" becomes a monk.",
            this_object(), ({ this_player() }));
    this_player()->SetClass("monk");
    this_player()->setenv("TITLE", "$N the novice monk");
    this_player()->init_skills("monk");
    this_player()->setenv("start", "/domains/Praxis/monk_hall");
    return 1;
}

int go_east() {
    if(creatorp(this_player())){
        write("Mighty immortals are always welcome into the humble monastary.");
        return 1;
    } else 
        if(this_player()->query_class() != "monk") {
            write("You cannot penetrate the force field that blocks the passage.");
            say(this_player()->query_cap_name()+" tries to get into the monk's sanctuary, but fails.", this_player());
            return 0;
        }
    return 1;
}


