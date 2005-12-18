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
    SetHelp("Syntax: <initfix OBJ>\n\n"
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
    object staff;
    staff = present("tanstaafl",this_player());
    if(!staff) {
	write("You must be holding the creator staff in order to use this command.");
	write("If you don't know where you put it, get another one from the chest ");
	write("in your workroom.");
	return 1;
    }

    if(staff->eventAddInit(base_name(ob)+".c") == 2) {
	return "File already has a working init function.";
    }
    else write("Done.");
    return 1;
}

mixed do_initfix_word(string wrd) {
    object ob = environment(this_player());
    return do_initfix_obj(ob);
}
