#include <lib.h>

inherit LIB_DAEMON;

mixed cmd(string args) {
    object ob, room, where;
    string last_loc, name;
    int result;

    if( !args || args == "" ){ 
        ob = this_player();
        name = "You";
    }
    else {
        if( !(ob = find_player(convert_name(args))) && !(ob = find_living(args)) )
            return "No such being exists anywhere presently.";
        name = "They";
    }
    if(!last_loc = ob->GetProperty("LastLocation")){
        write(name+" have nowhere to return to.");
        return 1;
    }

    if(grepp(last_loc,"#")) where = find_object(last_loc);
    else where = load_object(last_loc);

    if(!where){
        write("There is a problem with that location.");
        write(name+" remain where "+lower_case(name)+" are.");
        return 1;
    }

    if(environment(ob) == where){
        write(name+" are already there.");
        return 1;
    }

    else result = ob->eventMoveLiving(where);

    if(ob != this_player()){
        if(result){
            message("system", "You have been returned to your previous location by " +
                    (string)this_player()->GetName() + ".", ob);
            message("system", "You return " + (string)ob->GetCapName() +
                    " to their previous location.", this_player());
        }
        else {
            return "Failed to move " + (string)ob->GetCapName() + ".";
        }
    }
    return 1;
}

void help() {
    message("help", "Syntax: <return [living]>\n\n"
            "Sends someone back, after a trans.\n"
            "With no arguments, sends you to your last location.\n\n"
            "See also: goto", this_player());
}
