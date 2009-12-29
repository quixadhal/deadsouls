//      /bin/dev/_invis.c
//      from the Dead Souls mudlib
//      created by Descartes of Borg october 1992

#include <lib.h>

inherit LIB_DAEMON;

varargs int cmd(){
    if(this_player()->GetInvis()) {
        notify_fail("You are already invisible.\n");
        return 0;
    }
    this_player()->SetInvis(1);
    message ("my_action", "You fade into the shadows.", this_player());
    return 1;
}

string GetHelp(){
    return ("Syntax: invis\n\nMakes you invisible to players of lower levels in all circumstances.\nSee also: vis");
}
