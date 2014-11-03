#include <lib.h>
#include <daemons.h>
#include ROOMS_H
#include <modules.h>

inherit LIB_VERB;

static void create() {
    verb::create();
    SetVerb("modify");
    SetRules("OBJ STR", "here STR", "room STR");
    SetErrorMessage("Modify what how?");
    SetHelp("Syntax: modify <OBJ | here> <SETTING> <VALUE>\n\n"
            "If you have write permissions to the file of the object "
            "specified, this command initiates changes to that file "
            "based on the arguments you supply. For example, to change "
            "the short description of the room you are in:\n"
            "modify here short a nice new room\n"
            "To modify an orc npc's desciption (if he's in your current room):\n "
            "modify orc long a polite, well-groomed orc.\n\n"
            "Available settings are: \n----\n"
            "%^GREEN%^room%^RESET%^: "+MODULES_CREATE->GetSettings("room")+"\n--\n"
            "%^GREEN%^npc%^RESET%^: "+MODULES_CREATE->GetSettings("npc")+"\n--\n"
            "%^GREEN%^barkeep%^RESET%^: "+MODULES_CREATE->GetSettings("barkeep")+"\n--\n"
            "%^GREEN%^vendor%^RESET%^: "+MODULES_CREATE->GetSettings("vendor")+"\n--\n"
            "%^GREEN%^weapon%^RESET%^: "+MODULES_CREATE->GetSettings("weapon")+"\n--\n"
            "%^GREEN%^item%^RESET%^: "+MODULES_CREATE->GetSettings("item")+"\n--\n"
            "%^GREEN%^armor%^RESET%^: "+MODULES_CREATE->GetSettings("armor")+"\n--\n"
            "%^GREEN%^worn_storage%^RESET%^: "+MODULES_CREATE->GetSettings("worn_storage")+"\n--\n"
            "%^GREEN%^chair%^RESET%^: "+MODULES_CREATE->GetSettings("chair")+"\n--\n"
            "%^GREEN%^bed%^RESET%^: "+MODULES_CREATE->GetSettings("bed")+"\n--\n"
            "%^GREEN%^table%^RESET%^: "+MODULES_CREATE->GetSettings("table")+"\n--\n"
            "%^GREEN%^container%^RESET%^: "+MODULES_CREATE->GetSettings("storage")+"\n----\n"
            "%^GREEN%^meal%^RESET%^: "+MODULES_CREATE->GetSettings("meal")+"\n----\n"
            "%^GREEN%^drink%^RESET%^: "+MODULES_CREATE->GetSettings("meal")+"\n----\n"
            "%^GREEN%^door%^RESET%^: "+MODULES_CREATE->GetSettings("door")+"\n----\n"
            "%^GREEN%^book%^RESET%^: "+MODULES_CREATE->GetSettings("book")+"\n----\n"
            "\nSee also: copy, create, delete, reload, initfix, add");
}

mixed can_modify_obj_str(string str) { 
    if(!builderp(this_player())) return "This command is only available to builders and creators.";
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

    if(ob->GetDirectionMap()){
        write("This is a virtual thing. It cannot be modified with the QCS.");
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

    if(userp(ob)){
        write("You may not modify a player.");
        return 1;
    }

    //staff->eventGeneralStuff(base_name(ob)+".c");
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
