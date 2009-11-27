#include <lib.h>

string GetHelp();

inherit LIB_DAEMON;

int cmd(string str) {
    string ret, *ret_arr;
    switch(str){
        case "weap" : str = "weap";break;
        case "weapons" : str = "weap";break;
        case "weapon" : str = "weap";break;
        case "ob" : str = "obj";break;
        case "obs" : str = "obj";break;
        case "objs" : str = "obj";break;
        case "objects" : str = "obj";break;
        case "object" : str = "obj";break;
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
        write(GetHelp());
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

string GetHelp(){
    return ("Syntax: arealist [ npc | room | weapon | armor | obj ]\n\n"
            "Provides a list of the available files in a given category "
            "that are under the control of the builder issuing the command. "
            "For example, to see the list of rooms belonging to a builder, "
            "she might issue the command: arealist room\n"
            "See also: areaclone, areagoto");
}
