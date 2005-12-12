#include <lib.h>

inherit LIB_DAEMON;

mixed cmd(string args) {
    object ob, room;
    string last_loc;

    if( !args || args == "" ) {
	if(!last_loc = this_player()->GetProperty("LastLocation")){
	    write("You have nowhere to return to.");
	    return 1;
	}
	else if(!load_object(last_loc)){
	    write("There is a problem with that location.");
	    write("You remain where you are.");
	    return 1;
	}
	else this_player()->eventMoveLiving(load_object(last_loc));
	return 1;
    }

    if( !(ob = find_player(convert_name(args))) && !(ob = find_living(args)) )
	return "No such being exists anywhere presently.";
    if( !room = load_object(ob->GetProperty("ReturnSite"))){
	write("That person has nowhere to return.");
	return 1;
    }

    if(room == environment(ob)){
	write("That person is already there.");
	return 1;
    }

    ob->SetProperty("ReturnSite","");
    if( !((int)ob->eventMoveLiving(room) ))
	return "Failed to move " + (string)ob->GetCapName() + ".";
    message("system", "You have been returned to your previous location by " +
      (string)this_player()->GetName() + ".", ob);
    message("system", "You return " + (string)ob->GetCapName() + 
      " to their previous location.", this_player());
    return 1;
}

void help() {
    message("help", "Syntax: <return [living]>\n\n"
      "Sends someone back, after a trans.\n"
      "With no arguments, sends you to your last location.\n\n"
      "See also: goto", this_player());
}
