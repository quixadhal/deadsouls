#include <lib.h>
#include <rooms.h>

mixed cmd(){
    object who = this_player();
    object room = load_object(ROOM_ARCH);
    int ret, err;

    if(!archp(who)){
        write("You are naughty, and must be punished.");
        return 1;
    }

    if(!room){
        write("There seems to be a problem with the arch room.");
        return 1;
    }

    err = catch( ret = who->eventMoveLiving(room) );

    if(err || !ret){
        write("Looks like some kind of problem getting into the arch room.");
    }

    return 1;
}

void help(){
    message("help", "Syntax: arch\n\n"
        "Transports you to the arch room. \nSee also: wiz\n", this_player());
}


