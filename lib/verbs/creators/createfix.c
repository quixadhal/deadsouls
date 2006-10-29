

#include <lib.h>
#include <daemons.h>
#include <modules.h>

inherit LIB_VERB;

static void create() {
    verb::create();
    SetVerb("createfix");
    SetRules("OBJ", "here");
    SetErrorMessage("createfix what?");
    SetHelp("Syntax: <createfix OBJ>\n\n"
      "If you have write permissions to the file of the object "
      "specified, this command adds a ::create call. Lacking "
      "this call makes many objects break or behave unpredictably."
      "\nSee also: initfix, copy, create, delete, modify, reload, add");
}

mixed can_createfix_obj(string str) { 
    if(!creatorp(this_player())) 
	return "This command is only available to builders and creators.";
    else return 1;
}

mixed can_createfix_word(string str) { return can_createfix_obj("foo"); }

mixed do_createfix_obj(object ob) {
    object staff;
    staff = present("tanstaafl",this_player());
    if(!staff) {
	write("You must be holding the creator staff in order to use this command.");
	write("If you don't know where you put it, get another one from the chest ");
	write("in your workroom.");
	return 1;
    }

    if(ob->GetDoor()) ob = load_object(ob->GetDoor());

    if(first(base_name(ob),5) == "/lib/") {
	write("This appears to be a lib file. Aborting modification.");
	return 1;
    }

    if(interactive(ob)) {
	write("Players are not createfixable.");
	return 1;
    }

    if(staff->eventAddCreate(base_name(ob)+".c") == 2) {
	write("File already has a working create function.");
    }

    else write("Done.");
    reload(ob);
    return 1;
}

mixed do_createfix_word(string wrd) {
    object ob = environment(this_player());
    return do_createfix_obj(ob);
}
