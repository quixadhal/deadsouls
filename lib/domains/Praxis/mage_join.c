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
    SetProperty("light", 2);
    SetProperty("indoors", 1);
    SetShort( "Inside a tree");
    SetLong(
            "The inside of the tree impossibly occupies more space than "
            "its physical size!  Torches circle the wooden walls of the "
            "tree, giving it a magical light.  A flight of stairs leads "
            "down into a hole in the ground protected by a magical "
            "%^BLUE%^blue%^RESET%^ "
            "light.  The High Mage Warlock waits here to initiate aspiring "
            "magic users into the mysterious class of Nightmare mages.  "
            "<preview> will tell you about becoming a mage.");
    SetItems(
            (["tree" : "Like Dr. Who's tardis, bigger inside than outside.",
             "hole" : "The hole leads down into an undergorund room.",
             "light" : "You are certain that it is a field which allows "
             "only mages to pass.",
             "walls" : "They are made from the wood of this old tree. An "
             "opening leads outside.",
             "stairs" : "They are very nice.",
             "torch" : "It is locked magically into the wall.",
             "warlock" : "He awaits to initiate new mages.",
             "mage" : "He awaits to initiate new mages and explain "
             "the difference between transitive and intransitive verbs.",
             "opening" : "It leads out to West Road.",
             "torches" : "They create an eerie glow to the room."]) );
    SetExits( 
            (["out" :	"/domains/Praxis/west_road3"]) );
    AddExit("down", "/domains/Praxis/mage_hall.c", (:go_down:));
}

int preview() {
    if(this_player()->query_class() != "explorer") {
        message("info", "This is not for you.", this_player());
        return 1;
    }
    message("other_action", this_player()->query_cap_name()+" seeks to learn about mages.", this_object(), ({this_player()}));
    message("info", "Welcome, explorer!", this_player());
    message("info", "Mages have no overriding beliefs which unite them, but instead "
            "they are united in their love of the art of magic.  Some of them "
            "are devoted practitioners of white magic, while others are evil "
            "dark mages.  Their magical arts are centered around those "
            "which will most help them along in the material world, the arts "
            "of conjuring and magical combat.  You can type <become mage> to "
            "be initiated into the class of mages.", this_player());
    return 1;
}

int become(string str) {
    if(!str) {
        notify_fail("Become what?\n");
        return 0;
    }
    if(str != "mage") {
        notify_fail("You cannot become that here.\n");
        return 0;
    }
    if(this_player()->query_class() != "explorer") {
        message("my_action", "You are much too old to start learning our ways now!", this_player());
        return 1;
    }
    message("my_action", "The High Mage Warlock initiates you into the class of mages.", this_player());
    message("other_action", this_player()->query_cap_name()+" becomes a mage.", this_object(), ({this_player()}));
    this_player()->SetClass("mage");
    this_player()->setenv("TITLE", "$N the novice mage");
    this_player()->init_skills("mage");
    this_player()->setenv("start", "/domains/Praxis/mage_hall");
    return 1;
}

int go_down() {
    if(this_player()->query_class() != "mage") {
        message("my_action", "You cannot penetrate the force field that blocks the passage.", this_player());
        message("other_action", this_player()->query_cap_name()+" tries to get into the mage's sanctuary, but fails.", this_object(), ({this_player() }));
        return 0;
    }
    return 1;
}

