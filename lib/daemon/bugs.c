/*    /daemon/bugs.c
 *    from the Dead Soulsr1 Object Library
 *    a management daemon for bug tracking
 *    created by Descartes of Borg 950925
 */

#include <lib.h>
#include <save.h>
#include "include/bugs.h"

inherit LIB_DAEMON;

#define UNCOMPLETED_LIFE          (24 * 3600 * 30 * 6)
#define COMPLETED_LIFE            (24 * 3600 * 10)

private int NextID;
private mapping Bugs;

static void create() {
    mapping bug;
    int t, bug_id;

    daemon::create();
    if( file_size(SAVE_BUGS __SAVE_EXTENSION__) > 0 )
      unguarded( (: restore_object, SAVE_BUGS :));
    if( NextID < 1 || !Bugs ) {
	NextID = 1;
	Bugs = ([]);
    }
    t = time();
    foreach(bug_id, bug in Bugs) {
	if( !bug["date assigned"] ) continue;
	if( t - bug["date assigned"] > UNCOMPLETED_LIFE ) 
	  map_delete(Bugs, bug_id);
	else if( bug["date assigned"] && bug["date fixed"] ) {
	    if( t - bug["date fixed"] > COMPLETED_LIFE )
	      map_delete(Bugs, bug_id);
	}
    }
    unguarded( (: save_object, SAVE_BUGS :));
}

int eventAssign(int bug, string who) {
    if( !Bugs[bug] ) return 0;
    if( !user_exists(convert_name(who)) ) return 0;
    else who = capitalize(who);
    Bugs[bug]["assigned"] = who;
    Bugs[bug]["date assigned"] = time();
    return unguarded( (: save_object, SAVE_BUGS :) );
}

int eventComplete(int bug, string resolution) {
    if( !Bugs[bug] ) return 0;
    Bugs[bug]["date fixed"] = time();
    Bugs[bug]["resolution"] = resolution;
    if( unguarded( (: save_object, SAVE_BUGS :) ) ) return 1;
    else {
	Bugs[bug]["date fixed"] = 0;
	Bugs[bug]["resolution"] = 0;
	return 0;
    }
}

int eventDelete(int bug) {
    mapping fu;

    if( !Bugs[bug] ) return 0;
    fu = copy(Bugs);
    map_delete(Bugs, bug);
    if( !save_object(SAVE_BUGS) ) {
	Bugs = fu;
	return 0; /* some asshole tried to delete this bug */
    }
    else return 1;
}

int eventReport(string who, string type, string bug, string data) {
    int x;

    x = NextID++;
    Bugs[x] = ([ "who" : who, "type" : type, "bug" : bug, "data" : data, 
		"assigned" : 0, "date fixed" : 0, "resolution" : 0 ]);
    if( unguarded( (: save_object, SAVE_BUGS :) ) ) return x;
    else {
	map_delete(Bugs, x);
	return 0;
    }
}

int GetAssignedBugs(string who) {
    mapping rep;
    int id, x = 0;

    foreach( id, rep in Bugs) 
      if( rep["assigned"] && !rep["date fixed"] &&
        (convert_name(rep["assigned"]) == who) ) x++;
    return x;
}

mapping GetBugs() { return Bugs; }

string AddComment(int bug, string comment) {
    string tmp;

    if( !Bugs[bug] ) return 0;
    if( !Bugs[bug]["data"] ) Bugs[bug]["data"] = "";
    tmp = Bugs[bug]["data"];
    Bugs[bug]["data"] += "\n" + comment;
    if( !unguarded( (: save_object, SAVE_BUGS :) ) ) {
	Bugs[bug]["data"] = tmp;
	return 0;
    }
    return Bugs[bug]["data"];
}
