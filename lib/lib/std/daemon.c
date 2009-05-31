/*    /lib/daemon.c
 *    From Dead Souls Mud Library
 *    Standard inheritable for daemon objects
 *    created by Descartes of Borg 940211
 *    Version: @(#) daemon.c 1.4@(#)
 *    Last modified: 96/12/16
 */

#include <lib.h>
#include <privs.h>

inherit LIB_CLEAN;

static private string DaemonSaveFile = 0;

/* ******************** daemon.c attributes ********************** */
string GetSaveFile(){
    return DaemonSaveFile;
}

static string SetSaveFile(string str){
    return (DaemonSaveFile = save_file(str));
}

/* ********************* daemon.c events ************************* */
int eventDestruct(){
    if( !master()->valid_apply(({ PRIV_ASSIST })) ){
        return 0;
    }
    return clean::eventDestruct();
}

varargs int eventRestore(int do_not_zero_out){
    if( !DaemonSaveFile ){
        return 0;
    }
    if( unguarded((: file_exists(DaemonSaveFile) :)) ){
        return RestoreObject(DaemonSaveFile, do_not_zero_out);
    }
    else {
        return 1;
    }
}

varargs int eventSave(int save_zero_values){
    if( !DaemonSaveFile ){
        return 0;
    }
    return SaveObject(DaemonSaveFile, save_zero_values);
}

/* ******************* daemon.c driver applies ********************* */
static void create(){
    eventRestore(1);
    SetNoClean(1);
}
