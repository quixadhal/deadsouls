#include <lib.h>

inherit LIB_DAEMON;

varargs int cmd(string str){
    object tmp, player, mount, room;
    string name;
    if(!str){
        write("Try: help pwhere");
        return 1;
    }
    player = find_player(str);
    if(!player){
        write("That person is not logged in.");
        return 1;
    }
    else name = player->GetShort();
    tmp = environment(player);
    if(!tmp){
        write(name+" has no environment.");
        return 1;
    }
    if(living(tmp) && environment(tmp)){
        mount = tmp;
        room =  environment(tmp);
    }
    else room = tmp;
    write(name+" is"+(mount ? " mounted on "+mount->GetPlainShort() : "")+
            " in "+room->GetShort()+" ( "+base_name(room)+" )");
    return 1;
}

string GetHelp(){
    return "Syntax: pwhere <player>\n\n"
        "Tells you the location of the person specified.\n"
        "See also: people, who";
}
