#include <lib.h>
#include <rooms.h>

mixed cmd(){
    object who = this_player();
    object env = environment(this_player());
    object room = load_object(ROOM_ARCH);
    int ret, err;

    if(env == room){
        write("You're already there.");
        return 1;
    }

    if(!archp(who)){
        write("\nYou are naughty, and must be punished.\n");
        who->eventDestruct();
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

string GetHelp(){
    return "Syntax: arch\n\n"
        "Transports you to the arch room. \nSee also: wiz\n";
}
