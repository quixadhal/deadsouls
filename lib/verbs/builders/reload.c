#include <lib.h>
#include <daemons.h>
#include <modules.h>
#include ROOMS_H
#include <commands.h>

inherit LIB_VERB;

string libfile = "foo";
int quiet = 0;

static void create() {
    verb::create();
    SetVerb("reload");
    SetRules("OBJ", "STR OBJ", "STR here", "here", "every STR");
    SetErrorMessage("reload what?");
    SetHelp("Syntax: reload [every] <OBJ>\n\n"
            "This command loads into memory the file of the object "
            "you specify, and replaces the current copy with a new "
            "copy. If you change something about a sword you are "
            "holding, for example, \"reload sword\" will update the "
            "changes and you will be holding a sword with the updates.\n"
            "  When used with the -r flag it recursively loads all the objects "
            "inherited by the target object. If any of those objects "
            "or the target object's file fail to load, the object "
            "is not updated.\n"
            "    If you \"reload every npc\", then any loaded object that "
            "inherits LIB_NPC gets reloaded. Other valid lib objects "
            "that can be used this way are: room, sentient, armor, item.\n"
            "Please note that if there are too many items to reload, "
            "the command will fail with \"Too long evaluation\" errors.\n"
            "    Books, due to their processing-intensive load time, "
            "are excluded from the \"every\" keyword.\n"
            "Please note that reloading a door also reloads the "
            "door's adjoining rooms.\n"
            "\nSee also: copy, create, delete, modify, initfix, add");
}

mixed can_reload_obj(string str) { 
    if(!builderp(this_player())) return "This command is only available to builders and creators.";
    else return 1;
}

mixed can_reload_every_str(string str){
    if(!builderp(this_player())) return "This command is only available to builders and creators.";
    else return 1;
}

mixed can_reload_str_obj(string str){
    return can_reload_obj(str);
}

mixed can_reload_word(string str){
    return can_reload_obj("foo"); 
}

mixed can_reload_str_word(string str, string str2){ 
    return can_reload_obj("foo"); 
}

mixed do_reload_obj(object ob) {
    string s1,s2, foo = "Null object: ";
    if(ob && ob->GetDoor()) ob = load_object(ob->GetDoor());
    if(!creatorp(this_player()) && strsrch(base_name(ob), homedir(this_player()))){
        write("Builders can only reload things that belong to them.");
        return 1;
    }
    if(!ob || userp(ob)) {
        if(ob) foo = base_name(ob)+": ";
        if(!quiet){
            write(foo+"Invalid for reloading.");
        }
        return -1;
    }
    if(ob && ob->GetDirectionMap()){
        write(base_name(ob)+" is a virtual room, and not subject to normal reloading.");
        return 1;
    }
    if(!strsrch(base_name(ob),"/open") ||
            sscanf(base_name(ob),"/realms/%s/tmp/%s",s1,s2) == 2){
        write(base_name(ob)+" is a temp file and not subject to reloading.");
        return 1;
    }
    reload(ob, 0, quiet);
    if(ob && inherits(LIB_DOOR,ob)){
        string *doors = environment(this_player())->GetDoors();
        if(!sizeof(doors)) return 1;
        foreach(string dir in doors){
            string substr = environment(this_player())->GetDoor(dir);
            if(last(substr,2) == ".c") substr = truncate(substr,2);
            if(substr == base_name(ob)){
                reload(load_object(environment(this_player())->GetExit(dir)));
                reload(environment(this_player()));
            }
        }
    }
    return 1;
}

mixed do_reload_str_obj(string str, object ob) {
    if(str == "-r") reload(ob, 1, quiet);
    else return reload(ob, 0, quiet);
}

mixed do_reload_word(string wrd) {
    object ob = environment(this_player());
    if(wrd == "here") return do_reload_obj(ob);
    else return "failed.";
}

mixed do_reload_str_word(string wrd1, string wrd2) {
    object ob = environment(this_player());
    if(wrd1 == "-r" && wrd2 = "here") reload(ob, 1);
    else return "Failed.";
}

mixed do_reload_every_str(string str){
    object *ob_pool = ({});
    int count;
    if(!archp(this_player())){
        write("This verb is intended for arches only.");
        return 1;
    }

    switch(str){
        case "npc" : libfile = LIB_NPC; break;
        case "sentient" : libfile = LIB_SENTIENT; break;
        case "room" : libfile = LIB_ROOM; break;
        case "weapon" : libfile = LIB_WEAPON; break;
        case "item" : libfile = LIB_ITEM; break;
        case "container" : libfile = LIB_STORAGE; break;
        case "armor" : libfile = LIB_ARMOR; break;
        case "worn_storage" : libfile = LIB_WORN_STORAGE; break;
        default : libfile = str;
    }

    if(!file_exists(libfile) && !file_exists(libfile+".c")){
        write("There is no such library file.");
        return 1;
    }

    if(last(libfile,2) == ".c") libfile = truncate(libfile,2);
    load_object("/secure/cmds/creators/update")->cmd("-a -r "+libfile);

    ob_pool = filter(objects(), (: ( inherits(libfile, $1) ) :) );
    if(!sizeof(ob_pool)) 
        ob_pool = filter(objects(), (: ( base_name($1) == libfile ) :) );

    if(!sizeof(ob_pool)) {
        write("None found.");
        return 1;
    }

    quiet = 1;
    call_out("unQuiet", 6);
    write("Reloading...");
    foreach(object ob in ob_pool){
        if(do_reload_obj(ob) > 0) count++;
    }
    quiet = 0;

    write("Done. Reloaded "+count+" objects.");
    libfile = "foo";
    return 1;
}

void unQuiet(){
    quiet = 0;
}

