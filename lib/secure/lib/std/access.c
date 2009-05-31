/*    /secure/lib/std/access.c
 *    From the Dead Souls 3 Object Library
 *    An inheritable daemon for giving out access
 *    Created by Descartes of Borg 940918
 *    Version: @(#) access.c 1.3@(#)
 *    Last modified: 97/01/03
 */

#include <lib.h>
#include <function.h>

inherit LIB_DAEMON;

private mapping         ReadAccess    = ([]);
private static function ReadFunction  = 0;
private mapping         WriteAccess   = ([]);
private static function WriteFunction = 0;

/* ****************** access.c attributes ****************** */
/*
 * Note: this uses the old NM 3 naming scheme and is not worth the
 * trouble of changing
 */
static void set_access(string type, function f) {
    if( type == "read" ) {
        ReadFunction = f;
    }
    else {
        WriteFunction = f;
    }
}

mapping query_access(string type) {
    if( type == "read" ) {
        return copy(ReadAccess);
    }
    else {
        return copy(WriteAccess);
    }
}

/* ********************** access.c events ********************** */
nomask int check_access(object ob, string fun, string file, string oper) {
    string array who;
    mapping access;

    if( oper == "read" ) {
        if( functionp(ReadFunction) ) {
            if( !(functionp(ReadFunction) & FP_OWNER_DESTED) ) {
                if( evaluate(ReadFunction, ob, fun, file) ) {
                    return 1;
                }
            }
        }
        access = ReadAccess;
    }
    else {
        if( functionp(WriteFunction) ) {
            if( !(functionp(WriteFunction) & FP_OWNER_DESTED) ) {
                if( evaluate(WriteFunction, ob, fun, file) ) {
                    return 1;
                }
            }
        }
        access = WriteAccess;
    }
    if( !arrayp(who = match_path(access, file)) ) {
        return 0;
    }
    return (member_array(ob->GetKeyName(), who) != -1);
}

nomask int grant_access(string type, string file, string who) {
    if( type == "read" ) {
        if( ReadAccess[file] ) {
            ReadAccess[file] = distinct_array(ReadAccess[file] + ({ who }));
        }
        else {
            ReadAccess[file] = ({ who });
        }
        if( !eventSave() ) {
            Destruct();
            return 0;
        }
        return 1;
    }
    else if( type != "write" ) {
        return 0;
    }
    if( WriteAccess[file] ) {
        WriteAccess[file] = distinct_array(WriteAccess[file] + ({ who }));
    }
    else {
        WriteAccess[file] = ({ who });
    }
    if( !eventSave() ) {
        Destruct();
        return 0;
    }
    return 1;
}

nomask int remove_access(string type, string file, string who) {
    if( type == "read" ) {
        if( !ReadAccess[file] ) {
            return 0;
        }
        else {
            ReadAccess[file] -= ({ who });
        }
        if( !eventSave() ) {
            Destruct();
            return 0;
        }
        return 1;
    }
    else if( type != "write" ) {
        return 0;
    }
    if( !WriteAccess ) {
        return 0;
    }
    else {
        WriteAccess[file] -= ({ who });
    }
    if( !eventSave() ) {
        Destruct();
        return 0;
    }
    return 1;
}

/* ********************* access.c driver applies ******************* */
void create() {
    string file = base_name(this_object());

    if( !strsrch(file, REALMS_DIRS) || !strsrch(file, DOMAINS_DIRS) 
            || !strsrch(file, ESTATES_DIRS) ) {
        SetSaveFile(file);
    }
    daemon::create();
}
