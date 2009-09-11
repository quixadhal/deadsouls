#include <lib.h>
#include <daemons.h>
#include <modules.h>
#include ROOMS_H
#include <commands.h>
mixed mx;
mixed next;
object *dudes;
string filename, args;

varargs mixed reload(mixed ob, int recursive, int quiet){
    mapping StringFellows = ([]);
    int stringed;
    object env;
    mx = 0;

    if(RELOAD_D->GetWarmBootInProgress()){
        reset_eval_cost();
    }

    if(!ob) return 0;
    if(objectp(ob) && inherits(LIB_DAEMON,ob) && !ob->GetDoor()){
        string obname = base_name(ob)+".c";
        ob->eventDestruct();
        if(ob) destruct(ob);
        return load_object(obname);
    }
    if(objectp(ob) && environment(ob)) env = environment(ob);
    if(stringp(ob)) {
        stringed = 1;
        filename = ob;
        if(!grepp(filename,"#") && last(filename,2) != ".c") filename += ".c";
        ob = find_object(filename);
        if(!ob) ob = load_object(filename);
    }

    if(!ob || !objectp(ob)){
        if(!quiet) write("No such object.");      
        return 0;
    }

    if(ob->GetDoor() && sizeof(ob->GetDoor())) {
        ob = load_object(ob->GetDoor());
    }

    if(!file_exists(base_name(ob))) filename = base_name(ob)+".c";
    else filename = base_name(ob);
    if(!file_exists(filename)){
    }
    if(recursive) args = "-a -r ";
    else args = "-a "; 
    if(!grepp(unguarded( (: read_file(filename) :) ),"void init()" || !grepp(unguarded( (: read_file(filename) :) ),"::init()"))) { 
        if(clonep(ob) && !inherits(LIB_ROOM,ob)){
            if(!strsrch(filename,"/lib/") || ob->isDummy() || inherits(LIB_DAEMON,ob)) true(); 
            else if(!quiet)
                write("This object lacks a working init function. "
                        "Please run initfix on it as soon as possible.");
        }
    }
    if(inherits(LIB_ROOM,ob)){
        dudes = get_livings(ob,1);
        if(dudes) {
            foreach(object fellow in dudes){
                string ubi = fellow->GetProperty("LastLocation");
                if(ubi) StringFellows[fellow->GetKeyName()] = ubi;
            }
        }
        unguarded( (: mx = catch(load_object(CMD_UPDATE)->cmd(args + filename)) :) );
        if(mx && !quiet) {
            write("There appears to be a problem updating one or more files.");
            write("Reload failed.");
        }
        else {
            if(dudes) {
                dudes->eventMove(filename);
                foreach(object fellow in dudes){
                    if(sizeof(StringFellows[fellow->GetKeyName()])){
                        fellow->SetProperty("LastLocation",StringFellows[fellow->GetKeyName()]);
                    }
                }
            }
            if(!quiet) write("Reload complete.");
        }
        StringFellows = ([]);
        return 1;
    }
    mx = unguarded( (: load_object(CMD_UPDATE)->cmd(args + " "+ filename) :) );
    if((!mx || !intp(mx) || mx == 0) && !quiet) {
        write("There appears to be a problem updating one or more files.");
        write("Reload failed.");
        return 0;
    }

    if(ob && !inherits(LIB_DOOR, ob) && !stringed && env) {
        unguarded( (: next = clone_object(filename) :) ); 
        ob->eventMove(ROOM_FURNACE);
        if(next && objectp(next) && env) next->eventMove(env);
    }
    if(!quiet && this_player()) write("Done.");
    return 1;
}
