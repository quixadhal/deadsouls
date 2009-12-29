#include <lib.h>
#include <rooms.h>

mixed cmd(){
    object who = this_player();
    object env = environment(this_player());
    object room = load_object(ROOM_WIZ);
    int ret, err;

    if(!room){
        write("There seems to be a problem with the wiz room.");
        return 1;
    }

    if(env && env == room){
        write("Wiz!");
        return 1;
    }

    err = catch( ret = who->eventMoveLiving(room) );

    if(err || !ret){
        write("Looks like some kind of problem getting into the wiz room.");
    }

    return 1;
}

string GetHelp(){
    return ("Syntax: wiz\n\n"
            "Transports you to the wiz room.");
}
