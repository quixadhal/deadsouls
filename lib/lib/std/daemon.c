/*    /lib/daemon.c
 *    From Dead Souls Object Library
 *    Standard inheritable for daemon objects
 *    created by Descartes of Borg 940211
 *    Version: @(#) daemon.c 1.4@(#)
 *    Last modified: 96/12/16
 */

#include <lib.h>
#include <privs.h>

inherit LIB_CLEAN;

static private string SaveFile = 0;

/* ******************** daemon.c attributes ********************** */
string GetSaveFile() {
    return SaveFile;
}

static string SetSaveFile(string str) {
    return (SaveFile = str);
}

/* ********************* daemon.c events ************************* */
int eventDestruct() {
    string str;
   
    if( !master()->valid_apply(({ PRIV_ASSIST })) ) {
	return 0;
    }
    return clean::eventDestruct();
}

varargs int eventRestore(int do_not_zero_out) {
    if( !SaveFile ) {
	return 0;
    }
    if( unguarded((: file_exists(SaveFile + __SAVE_EXTENSION__) :)) ) {
	return unguarded((: restore_object, SaveFile, do_not_zero_out :));
    }
    else {
	return 1;
    }
}

varargs int eventSave(int save_zero_values) {
    if( !SaveFile ) {
	return 0;
    }
    return unguarded((: save_object, SaveFile, save_zero_values :));
}

/* ******************* daemon.c driver applies ********************* */
static void create() {
    eventRestore(1);
}
