#include <lib.h>

inherit LIB_VERB;

static void create() {
    verb::create();
    SetVerb("sleep");
    SetRules("");
    SetErrorMessage("Sleep?");
    SetSynonyms("go to sleep");
    SetHelp("Syntax: sleep\n"
            "If lying down, you fall asleep. If not, you collapse "
            "asleep. Being attacked will usually wake you up, "
            "otherwise you will awaken after a few minutes, stronger "
            "and more refreshed.");
}

mixed can_sleep() {
    if( !creatorp(this_player()) && this_player()->GetCaffeine() > 10 ) {
        return "You are too wired to sleep right now.";
    }
    if(this_player()->GetRace()=="elf") return "Elves don't sleep.";

    return 1;
}

mixed do_sleep() {
    tell_player(this_player(),"You fall asleep.");
    tell_room(environment(this_player()), this_player()->GetName()+ 
            " closes "+possessive(this_player())+" eyes and appears to "+
            "lose consciousness.", ({this_player()}) );
    return this_player()->SetSleeping(random(10)+5);
}
