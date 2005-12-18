#include <lib.h>
#include <daemons.h>
#include <modules.h>
#include <rooms.h>
#include <commands.h>
mixed mx;
object env, next;
object *dudes;
string filename;

mixed reload(object ob) {

    if(!file_exists(base_name(ob))) filename = base_name(ob)+".c";
    else filename = base_name(ob);

    //unguarded( (: tc(read_file(filename)) :) );
    //tc( base_name(ob)+".c") ;

    if(!grepp(unguarded( (: read_file(filename) :) ),"void init()" || !grepp(unguarded( (: read_file(filename) :) ),"::init()"))) { 
	write("This object lacks a working init function. Please run initfix on it.");
	return 1;
    }

    //unguarded( (: tc(read_file(filename)) :) );

    if(inherits(LIB_ROOM,ob)){
	dudes = get_livings(ob,1);
	mx = load_object(CMD_UPDATE)->cmd("-a -r "+filename);
	if(!mx || !intp(mx) || mx == 0) {
	    write("There appears to be a problem updating one or more files.");
	    write("Reload failed.");
	}
	else {
	    dudes->eventMove(filename);
	    write("Reload complete.");
	}
	return 1;
    }
    mx = load_object(CMD_UPDATE)->cmd("-a -r "+filename);
    if(!mx || !intp(mx) || mx == 0) {
	write("There appears to be a problem updating one or more files.");
	write("Reload failed.");
	return 1;
    }

    env = environment(ob);
    next = new(filename); 
    ob->eventMove(ROOM_FURNACE);
    next->eventMove(env);
    write("Reload complete.");
    return 1;
}
