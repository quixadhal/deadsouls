#pragma save_binary

#include <lib.h>
#include <daemons.h>
#include <rooms.h>

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
      "room: "+CREATE_D->GetSettings("room")+"\n--\n"
      "npc: "+CREATE_D->GetSettings("npc")+"\n--\n"
      "weapon: "+CREATE_D->GetSettings("weapon")+"\n--\n"
      "item: "+CREATE_D->GetSettings("item")+"\n--\n"
      "armor: "+CREATE_D->GetSettings("armor")+"\n--\n"
      //"chair: "+CREATE_D->GetSettings("chair")+"\n--\n"
      //"bed: "+CREATE_D->GetSettings("bed")+"\n--\n"
      //"table: "+CREATE_D->GetSettings("table")+"\n--\n"
      //"container: "+CREATE_D->GetSettings("storage")+"\n----\n"
      "\nSee also: create, delete, copy");
}

mixed can_modify_obj_str(string str) { 
    if(!creatorp(this_player())) return "This command is only available to builders and creators.";
    //    if(last_string_element(base_name(environment(this_player())),"/") == "start")
    //	return "The start room should probably be edited by hand. Change cancelled.";
    //write(base_name(environment(this_player())));
    //write(ROOM_START);
    if(base_name(environment(this_player())) == ROOM_START)
	return "The start room should probably be edited by hand. Change cancelled.";
    else return 1;
}

mixed can_modify_word_str(string str) { return can_modify_obj_str("foo"); }

mixed do_modify_obj_str(object ob, string str) {
    if(base_name(ob) == LIB_DUMMY) ob = environment(this_player());
    return CREATE_D->eventModify(ob, str);
}

mixed do_modify_word_str(string wrd, string str) {
    object ob = environment(this_player());
    return CREATE_D->eventModify(ob, str);
}
