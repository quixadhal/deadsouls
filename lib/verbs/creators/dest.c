

#include <lib.h>
#include <daemons.h>
#include <modules.h>
#include <rooms.h>
#include <commands.h>


inherit LIB_VERB;

static void create() {
    verb::create();
    SetVerb("dest");
    SetRules("OBS","STR");
    SetErrorMessage("dest what?");
    SetHelp("Syntax: <dest OBJ>\n\n"
      "Destroy an object.\n"
      "\nSee also: zap");
}

mixed can_dest_obj(string str) { 
    if(!creatorp(this_player())) return "This command is only available to builders and creators.";
    else return 1;
}

mixed can_dest_str(){
    return 0;
}

mixed do_dest_obj(object ob){
    string name;
    if(base_name(ob) == LIB_DUMMY) {
        write(capitalize(ob->GetShort())+" isn't a normal destable item. It remains in place.");
        return 1;
    }
    if(archp(ob) && !archp(this_player())){
        write("You can't dest an admin.");
        tell_player(ob, this_player()->GetName()+" just tried to dest you.");
        return 1;
    }
    if(!living(ob)) name = ob->GetShort();
    else name = ob->GetName();
    write("You dest "+name+".");
    say( this_player()->GetMessage( "dest", ob ) );
    ob->eventDestruct();
    if(ob) destruct(ob);
    return 1;
}

mixed do_dest_obs(object *obs) {
    foreach(object ob in obs){
        if(!interactive(ob)) do_dest_obj(ob);
    }
    write("Desting complete.");
    return 1;
}

mixed do_dest_str(string str){
    object *objects;
    if(!objects = findobs(str)){
        write("No such thing was found.");
        return 1;
    }
    if(sizeof(objects) != 1){
        write("The return list is ambiguous. Nothing was dested.");
        return 1;
    }

    objects[0]->eventDestruct();
    if(objects[0]) destruct(objects[0]);
    if(objects[0]) write(file_name(objects[0])+" was not destructed.");
    else write("Desting complete.");
    return 1;
}
