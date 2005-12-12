#include <lib.h>
#include <daemons.h>
#include <rooms.h>
#include <modules.h>
#include <commands.h>

inherit LIB_VERB;

int eventDeleteExit(string str);
int eventDeleteObject(object ob1, object ob2);

static void create() {
    verb::create();
    SetVerb("delete");
    SetRules("exit STR", "OBJ", "OBJ from OBJ");
    SetErrorMessage("Delete what?");
    SetHelp("Syntax: <delete exit DIRECTION>\n"
      "        <delete OBJECT>\n"
      "        <delete OBJECT from OBJECT>\n"
      "See also: copy, create, delete, modify");
}


mixed can_delete_exit_str(string str) {
    return 1;
}

mixed can_delete_obj(object ob){
    return 1;
}

mixed can_delete_obj_from_obj(object ob1, object ob2){
    return 1;
}

mixed do_delete_exit_str(string str) {
    return eventDeleteExit(str);
}

mixed do_delete_obj(object ob){
    return eventDeleteObject(ob, environment(this_player()));
}

mixed do_delete_obj_from_obj(object ob1, object ob2){
    return eventDeleteObject(ob1,ob2);
}

int eventDeleteObject(object ob1, object ob2){
    if(environment(ob1) != ob2) {
	write("That doesn't exist there.");
	return 1;
    }
    load_object(MODULES_GENERIC)->eventDeleteItem(ob1, ob2);
    return 1;
}

int eventDeleteExit(string str){
    string filename;
    string *exits;
    object *players;

    filename = base_name(environment(this_player()))+".c";
    exits = load_object(filename)->GetExits();

    if(member_array(str,exits) == -1) {
	write("That exit does not exist here.");
	return 1;
    }

    if(base_name(environment(this_player())) == ROOM_START){
	write("You should edit the start room by hand. Change cancelled.");
	return 1;
    }

    if(!check_privs(this_player(),filename)){
	write("You can't delete an exit from a room that is not yours.");
	return 1;
    }

    players = get_livings(environment(this_player()),1);
    load_object(MODULES_ROOM)->eventRemoveExit(str, filename);
    load_object("/secure/cmds/creators/update")->cmd("-a "+filename);
    players->eventMove(load_object(filename));

    exits = load_object(filename)->GetExits();

    if(member_array(str,exits) == -1) {
	write("Exit successfully removed.");
	say(this_player()->GetCapName()+" removes an exit.");
	return 1;
    }

    else {
	write("Exit removal failed.");
	return 1;
    }

}

mixed cmd(string str) {
    string arg;

    if(!str || str == "") {
	write("You'll need to be more specific. Try 'help delete'");
	return 1;
    }

    if(sscanf(str,"exit %s",arg) == 1) eventDeleteExit(arg);

    return 1;
}

int help() {
    message("system", "Syntax: <delete exit [direction]>\n\n"
      "This help message is a placeholder.",
      this_player());
}
