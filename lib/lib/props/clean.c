/*    /lib/clean.c
 *    From the Dead Souls LPC Library
 *    the central object of the entire mudlib
 *    created by Descartes of Borg 940210
 *    Version: @(#) clean.c 1.3@(#)
 *    Last Modified: 96/12/16
 */

#include <clean_up.h>
 
private static int NoClean = 0; 

/* ******************* clean.c attributes ************************* */
int GetNoClean() {
    return NoClean;
}  

static int SetNoClean(int x) {
    return (NoClean = x);
} 

/* ******************* clean.c events ************************* */
/**
 * Destruct() and eventDestruct() differ in that external objects use
 * eventDestruct() to ask permission for destructing an object.  Destruct()
 * really should be called eventInternalDestruct() or something for
 * naming consistency, but the Destruct() name remains for historical
 * purposes.  To destruct this_object(), call Destruct().  To destruct
 * another object, call ob->eventDestruct().
 */
static int Destruct() {
    object env; 
    object *inv; 
    int i; 

    if( !this_object() ) {
	return 1;
    }
    env = environment();
    if( env ) {
	foreach(object ob in all_inventory()) {
	    if( ob ) {
		ob->eventMove(env);
	    }
	}
    }
    remove_call_out();
    destruct(this_object()); 
    return !(this_object()); 
}
 
int eventDestruct() {
    return Destruct();
}

/* ******************* clean.c driver applies ********************* */
int clean_up(int ref_exists) { 
    object array inv; 
    object env;
 
    if( NoClean || ref_exists ) {
	return NEVER_AGAIN;
    }
    if( !this_object() ) {
	return NEVER_AGAIN;
    }
    env = environment();
    if( env ) { 
        if( env->isBag() ) {
	    return TRY_AGAIN_LATER;
	}
        if( env->GetProperty("storage room") ) {
	    return TRY_AGAIN_LATER;
	}
    } 
    inv = deep_inventory(this_object());
    if( sizeof(filter(inv, (: userp :))) ) {
	return TRY_AGAIN_LATER;
    }
    if( !env ) { 
        catch(inv->eventDestruct());
        if( this_object() ) {
	    Destruct();
	}
        if( this_object() ) {
	    destruct(this_object());
	}
        return NEVER_AGAIN; 
    } 
    if( userp(env) ) {
	return TRY_AGAIN_LATER;
    }
    return env->clean_up(); 
} 
