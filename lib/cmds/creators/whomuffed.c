#include <lib.h>

inherit LIB_DAEMON;

mixed cmd(string str) {
    object player;
    if(!str || str =="") {
        if(!sizeof(this_player()->GetMuffed())) write("You are ignoring no one.");
        else {
            write("You are ignoring the following:\n");
            write(implode(filter(this_player()->GetMuffed(), (: capitalize($1) :)), "\n")+"\n");
        }
        return 1;
    }

    else str = lower_case(str);
    if(!(player = find_player(str))){
        write("No such player to be found.");
        return 1;
    }

    if(!sizeof(player->GetMuffed())){
        write("That player is ignoring no one.");
        return 1;
    }

    else {
        write(capitalize(str)+" is ignoring: \n");
        write(implode(filter(player->GetMuffed(), (: capitalize($1) :)), "\n")+"\n");
    }
    return 1;
}

string GetHelp() {
    return ("Syntax: whomuffed [name]\n\n"
            "This command lists who is ignored by the player specified.\n"
            "See also: earmuff, unmuff");
}

