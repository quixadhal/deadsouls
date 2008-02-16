#include <lib.h>
void help();

inherit LIB_DAEMON;

int cmd(string str) {
    string ret, *ret_arr;
    switch(str){
    case "weap" : str = "weapon";break;
    case "weapons" : str = "weapon";break;
    case "ob" : str = "obj";break;
    case "obs" : str = "obj";break;
    case "objs" : str = "obj";break;
    case "objects" : str = "obj";break;
    case "meals" : str = "meal";break;
    case "npcs" : str = "npc";break;
    case "mob" : str = "npc";break;
    case "mobs" : str = "npc";break;
    case "mon" : str = "npc";break;
    case "monster" : str = "npc";break;
    case "monsters" : str = "npc";break;
    case "rooms" : str = "room";break;
    case "arm" : str = "armor";break;
    case "armors" : str = "armor";break;
    }

    if(!sizeof(str)){
        help();
        return 1;
    }

    write("Category selected: "+str);
    str = homedir(this_player()) + "/area/" + str;

    if(!directory_exists(homedir(this_player()) + "/area")){
        write(homedir(this_player()) + "/area/ does not exist!");
        return 1;
    }

    if(!directory_exists(str)){
        write(str + " does not exist!");
        return 1;
    }

    ret_arr = sort_array(get_dir(str + "/"),1);
    ret = "";

    tc("str: "+str);
    tc("ret_arr: "+identify(ret_arr));

    foreach(string element in ret_arr){
        if(last(element,2) == ".c"){
            element = truncate(element,2);
            ret += element + "\n";
        }
    }

    if(!sizeof(ret)){
        write("No files.");
        return 1;
    }

    this_player()->eventPage( ({ ret }) );
    return 1;
}


void help() {
    message("help", "Syntax: arealist [ npc | room | weapon | armor | object ]\n\n"
      "", 
      this_player());
}
