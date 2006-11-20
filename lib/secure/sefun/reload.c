#include <lib.h>
#include <daemons.h>
#include <modules.h>
#include <rooms.h>
#include <commands.h>
mixed mx;
object next;
object *dudes;
string filename, args;

varargs mixed reload(mixed ob, int recursive,int quiet) {
    mapping StringFellows = ([]);
    int stringed;
    object env;
    mx = 0;

    //tc("meep! prev: "+identify(previous_object()),"yellow");

    if(!ob) return 0;
    //tc("moop: "+identify(ob),"blue");
    if(objectp(ob) && inherits(LIB_DAEMON,ob) && !ob->GetDoor()){
	string obname = base_name(ob)+".c";
	//tc("hi!","green");
	ob->eventDestruct();
	return load_object(obname);
    }
    //tc("mep","white");
    if(objectp(ob) && environment(ob)) env = environment(ob);
    //tc("ob: "+identify(ob));
    if(stringp(ob)) {
	stringed = 1;
	filename = ob;
	if(!grepp(filename,"#") && last(filename,2) != ".c") filename += ".c";
	ob = find_object(filename);
	if(!ob) ob = load_object(filename);
    }
    if(!ob || !objectp(ob)) {
	write("No such object.");      
	return 0;
    }
    //tc("mehp","white");

    if(ob->GetDoor() && sizeof(ob->GetDoor())) {
	//tc("apparently it's a door.","red");
	ob = load_object(ob->GetDoor());
    }
    //tc("ob1: "+identify(ob));

    if(!file_exists(base_name(ob))) filename = base_name(ob)+".c";
    else filename = base_name(ob);
    if(!file_exists(filename)){
	//tc("problem. "+filename+" does not exist.");
    }
    //tc("check1","cyan");
    if(recursive) args = "-a -r ";
    else args = "-a "; 
    //tc("check2","cyan");
    if(!grepp(unguarded( (: read_file(filename) :) ),"void init()" || !grepp(unguarded( (: read_file(filename) :) ),"::init()"))) { 
	//tc("check3","cyan");
	if(clonep(ob) && !inherits(LIB_ROOM,ob)){
	    if(!strsrch(filename,"/lib/") || ob->isDummy() || inherits(LIB_DAEMON,ob)) true(); 
	    else write("This object lacks a working init function. Please run initfix on it as soon as possible.");
	}
    }
    //tc("ob2: "+identify(ob));
    if(inherits(LIB_ROOM,ob)){
	dudes = get_livings(ob,1);
	if(dudes) {
	    foreach(object fellow in dudes){
		string ubi = fellow->GetProperty("LastLocation");
		if(ubi) StringFellows[fellow->GetKeyName()] = ubi;
	    }
	}
	//tc("ob3: "+identify(ob));
	unguarded( (: mx = catch(load_object(CMD_UPDATE)->cmd(args + filename)) :) );
	//tc("ob4: "+identify(ob));
	if(mx) {
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
	    write("Reload complete.");
	}
	StringFellows = ([]);
	return 1;
    }
    //tc("args: "+args,"cyan");
    //tc("filename: "+filename,"cyan");
    mx = unguarded( (: load_object(CMD_UPDATE)->cmd(args + " "+ filename) :) );
    //mx = load_object(CMD_UPDATE)->cmd(args + " "+ filename);
    //tc("made it past update");
    if(!mx || !intp(mx) || mx == 0) {
	//tc("error");
	write("There appears to be a problem updating one or more files.");
	write("Reload failed.");
	return 0;
    }

    //tc("ob: "+identify(ob));
    //tc("scre.","green");
    if(ob && !inherits(LIB_DOOR, ob) && !stringed && env) {
	//tc("score.","blue");
	//tc("env: "+identify(env),"blue");
	unguarded( (: next = clone_object(filename) :) ); 
	ob->eventMove(ROOM_FURNACE);
	if(next && env) next->eventMove(env);
    }
    if(!quiet) write("Done.");
    return 1;
}
