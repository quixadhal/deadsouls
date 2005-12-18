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
    SetRules("OBJ", "here");
    SetErrorMessage("reload what?");
    SetHelp("Syntax: <reload OBJ>\n\n"
      "If you have write permissions to the file of the object "
      "specified, this command adds an init(){} function. Lacking "
      "this function makes many objects break or behave unpredictably."
      "\nSee also: modify, create, delete, copy");
}

mixed can_reload_obj(string str) { 
    if(!creatorp(this_player())) return "This command is only available to builders and creators.";
    else return 1;
}

mixed can_reload_word(string str) { return can_reload_obj("foo"); }

mixed do_reload_obj(object ob) {
    reload(ob);
}

mixed do_reload_word(string wrd) {
    object ob = environment(this_player());
    return do_reload_obj(ob);
}
