#pragma save_binary

#include <lib.h>
#include <daemons.h>
#include <rooms.h>
#include <modules.h>
#include <commands.h>

inherit LIB_VERB;

mixed do_add_obj_to_obj(object ob, object ob2);

static void create() {
    verb::create();
    SetVerb("add");
    SetRules("OBJ to OBJ", "OBJ to here");
    SetErrorMessage("Add what?");
    SetHelp("Syntax: <add [OBJ] to [OBJ]>\n"
      "        <add [OBJ] to here>\n"
      "This command adds the first object to the permanent "
      "inventory of the second object, if you have access "
      "privileges to both files. You will be prompted for input. "
      "If the taget is an NPC, you can input a command for the "
      "NPC to execute when it is created, such as: wear shirt\n"
      "Otherwise, enter the number of these items you want when "
      "prompted.\n"
      "\nSee also: copy, create, delete, modify");
}



mixed can_add_obj_to_obj(string one, string two){
    if(!creatorp(this_player())) return "This command is for creators only.";
    return 1; 
}
mixed can_add_obj_to_here(string one) { return 1; }


mixed do_add_obj_to_here(object ob){
    return do_add_obj_to_obj(ob, environment(this_player()));
}

mixed do_add_obj_to_obj(object ob, object ob2) {
    string str = base_name(ob2)+".c";
    string sourcefile = base_name(ob)+".c";

    if(!inherits(LIB_NPC,ob2) &&
      !inherits(LIB_STORAGE,ob2) &&
      !inherits(LIB_ROOM,ob2)){
	write("That object is not intended to contain other objects. Addition halted.");
	return 0;
    }

    if(!check_privs(this_player(),str) || 
      !check_privs(this_player(),sourcefile)){
	write("You lack sufficient privileges for this operation. Copy failed.");
	return 0;
    }
    if(!file_exists(sourcefile) || !file_exists(str)) {
	write("That file no longer exists.");
	return 0;
    }

    if(base_name(ob2) == LIB_DUMMY) ob = environment(this_player());
    if(base_name(ob) == LIB_DUMMY) {
	write("That's not the kind of thing you can add to something.");
	return 1;
    }
    load_object(MODULES_GENERIC)->eventAddItem(ob2, base_name(ob));
    return 1;
}
