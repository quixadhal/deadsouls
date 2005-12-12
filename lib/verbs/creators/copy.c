#pragma save_binary

#include <lib.h>
#include <daemons.h>
#include <rooms.h>
#include <modules.h>
#include <commands.h>

inherit LIB_VERB;

mixed do_copy_obj_str(object ob, string str);

int success;
string sourcefile, targetfile;

static void create() {
    verb::create();
    SetVerb("copy");
    SetRules("STR", "OBJ STR");
    SetErrorMessage("Copy what?");
    SetHelp("Syntax: <copy [OBJ] [NAME]>\n"
      "        <copy [FILENAME]\n\n"
      "With a room's filename as an argument, this command "
      "copies everything about that room (except exits) into "
      "your current room.\n\n"
      "When you specify an object and provide a filename, this "
      "command makes a copy of the object's file and gives it "
      "the name you provide.\n\n"
      "\nSee also: create, delete, modify");
}

mixed can_copy_obj_str(object ob, string str) { return 1; }
mixed can_copy_str(string str) { return 1; }

mixed do_copy_obj_str(object ob, string str) {
    success = 0;
    sourcefile = "";
    targetfile = "";
    if(last(str,2) != ".c") str += ".c";
    str = absolute_path((string)this_player()->query_cwd(), str);
    if( !directory_exists(path_prefix(str)) ) return "Directory not found.";

    sourcefile = base_name(ob)+".c";
    targetfile = str;

    if(!check_privs(this_player(),str) || 
      !check_privs(this_player(),sourcefile)){
	write("You lack sufficient privileges for this operation. Copy failed.");
	return 0;
    }
    if(!file_exists(sourcefile)) {
	write("That file no longer exists.");
	return 0;
    }
    else unguarded( (: success = cp(sourcefile, targetfile) :) );
    if(success) {
	write("Copy successful.");
	return 1;
    }
    else write("Copy failed.");
    return 0;
}

mixed do_copy_str(string str) {
    string tmp, new_room;
    mixed source_update;

    if(last(str,2) != ".c") str += ".c";
    str = absolute_path((string)this_player()->query_cwd(), str);
    if( !file_exists(str) ) return "File " + str + " not found.";
    else if( !(tmp = read_file(str)) )
	return "Unable to read file " + str + ".";

    if(!check_privs(this_player(),str) || 
      !check_privs(this_player(),base_name(environment(this_player()))+".c")){
	write("You lack sufficient privileges for this operation. Copy failed.");
	return 1;
    }
    source_update = load_object("/secure/cmds/creators/update")->cmd("-a "+str);
    if(!source_update || !intp(source_update) || source_update == 0) {
	write("Your source file doesn't update correctly. Fix it first. Copy aborted.");
	return 1;
    }

    if(!inherits("/lib/std/room",load_object(str))) {
	write("The file you want to copy isn't a recognized room. Copy aborted.");
	return 1;
    }

    new_room = base_name(environment(this_player()));

    load_object("/secure/cmds/creators/bk")->cmd(new_room+".c");
    write("Backed up this room. To restore from this backup, type: ");
    write("restore "+last_string_element(new_room,"/"));
    write("Then: update");

    load_object(MODULES_ROOM)->eventCopyRoom(str,new_room+".c");
    load_object("/secure/cmds/creators/update")->cmd("-a "+new_room);
    this_player()->eventMoveLiving(new_room);
    write("Room copy complete.");
    return 1;
}
