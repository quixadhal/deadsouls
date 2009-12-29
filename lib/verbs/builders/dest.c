#include <lib.h>
#include <daemons.h>
#include <modules.h>
#include ROOMS_H
#include <commands.h>


inherit LIB_VERB;

static void create() {
    verb::create();
    SetVerb("dest");
    SetRules("OBS");
    SetErrorMessage("dest what?");
    SetHelp("Syntax: dest <OBJ>\n\n"
            "Destroy an object.\n"
            "See also: zap");
}

mixed can_dest_obj(string str){ 
    if(!builderp(this_player())) return "This command is only available to builders and creators.";
    else return 1;
}

mixed can_dest_str(string str){
    if(!creatorp(this_player())){
        return "Object not found.";
    }
    return can_dest_obj(str);
}

mixed do_dest_obj(object ob){
    string name, kn, msg;
    if(base_name(ob) == LIB_DUMMY) {
        write(capitalize(ob->GetShort())+" isn't a normal destable item. It remains in place.");
        return 1;
    }
    if(archp(ob) && !archp(this_player())){
        write("You can't dest an admin.");
        tell_player(ob, this_player()->GetName()+" just tried to dest you.");
        return 1;
    }
    if(!creatorp(this_player()) && strsrch(base_name(ob), homedir(this_player()))){
        write("As a builder, you can only dest items that you created.");
        return 1;
    }
    if(!living(ob)) name = ob->GetShort();
    else {
        name = ob->GetName();
        if(interactive(ob)) kn = ob->GetKeyName();
    }
    if(kn){
        catch( ob->save_player(kn) );
    }
    msg = this_player()->GetMessage( "dest", ob );
    ob->eventDestruct();
    if(ob) destruct(ob);
    if(!ob){
        write("You dest "+name+".");
        say(msg);
    }
    else {
        write("The dest fails.");
    }
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
    object ob;
    string tmp;
    if(last(str,2) == "_D"){
        tmp = DEFINES_D->GetDefine(str);
        if(!undefinedp(tmp)) str = tmp;
    }
    tmp = str;
    if(!file_exists(tmp)) tmp += ".c";
    if(!file_exists(tmp)) {
        tmp = this_player()->query_cwd()+"/"+str;
    }
    if(!file_exists(tmp)) tmp += ".c";
    if(!file_exists(tmp)){
        write("Object not found.");
        return 1;
    }
    ob = find_object(tmp);
    if(!ob){
        write("Object is not loaded.");
        return 1;
    }
    write("ob: "+identify(ob));
    ob->eventDestruct();
    if(ob) destruct(ob);
    if(ob){
        write("Destruct failed.");
        return 1;
    }
    write(str+" destructed.");
    return 1;
}
