#pragma save_binary

#include <lib.h>
#include <daemons.h>
#include <rooms.h>
#include <modules.h>

inherit LIB_VERB;

static void create() {
    verb::create();
    SetVerb("modify");
    SetRules("OBJ STR", "here STR", "room STR");
    SetErrorMessage("Modify what how?");
    SetHelp("Syntax: <modify OBJ SETTING VALUE>\n\n"
      "If you have write permissions to the file of the object "
      "specified, this command initiates changes to that file "
      "based on the arguments you supply. For example, to change "
      "the short description of the room you are in:\n"
      "modify here short a nice new room\n"
      "To modify an orc npc's desciption (if he's in your current room):\n "
      "modify orc long a polite, well-groomed orc.\n\n"
      "Available settings are: \n----\n"
      "room: "+MODULES_CREATE->GetSettings("room")+"\n--\n"
      "npc: "+MODULES_CREATE->GetSettings("npc")+"\n--\n"
      "weapon: "+MODULES_CREATE->GetSettings("weapon")+"\n--\n"
      "item: "+MODULES_CREATE->GetSettings("item")+"\n--\n"
      "armor: "+MODULES_CREATE->GetSettings("armor")+"\n--\n"
      "chair: "+MODULES_CREATE->GetSettings("chair")+"\n--\n"
      "bed: "+MODULES_CREATE->GetSettings("bed")+"\n--\n"
      "table: "+MODULES_CREATE->GetSettings("table")+"\n--\n"
      "container: "+MODULES_CREATE->GetSettings("storage")+"\n----\n"
      "meal: "+MODULES_CREATE->GetSettings("meal")+"\n----\n"
      "drink: "+MODULES_CREATE->GetSettings("meal")+"\n----\n"
      "door: "+MODULES_CREATE->GetSettings("door")+"\n----\n"
      "book: "+MODULES_CREATE->GetSettings("book")+"\n----\n"
      "\nSee also: copy, create, delete, reload, initfix, add");
}

mixed can_modify_obj_str(string str) { 
    if(!creatorp(this_player())) return "This command is only available to builders and creators.";
    else return 1;
}

mixed can_modify_word_str(string str) { return can_modify_obj_str("foo"); }

mixed do_modify_obj_str(object ob, string str) {
    object staff;
    staff = present("tanstaafl",this_player());
    if(!staff) {
	write("You must be holding the creator staff in order to use this command.");
	write("If you don't know where you put it, get another one from the chest ");
	write("in your workroom.");
	return 1;
    }

    if(ob->GetNoModify() && !grepp(lower_case(str),"modify")) {
	write("This needs to be edited by hand.");
	return 1;
    }

    if(base_name(ob) == LIB_DUMMY &&
      !ob->GetDoor()) ob = environment(this_player());
    else if(base_name(ob) == LIB_DUMMY) ob = load_object(ob->GetDoor());

    if(starts_with(base_name(ob),"/lib/")) {
	write("This appears to be a library object. Canceling modification.");
	return 1;
    }

    return staff->eventModify(ob, str);
}

mixed do_modify_word_str(string wrd, string str) {
    object staff, ob;
    staff = present("tanstaafl",this_player());
    if(!staff) {
	write("You must be holding the creator staff in order to use this command.");
	write("If you don't know where you put it, get another one from the chest ");
	write("in your workroom.");
	return 1;
    }

    ob = environment(this_player());

    if(starts_with(base_name(ob),"/lib/")) {
	write("This appears to be a library object. Canceling modification.");
	return 1;
    }

    return do_modify_obj_str(ob, str);
}
