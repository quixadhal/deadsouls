//	/bin/dev/_reset.c
//	Reset command
//	created by Descartes of Borg 24 Nov 1992

#include <lib.h>

inherit LIB_DAEMON;

int cmd(string str) {
    object ob;

    if(!str) {
	environment(this_player())->reset();
	write("You reset the room.\n");
	return 1;
    }
    ob = present(str, environment(this_player()));
    if(!ob) ob = present(str, this_player());
    if(!ob) ob = parse_objects(environment(this_player()), str);
    if(!ob) ob = parse_objects(this_player(), str);
    if(!ob) call_other(str, "reset");
    else ob->reset();
    write("You reset "+str+".\n");
    return 1;
}

void help() {
    write("Syntax: <reset [(object)]> or <reset [(file)]> or <reset>\n\n"+
	"Resets the object named or the environment you are in if no object\n"+
	"is named.\n");
}
