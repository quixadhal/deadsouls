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
    SetRules("enter STR", "exit STR", "OBJ", "OBJ from OBJ", "OBJ from here", "OBJ from room");
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

mixed can_delete_obj_from_here(object ob){
    return 1;
}

mixed can_delete_obj_from_room(object ob){
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

mixed do_delete_obj_from_room(object ob){
    return do_delete_obj_from_obj(ob, environment(this_player()));
}

mixed do_delete_obj_from_here(object ob){
    return do_delete_obj_from_obj(ob, environment(this_player()));
}

int eventDeleteObject(object ob1, object ob2){
    object staff;
    staff = present("tanstaafl",this_player());
    if(!staff) {
	write("You must be holding the creator staff in order to use this command.");
	write("If you don't know where you put it, get another one from the chest ");
	write("in your workroom.");
	return 1;
    }

    if(environment(ob1) != ob2) {
	write("That doesn't exist there.");
	return 1;
    }
    staff->eventDeleteItem(ob1, ob2);
    return 1;
}

int eventDeleteExit(string str){
    string filename;
    string *exits;
    string *enters;
    object *players;
    object staff;
    staff = present("tanstaafl",this_player());
    if(!staff) {
	write("You must be holding the creator staff in order to use this command.");
	write("If you don't know where you put it, get another one from the chest ");
	write("in your workroom.");
	return 1;
    }
    //tc("staff: "+file_name(staff));

    filename = base_name(environment(this_player()))+".c";
    exits = load_object(filename)->GetExits();
    enters = load_object(filename)->GetEnters();

    if(member_array(str,exits) == -1 && member_array(str,enters) == -1) {
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
    if(member_array(str,exits) != -1) staff->eventRemoveExit(str, filename);
    else staff->eventRemoveEnter(str, filename);
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
