/*    /lib/user/autosave.c
 *    From the Dead Souls Object Library
 *    Object to save a player to file
 *    Created by Descartes of Borg 940216
 *    Version: @(#) autosave.c 1.3@(#)
 *    Last modified: 97/01/03
 */

#include <message_class.h>
#include <config.h>
#include <dirs.h>
#include <privs.h>

private mixed array Inventory = 0;
private static int  LastSave  = time();

// abstract methods
string GetKeyName();
varargs void eventPrint(string str, mixed args...);
// end abstract methods

nomask void restore_inventory();
nomask void save_player(string nom);

int Setup() {
    object ob;
    int i;

    master()->create_save();
    call_out( (: save_player, GetKeyName() :), 2 );
    restore_inventory();
    return 1;
}

nomask void restore_inventory() {
    if( Inventory ) {
        foreach(string obdata in Inventory) {
	    object ob;
	    mixed tmp;
	    
            tmp = restore_variable(obdata);
	    if( arrayp(tmp) ) {
		ob = new(tmp[0]);
		if( ob ) ob->eventConvertObject(tmp, 1);
	    }
	    else {
		if( !catch(ob = new(tmp["#base_name#"])) ) {
		    if( ob ) {
			ob->eventLoadObject(tmp, 1);
		    }
		}
	    }
        }
    }
    Inventory = 0;
}

nomask int restore_player(string nom) {
    string file;

    if( previous_object(0) != master() ) {
	return 0;
    }
    file = save_file(nom);
    return unguarded((: restore_object, file, 1 :));
}

nomask void save_player(string nom) {
    string file;

    if( !nom || nom != GetKeyName() ) {
	return;
    }
    Inventory = filter(map(all_inventory(), (: $1->GetSaveString() :)),
		       (: $1 :));
    file = save_file(GetKeyName());
    unguarded((: save_object, file, 1 :));
    Inventory = 0;
}

static void heart_beat() {
    int x = time();

    if( x - LastSave < AUTOSAVE_TIME ) {
	return;
    }
    LastSave = x;
    save_player(GetKeyName());
    if( !creatorp(this_object()) ) {
	eventPrint("Autosaving...", MSG_SYSTEM);
    }
}

