//	/bin/dev/_reset.c
//	Reset command
//	created by Descartes of Borg 24 Nov 1992

#include <lib.h>

inherit LIB_DAEMON;

int cmd(string str) {
    object ob;

    if(!str || str == "here") {
        environment(this_player())->reset();
        write("You reset the room.\n");
        return 1;
    }
    ob = present(str, environment(this_player()));
    if(!ob) ob = present(str, this_player());
    if(!ob) ob = parse_objects(environment(this_player()), str);
    if(!ob) ob = parse_objects(this_player(), str);
    if(!ob) {
        write("That object can't be found.");
        return 1;
    }
    else ob->reset();
    write("You reset "+str+".\n");
    return 1;
}

string GetHelp() {
    return ("Syntax: reset [object | file]\n\n"+
            "Resets the object named or the environment you are in if no object\n"+
            "is named.");
}
