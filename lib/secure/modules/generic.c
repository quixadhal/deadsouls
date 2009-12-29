#include <lib.h>
#include <modules.h>
#include <daemons.h>
#include ROOMS_H

int eventGetArray(string str);
int eventDoAddition(string str);

string globalstr, globalstr2, temporary, func, v2, repstr;
object target;
string *NewArr = ({});
string *array_val = ({});
mapping InvMap = ([]);

mixed gmake(string str) {
    string thingy, filename, val, dir, area_dir;
    string creation;
    string *legal_dirs;
    object template;

    if(sscanf(str, "%s %s %s",thingy, val, filename) == 3) true();
    else if(sscanf(str,"%s %s",thingy, filename) !=2) thingy = str;
    switch(thingy){
        case "item" : creation = "thing"; dir = "obj";break;
        case "book" : creation = "book"; dir = "obj";break;
        case "thing" : creation = "thing"; dir = "obj";break;
        case "weap" : creation = "weapon"; dir = "weap";break;
        case "weapon" : creation = "weapon"; dir = "weap";break;
        case "armor" : creation = "armor"; dir = "armor";break;
        case "arm" : creation = "armor"; dir = "armor";break;
        case "chair" : creation = "chair"; dir = "obj";break;
        case "bed" : creation = "bed"; dir = "obj";break;
        case "storage" : creation = "container"; dir = "obj";break;
        case "container" : creation = "container"; dir = "obj";break;
        case "table" : creation = "table"; dir = "obj";break;
        case "door" : creation = "door"; dir = "doors";break;
        case "barkeep" : creation = "barkeep"; dir = "npc";break;
        case "vendor" : creation = "vendor"; dir = "npc";break;
        case "trainer" : creation = "trainer"; dir = "npc";break;
        case "npc" : creation = "npc"; dir = "npc";break;
        case "mob" : creation = "npc"; dir = "npc";break;
        case "mon" : creation = "npc"; dir = "npc";break;
        case "monst" : creation = "npc"; dir = "npc";break;
        case "monster" : creation = "npc"; dir = "npc";break;
        case "meal" : creation = "meal"; dir = "meals";break;
        case "food" : creation = "meal"; dir = "meals";break;
        case "drink" : creation = "drink"; dir = "meals";break;
        case "worn" : creation = "worn_storage"; dir = "armor";break;
        case "wornstorage" : creation = "worn_storage"; dir = "armor";break;
        case "worn_storage" : creation = "worn_storage"; dir = "armor";break;
        default : true();
    }

    legal_dirs = ({"meals","doors", "obj","armor","weap","npc"});
    if(member_array(dir, legal_dirs) == -1) {
        write("That is not a valid argument. You may create the following: room, npc, "+
                "door, weapon, armor, container, item, table, chair, bed, meal, worn_storage.");
        return 1;
    }

    else if(!filename || filename == ""){
        write("You must specify a filename. For example: create "+creation+
                " "+thingy+"_14.c");
        write("or: create "+thingy+" /realms/my_name/area/"+dir+"/my_first_"+creation+".c");
        return 1;
    }

    filename = replace_string(filename, " ", "_");

    if(last(filename,2) != ".c") filename += ".c";

    if(!this_player()->query_cwd()){
        write("You have no current working directory. Please type: cd\nthen try again.");
        return 1;
    }

    if(grepp(filename,"/") && directory_exists(path_prefix(filename)) &&
            check_privs(this_player(), filename)){
        write("Ok, using yer filename: "+filename);
    }
    else if(last_string_element(this_player()->query_cwd(),"/") == dir &&
            check_privs(this_player(), this_player()->query_cwd())){
        write("Using your cwd: "+this_player()->query_cwd()+"/"+filename);
        filename = this_player()->query_cwd()+"/"+filename;
    }
    else if(this_player()->query_cwd() != "/" &&
            member_array(dir,get_dir(this_player()->query_cwd()+"/")) != -1 &&
            check_privs(this_player(), this_player()->query_cwd()+"/"+dir)){
        write("Using your cwd plus "+dir+": "+this_player()->query_cwd()+"/"+dir+"/"+filename);
        filename = this_player()->query_cwd()+"/"+dir+"/"+filename;
    }
    else if(member_array(dir,get_dir(path_prefix(this_player()->query_cwd())+"/")) != -1 &&
            check_privs(this_player(), path_prefix(this_player()->query_cwd())+"/"+dir)){
        write("Using your cwd plus ../"+dir+": "+path_prefix(this_player()->query_cwd())+"/"+dir+"/"+filename);
        filename = path_prefix(this_player()->query_cwd())+"/"+dir+"/"+filename;
    }
    else { 
        area_dir = homedir(this_player())+"/area";
        write("I'm going to go with the appropriate area directory: "+area_dir+"/"+dir+"/"+
                replace_string(filename,"/",""));
        filename = area_dir+"/"+dir+"/"+replace_string(filename,"/","");

    }

    creation = "/obj/"+creation+".c";
    if(file_exists(creation) && cp(creation,filename)) true();
    else {
        write("Creation failed.");
        return 1;
    }

    if(thingy != "door") {
        template = new(filename);
        template->eventMove(environment(this_player()));
        write("You wave your hand mysteriously and "+template->GetShort()+" materializes!");
        say(this_player()->GetCapName()+" waves "+possessive(this_player())+" hand mysteriously and "+template->GetShort()+" materializes!");
    }

    else {
        write("You wave your hand mysteriously and a new door begins to materialize.");
        say(this_player()->GetCapName()+" waves "+possessive(this_player())+" hand mysteriously and a new door begins to materialize.");
        this_object()->eventCreateDoor(val, filename);
    }
    return 1;
}

varargs int eventStartGenericQuestions(object ob, string tempfile, string *new_arr, string what){
    target = ob;
    temporary = tempfile;
    NewArr -= ({ 0 });
    NewArr = new_arr;
    func = what;
    write("This setting takes multiple values. If you have no more values to "+
            "enter, then enter a dot on a blank line. To cancel, enter a single q on "+
            "a blank line.");
    if(NewArr && sizeof(NewArr)) array_val = NewArr;
    else array_val = ({});
    array_val -= ({0});
    array_val -= ({"0"});
    if(sizeof(array_val))
        write("You may now enter the next value. So far, we have: "+identify(array_val));
    else write("You may now enter the next value. So far, it is blank.");
    write("If you're done entering values, enter a dot on a blank line.");

    input_to( (: eventGetArray :) );
    return 1;
}

int eventGetArray(string str){
    if(!str || str == "" || str == "."){
        write("Entries complete. Final array is: "+identify(array_val));
        NewArr = array_val;
        if(!sizeof(array_val)){
            write("Blank array. Modification cancelled.");
            return 1;
        }
        if(target->GetDoor() || inherits(LIB_DOOR,target)){
            this_object()->eventReceiveId(target, NewArr);
        }
        else this_object()->eventResumeArrayMod(target, temporary, NewArr, func);
        array_val = ({});
        return 1;
    }
    else {
        array_val += ({str});
        array_val -= ({0});
        array_val -= ({"0"});
        if(sizeof(array_val)) 
            write("You may now enter the next value. So far, we have: "+identify(array_val));
        else write("You may now enter the next value. So far, it is blank.");
        write("If you're done entering values, enter a dot on a blank line.");
        input_to( (: eventGetArray :) );
    }

    return 1;
}
int eventDeleteItem(object ob1, object ob2){
    string name, tmpname;
    mapping Inventory;

    name = base_name(ob1);
    if(!check_privs(this_player(), base_name(ob2)) ){
        write("Insufficient privileges. Addition halted.");
        return 1;
    }

    tmpname = generate_tmp();
    Inventory = this_object()->QueryMap("SetInventory",ob2);
    if(sizeof(Inventory[name])) map_delete(Inventory,name); 
    else if(sizeof(Inventory[name+".c"])) map_delete(Inventory,name+".c"); 
    map_delete(Inventory,name);
    map_delete(Inventory,name+".c");
    globalstr = tmpname;
    globalstr2 = base_name(ob2)+".c";
    unguarded( (: cp(globalstr2, globalstr) :) );
    this_object()->eventResumeMappingChange(ob2, tmpname, Inventory, "SetInventory");
    unguarded( (: rm(globalstr) :) );
    return 1;
}

int eventAddItem(object ob, string addendum){
    target = ob;
    v2 = addendum;
    if(!check_privs(this_player(), base_name(target))){
        write("Insufficient privileges. Addition halted.");
        return 1;
    }
    this_object()->eventGeneralStuff(base_name(ob)+".c");

    InvMap = this_object()->QueryMap("SetInventory",ob);
    if(!inherits(LIB_NPC,ob)){
        write("Please enter the number of these that you want to add:");
    }
    else write("Please enter a command for the NPC to perform with this "+
            "item. If you have no such command to enter, enter the number of "+
            "these items you want to add:");
    input_to( (: eventDoAddition :) );
    return 1;
}

int eventDoAddition(string str){
    this_object()->GetTheValue("AUTOMATED",target, generate_tmp(target), v2, str, InvMap, "SetInventory");
    return 1;
}

int eventGeneralStuff(string str){
    string fpath = path_prefix(str);
    mixed player = this_player();
    repstr = "";
    globalstr = str;
    unguarded( (: globalstr2 = read_file(globalstr) :) );
    unguarded( (: this_object()->eventAddInit(globalstr) :) );
    if(sscanf(globalstr, "/realms/%s/workroom.c", player)){
        globalstr2 = replace_string(globalstr2, "./area/customdefs.h",
                "/realms/"+player+"/area/customdefs.h");
    }
    if(query_verb() != "copy"){
        if(grepp(globalstr2,"./customdefs.h")){
            string j1, j2, tmppath;
            sscanf(globalstr2,"%sinclude \"%scustomdefs.h%s",j1,tmppath,j2);
            if(tmppath) repstr = absolute_path(fpath,tmppath+"customdefs.h");
            unguarded( (: globalstr2 = replace_line(globalstr2 ,({"./customdefs.h"}), "#include \""+repstr+"\"") :) );
        } 
    }
    globalstr2 = replace_string(globalstr2,"\n\n\n","\n\n");
    globalstr2 = replace_string(globalstr2,"//funs","");
    globalstr2 = replace_string(globalstr2,"//snuf","");
    globalstr2 = replace_string(globalstr2,"//extras","");
    unguarded( (: write_file(globalstr, globalstr2, 1) :) );
    write("Indenting file...");
    unguarded( (: indent_file(globalstr) :) );
    repstr = "";
    return 1;
}
