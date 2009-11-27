#include <lib.h>

string GetHelp();

inherit LIB_DAEMON;

int cmd(string str) {
    object ob, env;
    string which, what, nom, ret;

    if(sscanf(str,"%s %s",which, what) != 2){
        write(GetHelp());
        return 1;
    }

    str = which;

    switch(str){
        case "weap" : str = "weap";break;
        case "weapons" : str = "weap";break;
        case "weapon" : str = "weap";break;
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

    env = environment(this_player());

    if(!env || strsrch(base_name(env), homedir(this_player()))){
        if(creatorp(this_player())){
            write("You are a creator. Use the clone command.");
        }
        else {
            write("You can only do this while in your area.");
        }
        return 1;
    }

    if(str == "room"){
        write("Rooms can't be cloned. Try: areagoto");
        return 1;
    }

    str = homedir(this_player()) + "/area/" + str + "/" + what;
    if(last(str,2) != ".c") str += ".c";
    write("File selected: "+str);

    if(!file_exists(str)){
        write(str + " does not exist!");
        return 1;
    }

    if( ret = catch(ob = new(str)) ){
        write("Error in cloning object: " + ret);
        return 1;
    }

    if(!ob){
        write("Failed to clone file: " + str);
        return 1;
    }

    if( !(ob->eventMove(this_player())) &&
            !(ob->eventMove(environment(this_player()))) ) {
        write("Failed to properly move the object.");
        return 1;
    }

    nom = ob->GetShort();

    if( !(ret = this_player()->GetMessage("clone", ob)) )
        ret = this_player()->GetName() + " clones " + nom + ".";

    say(ret);
    write("You clone " + nom + " ( " + str + " ).");

    return 1;
}

string GetHelp(){
    return ("Syntax: areaclone [ npc | weapon | armor | obj ] <name>\n\n"
            "Allows a builder to bring into existence a copy of one of "
            "her creations. To see the available items, use the arealist "
            "command.\n"
            "See also: arealist, areagoto");
}
