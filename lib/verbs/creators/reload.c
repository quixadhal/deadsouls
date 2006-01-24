#pragma save_binary

#include <lib.h>
#include <daemons.h>
#include <modules.h>
#include <rooms.h>
#include <commands.h>

inherit LIB_VERB;

static void create() {
    verb::create();
    SetVerb("reload");
    SetRules("OBJ", "STR OBJ", "STR here", "here");
    SetErrorMessage("reload what?");
    SetHelp("Syntax: <reload OBJ>\n\n"
      "This command loads into memory the file of the object "
      "you specify, and replaces the current copy with a new "
      "copy. If you change something about a sword you are "
      "holding, for example, \"reload sword\" will update the "
      "changes and you will be holding a sword with the updates.\n"
      "  When used with the -r flag it recursively loads all the objects "
      "inherited by the target object. If any of those objects "
      "or the target object's file fail to load, the object "
      "is not updated.\n"
      "\nSee also: copy, create, delete, modify, initfix, add");
}

mixed can_reload_obj(string str) { 
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
