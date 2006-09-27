#pragma save_binary

#include <lib.h>
#include <daemons.h>
#include <modules.h>
#include <rooms.h>
#include <commands.h>

inherit LIB_VERB;

string libfile = "foo";

static void create() {
    verb::create();
    SetVerb("reload");
    SetRules("OBJ", "STR OBJ", "STR here", "here", "every STR");
    SetErrorMessage("reload what?");
    SetHelp("Syntax: <reload OBJ>, <reload every OBJ>\n\n"
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
      "\nSee also: copy, create, delete, modify, initfix, add");
}

mixed can_reload_obj(string str) { 
    if(!creatorp(this_player())) return "This command is only available to builders and creators.";
    else return 1;
}

mixed can_reload_every_str(string str){
    if(!creatorp(this_player())) return "This command is only available to builders and creators.";
    else return 1;
}

mixed can_reload_str_obj(string str) {
    return can_reload_obj(str);
}

mixed can_reload_word(string str) { 
    return can_reload_obj("foo"); }

mixed can_reload_str_word(string str, string str2) { 
    return can_reload_obj("foo"); }

mixed do_reload_obj(object ob) {
    string s1,s2;
    if(userp(ob)) {
	write("No.");
	return 1;
    }
    if(ob->GetDirectionMap()){
	write(base_name(ob)+" is a virtual room, and not subject to normal reloading.");
	return 1;
    }
    if(!strsrch(base_name(ob),"/open") ||
      sscanf(base_name(ob),"/realms/%s/tmp/%s",s1,s2) == 2){
	write(base_name(ob)+" is a temp file and not subject to reloading.");
	return 1;
    }
    return reload(ob);
}

mixed do_reload_str_obj(string str, object ob) {
    if(str == "-r") reload(ob, 1);
    else return reload(ob);
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

    foreach(object ob in ob_pool){
	if(ob) write("reloading: "+file_name(ob));
	do_reload_obj(ob);
    }

    write("Done.");
    libfile = "foo";
    return 1;
}


