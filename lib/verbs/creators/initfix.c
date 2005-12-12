#pragma save_binary

#include <lib.h>
#include <daemons.h>
#include <modules.h>

inherit LIB_VERB;

static void create() {
    verb::create();
    SetVerb("initfix");
    SetRules("OBJ", "here");
    SetErrorMessage("initfix what?");
    SetHelp("Syntax: <initfix OBJ SETTING VALUE>\n\n"
      "If you have write permissions to the file of the object "
      "specified, this command adds an init(){} function. Lacking "
      "this function makes many objects break or behave unpredictably."
      "\nSee also: modify, create, delete, copy");
}

mixed can_initfix_obj(string str) { 
    if(!creatorp(this_player())) return "This command is only available to builders and creators.";
    else return 1;
}

mixed can_initfix_word(string str) { return can_initfix_obj("foo"); }

mixed do_initfix_obj(object ob) {
    if(load_object(MODULES_FILE)->eventAddInit(base_name(ob)+".c") == 2) {
	return "File already has a working init function.";
    }
    else return "Done.";
}

mixed do_initfix_word(string wrd) {
    object ob = environment(this_player());
    return do_initfix_obj(ob);
}
