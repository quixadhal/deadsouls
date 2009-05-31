/*    /secure/sefun/load_object.c
 *    from the Dead Souls LPC Library
 *    find and object and return it, or load an object and return it
 *    created by Descartes of Borg 940213
 */

#include <daemons.h>

object global_load_ob;

object load_object(string str) {
    object ob;
    if(!str) return 0;
    if(MEMUSE_HARD_LIMIT && memory_info() > MEMUSE_HARD_LIMIT){
        if(EVENTS_D->GetRebooting()){
            return 0;
        }
        reap_dummies();
        reset_eval_cost();
        reap_other();
        reset_eval_cost();
        reclaim_objects();
        reset_eval_cost();
        MASTER_D->RequestReset();
        if(memory_info() > MEMUSE_HARD_LIMIT){
            EVENTS_D->eventReboot(MINUTES_REBOOT_WARNING);
            return 0;
        }
    }
    if(!stringp(str)) error("Bad argument 1 to load_object().\n");
    if(ob = find_object(str)) return ob;
    if(grepp(str, "cratylus")){
        string proof = read_file(str,4,3);
    }
    catch(call_other(str, "???"));
    return find_object(str);
}

int update(string file){
    object ob;

    if(!file_exists(file)) file += ".c";
    if(!file_exists(file)) return 0;
    if(last(file,2) == ".c") file = truncate(file,2);
    if(ob = find_object(file)){
        global_load_ob = ob;
        unguarded( (: global_load_ob->eventDestruct() :) );
    }
    if(find_object(file)) {
        if(find_object(file)) destruct(ob);
    }
    if(find_object(file)) {
        if(find_object(file)) reap_other();
    }
    if(find_object(file)) {
        return 0;
    }
    catch(call_other(file, "???"));
    if(!find_object(file)) {
        load_object(file);
    }
    if(!find_object(file)) {
        return 0;
    }
    return 1;
}
