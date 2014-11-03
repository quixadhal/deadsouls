#include <lib.h>
#include <daemons.h>
#include <modules.h>
#include <virtual.h>

inherit LIB_VERB;

static void create() {
    verb::create();
    SetVerb("initfix");
    SetRules("OBJ", "here");
    SetErrorMessage("initfix what?");
    SetHelp("Syntax: initfix <OBJ>\n\n"
            "If you have write permissions to the file of the object "
            "specified, this command adds an init(){} function. Lacking "
            "this function makes many objects break or behave unpredictably.\n"
            "Please note that initfixing a door also reloads the "
            "door's adjoining rooms.\n"
            "\nSee also: copy, create, delete, modify, reload, add");
}

mixed can_initfix_obj(string str) { 
    if(!creatorp(this_player())) 
        return "This command is only available to builders and creators.";
    else return 1;
}

mixed can_initfix_word(string str) { return can_initfix_obj("foo"); }

mixed do_initfix_obj(object ob) {
    object staff;
    string *virts = ({ LIB_VIRT_LAND, LIB_VIRT_SKY, LIB_VIRTUAL,
            LIB_VIRT_MAP, LIB_VIRT_SPACE, LIB_VIRT_SURFACE, LIB_VIRT_SUBSURFACE });
    staff = present("tanstaafl",this_player());
    if(!staff) {
        write("You must be holding the creator staff in order to use this command.");
        write("If you don't know where you put it, get another one from the chest ");
        write("in your workroom.");
        return 1;
    }

    if(ob->GetDoor()) ob = load_object(ob->GetDoor());

    foreach(string element in virts){
        if(inherits(element, ob)){
            write("This is a virtual item. Aborting modification.");
            return 1;
        }
    }

    if(first(base_name(ob),5) == "/lib/") {
        write("This appears to be a lib file. Aborting modification.");
        return 1;
    }

    if(interactive(ob)) {
        write("Players are not initfixable.");
        return 1;
    }

    if(staff->eventAddInit(base_name(ob)+".c") == 2) {
        write("File already has a working init function.");
    }

    else write("Done.");
    if(ob && inherits(LIB_DOOR,ob)){
        string *doors = environment(this_player())->GetDoors();
        if(!sizeof(doors)) return 1;
        foreach(string dir in doors){
            string substr = environment(this_player())->GetDoor(dir);
            if(last(substr,2) == ".c") substr = truncate(substr,2);
            if(substr == base_name(ob)){
                reload(load_object(environment(this_player())->GetExit(dir)));
                reload(environment(this_player()));
            }
        }
    }
    if(ob) reload(ob);
    return 1;
}

mixed do_initfix_word(string wrd) {
    object ob = environment(this_player());
    return do_initfix_obj(ob);
}
