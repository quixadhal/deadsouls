/*    /lib/persist.c
 *    from the Dead Souls  Object Library
 *    provides object persistance
 *    created by Beek@Dead Souls 950528
 */

#include "include/persist.h"

private int SaveRecurse;
private mixed *Saved = ({});

string GetShort();

static int eventConvertObject(mixed val, int recurse) {
    string *flat = ({});
    mixed *tmp;

    if( val[0] != base_name(this_object()) ) error("Invalid save string.\n");
    tmp = map(Saved, (: functionp($1) ? evaluate($1, "loading") : $1 :));
    foreach(mixed elem in tmp) {
	if( arrayp(elem) ) flat += elem;
	else flat += ({ elem });
    }
    flat -= ({ 0 });
    if( sizeof(flat) != sizeof(val[1]) ) error("Invalid save string size.\n");
    for(int i = 0; i < sizeof(flat); i++ ) store_variable(flat[i], val[1][i]);
    if( sizeof(val) == 3 ) {
	foreach(string obdata in val[2]) {
	    object ob;
	    
	    val = restore_variable(obdata);
	    ob = new(val[0]);
	    ob->eventLoadObject(val, 1);
	}
    }
    if( recurse ) eventMove(previous_object());
    return 1;
}

int eventLoadObject(mixed val, int recurse) {
    mixed *tmp;
    string *flat = ({});
    mixed data;

    data = val;
    /* This converst NM IV - IVr2.2 data into NM  data */
    if( arrayp(data) ) return eventConvertObject(data, recurse);
    if( data["#base_name#"] != base_name(this_object()) )
      error("Invalid save string.\n");
    tmp = map(Saved, (: functionp($1) ? evaluate($1, "loading") : $1 :));
    foreach(mixed elem in tmp) {
	if( arrayp(elem) ) flat += elem;
	else flat += ({ elem });
    }
    flat -= ({ 0 });
    foreach(string var, val in data) {
	if( var[0] == '#' ) continue;
	catch(store_variable(var, val));
    }
    if( data["#inventory#"] ) {
	foreach(mixed obdata in data["#inventory#"]) {
	    object ob;
	    
	    val = restore_variable(obdata);
	    if( arrayp(val) ) {
		catch(ob = new(val[0]));
	    }
	    else {
		catch(ob = new(val["#base_name#"]));
	    }
	    if( ob ) {
		ob->eventLoadObject(val, 1);
	    }
	}
    }
    if( recurse ) {
	object prev = previous_object();
	
	if( !eventMove(prev) ) {
	    call_out(function(object p) {
		object env = environment(p);
		
		if( !env ) {
		    p->eventPrint("You lose " + GetShort() + ".");
		}
		else {
		    p->eventPrint("You drop " + GetShort() + ".");
		    eventMove(env);
		}
	    }, 1, prev);
	}
    }
}

static mixed *AddSave(mixed *vars) { return (Saved += vars); }

mixed *GetSave() { return copy(Saved); }

static int SetSaveRecurse(int flag) { return (SaveRecurse = flag); }

string GetSaveString() {
    mixed *tmp;
    string *flat = ({});
    mapping mp = ([]);

    tmp = map(Saved, (: functionp($1) ? evaluate($1, "saving") : $1 :));
    foreach(mixed elem in tmp) {
	if( arrayp(elem) ) flat += elem;
	else flat += ({ elem });
    }
    flat -= ({ 0 });
    foreach(string var in flat) mp[var] = fetch_variable(var);
    mp["#base_name#"] = base_name(this_object());
    if( SaveRecurse )
      mp["#inventory#"] = (string *)all_inventory()->GetSaveString() - ({ 0 });
    return save_variable(mp);
}
