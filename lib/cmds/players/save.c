//	/bin/user/_save.c
//	from the Dead Souls mudlib
//	command to save the player
//	written by Descartes of Borg

#include <lib.h>

inherit LIB_DAEMON;

int cmd(){
    if(this_player()->query_disable()) {
        write("Wait a minute.");
        return 1;
    }
    this_player()->set_disable();
    this_player()->save_player(this_player()->GetKeyName());
    write(this_player()->GetName()+" is now saved.");
    return 1;
}

string GetHelp(){
    return ("Syntax: save\n\nThis command saves the"
            " present status of your character to disk. This is important as"
            " it is this file that will be used to set your character back "
            "up if the mud should crash. Your character is automatically "
            "saved if you quit.");
}
