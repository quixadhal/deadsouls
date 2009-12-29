#include <lib.h>

inherit LIB_DAEMON;

mixed cmd(string str){
    object room;
    string path;
    if(!sizeof(str) || str == "here"){
        room = room_environment(this_player());
        path = base_name(room);
    }
    else {
        str = absolute_path(this_player()->query_cwd(), str);
        if(!str) return "File not found.";
        room = find_object(str);
        if(room) path = base_name(room);
    }
    if(!path){
        write("Unable to set "+str+" as your home room.");
        return 1;
    } 
    this_player()->SetParanoia("homeroom", path);
    write("Your current home room is: "+path);
    return 1;
}

string GetHelp() {
    return ("Syntax: homeroom [path]\n\n"
            "Sets the specified file to be your \"home\" "
            "room, i.e. the room you teleport to "
            "when you type \"home\" with no arguments. If you do "
            "not specify a path, your current room environment is "
            "set to be your home."
            "\n"
            "See also: env, homedir");
}
