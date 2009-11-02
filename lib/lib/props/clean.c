/*    /lib/clean.c
 *    From the Dead Souls LPC Library
 *    the central object of the entire mudlib
 *    created by Descartes of Borg 940210
 *    Version: @(#) clean.c 1.3@(#)
 *    Last Modified: 96/12/16
 */

#include <clean_up.h>
#include ROOMS_H

inherit LIB_SAVE;

private static int NoClean = 0; 

/* ******************* clean.c attributes ************************* */
int GetNoClean(){
    return NoClean;
}  

static int SetNoClean(int x){
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
static int Destruct(){
    object env, furn; 
    int pers;

    if( !this_object() ){
        return 1;
    }

    env = environment();
    pers = this_object()->GetPersistent();

    foreach(object ob in all_inventory()){
        if( ob ){
            if(env && !pers) ob->eventMove(env);
            else ob->eventMove(ROOM_FURNACE);
        }
    }

    if(this_object()){
        if(living(this_object()) && furn = this_object()->GetProperty("furniture_object"))
            if(objectp(furn)) furn->eventReleaseStand(this_object());
    }

    remove_call_out();
    destruct(this_object()); 
    return !(this_object()); 
}

int eventDestruct(){
    save::eventDestruct();
    return unguarded( (: Destruct() :) );
}

/* ******************* clean.c driver applies ********************* */
int clean_up(int ref_exists){ 
    object array inv; 
    object env;

    if(origin() == "driver") return 0;

    if( NoClean || ref_exists ){
        return NEVER_AGAIN;
    }
    if( !this_object() ){
        return NEVER_AGAIN;
    }
    env = environment();
    if( env ){ 
        if( env->isBag() ){
            return TRY_AGAIN_LATER;
        }
        if( env->GetProperty("storage room") ){
            return TRY_AGAIN_LATER;
        }
    } 
    inv = deep_inventory(this_object());
    if(inv && sizeof(inv)){
        if( sizeof(filter(inv, (: interactive($1) || $1->GetNoClean() :))) ){
            return TRY_AGAIN_LATER;
        }
    }
    if( !env ){ 

        if(this_object() && !strsrch(base_name(this_object()),"/lib/")){
            return NEVER_AGAIN;
        }

        foreach(mixed ob in inv){
            if(ob) ob->eventDestruct();
            if(ob) destruct(ob);
            if(ob) catch(ob->eventMove(ROOM_FURNACE));
        }

        if( this_object() ){
            eventDestruct();
        }
        if( this_object() ){
            destruct(this_object());
        }
        return NEVER_AGAIN; 
    } 
    if( interactive(env) ){
        return TRY_AGAIN_LATER;
    }
    return env->clean_up(); 
} 
