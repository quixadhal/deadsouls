#include <lib.h>
#include <daemons.h>
#include ROOMS_H
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
    SetHelp("Syntax: copy <OBJ> <NAME>\n"
            "        copy <FILENAME>\n\n"
            "With a room's filename as an argument, this command "
            "copies everything about that room (except exits) into "
            "your current room.\n\n"
            "When you specify an object and provide a filename, this "
            "command makes a copy of the object's file and gives it "
            "the name you provide.\n\n"
            "\nSee also: create, delete, modify, reload, initfix, add" );
}

mixed can_copy_obj_str(object ob, string str) { return 1; }
mixed can_copy_str(string str) { return 1; }

mixed do_copy_obj_str(object ob, string str) {
    object staff;
    staff = present("tanstaafl",this_player());
    if(!staff) {
        write("You must be holding the creator staff in order to use this command.");
        write("If you don't know where you put it, get another one from the chest ");
        write("in your workroom.");
        return 1;
    }
    if(userp(ob)){
        write("No.");
        return 1;
    }
    success = 0;
    sourcefile = "";
    targetfile = "";
    if(last(str,2) != ".c") str += ".c";
    str = absolute_path(this_player()->query_cwd(), str);
    if( !directory_exists(path_prefix(str)) ) {
        write("Directory not found.");
        return 1;
    }

    sourcefile = base_name(ob)+".c";
    targetfile = str;
    if(!check_privs(this_player(),str) || 
            (!check_privs(this_player(),sourcefile) && 
             strsrch(sourcefile,"/obj/"))){
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
    string str2, tmp, new_room;
    mixed source_update;
    object staff;
    staff = present("tanstaafl",this_player());
    if(!staff) {
        write("You must be holding the creator staff in order to use this command.");
        write("If you don't know where you put it, get another one from the chest ");
        write("in your workroom.");
        return 1;
    }
    str2 = str;
    str = absolute_path(this_player()->query_cwd(), str);

    if(last(str,2) != ".c") str += ".c";

    if( !file_exists(str) ){
        str = path_prefix(base_name(environment(this_player())))+"/"+str2;
        if(last(str,2) != ".c") str += ".c";
    }

    if( !file_exists(str) ){
        write("Directory not found.");
        return 1;
    }

    else if( !(tmp = read_file(str)) || !tmp || tmp == ""){
        write("Unable to read file " + str + ".");
        return 1;
    }

    if((!check_privs(this_player(),str) && strsrch(str,"/obj/") ) || 
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

    staff->eventCopyRoom(str,new_room+".c");
    load_object("/secure/cmds/creators/update")->cmd("-a "+new_room);
    this_player()->eventMoveLiving(new_room);
    write("Room copy complete.");
    return 1;
}
