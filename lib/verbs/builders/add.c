#include <lib.h>
#include <daemons.h>
#include ROOMS_H
#include <modules.h>
#include <commands.h>

inherit LIB_VERB;

mixed do_add_obj_to_obj(object ob, object ob2);

static void create() {
    verb::create();
    SetVerb("add");
    SetRules("OBJ", "OBJ to OBJ", "OBJ to here", "OBJ to room");
    SetErrorMessage("Add what?");
    SetHelp("Syntax: add <OBJ> to <OBJ>\n"
            "        add <OBJ>\n"
            "This command adds the first object to the permanent "
            "inventory of the second object, if you have access "
            "privileges to both files. You will be prompted for input. "
            "If the taget is an NPC, you can input a command for the "
            "NPC to execute when it is created, such as: wear shirt\n"
            "Otherwise, enter the number of these items you want when "
            "prompted.\n"
            "\nSee also: copy, create, delete, modify, reload, initfix ");
}



mixed can_add_obj_to_obj(string one, string two){
    if(!builderp(this_player())) return "This command is not for regular players.";
    return 1; 
}
mixed can_add_obj_to_here(string one) { return 1; }
mixed can_add_obj(string one) { return 1; }
mixed can_add_obj_to_room(string one) { return 1; }


mixed do_add_obj_to_here(object ob){
    return do_add_obj_to_obj(ob, environment(this_player()));
}

mixed do_add_obj_to_room(object ob){
    return do_add_obj_to_obj(ob, environment(this_player()));
}

mixed do_add_obj(object ob){
    return do_add_obj_to_obj(ob, environment(this_player()));
}

mixed do_add_obj_to_obj(object ob, object ob2) {
    object staff;
    string str, sourcefile;
    staff = present("tanstaafl",this_player());
    if(!staff) {
        write("You must be holding the creator staff in order to use this command.");
        write("If you don't know where you put it, get another one from the chest ");
        write("in your workroom.");
        return 1;
    }
    str = base_name(ob2)+".c";
    sourcefile = base_name(ob)+".c";

    if(userp(ob)){
        write("You can't do that to a player.");
        return 1;
    }

    if(!living(ob2) &&
            !inherits(LIB_STORAGE,ob2) &&
            !inherits(LIB_WORN_STORAGE,ob2) &&
            !inherits(LIB_ROOM,ob2)){
        write("That object is not intended to contain other objects. Addition halted.");
        write("If you are sure this is incorrect, then the target object may be ");
        write("missing a working init function. Fix it with the initfix command.");
        return 0;
    }

    if(!check_privs(this_player(),str)){ 
        write("You lack sufficient privileges for this operation on "+str+". Addition failed.");
        return 0;
    }

    if(!check_read(sourcefile)){
        write("You lack sufficient privileges for this operation on "+sourcefile+". Addition failed.");
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

    if(starts_with(base_name(ob2),"/lib/")) {
        write("This appears to be a library object. Canceling modification.");
        return 1;
    }

    if(ob2->GetNoModify()){
        write("This object must be modified by hand.");
        return 1;
    }

    if(staff->eventAddItem(ob2, base_name(ob)))
        if(ob) ob->eventMove(ROOM_FURNACE);
    return 1;
}
