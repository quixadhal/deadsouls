/*    /secure/sefun/load_object.c
 *    from the Dead Souls LPC Library
 *    find and object and return it, or load an object and return it
 *    created by Descartes of Borg 940213
 */

object load_object(string str) {
    object ob;
    if(!str) return 0;
    if(!stringp(str)) error("Bad argument 1 to load_object().\n");
    if(ob = find_object(str)) return ob;
    catch(call_other(str, "???"));
    return find_object(str);
}

int update(string file){
    object ob;
    if(!file_exists(file)) file += ".c";
    if(!file_exists(file)) return 1;
    if(ob = find_object(file)) ob->eventDestruct();
    if(find_object(file)) destruct(ob);
    if(find_object(file)) reap_other();
    if(find_object(file)) return 2;
    catch(call_other(file, "???"));
    if(!find_object(file)) load_object(file);
    if(!find_object(file)) return 3;
    return 4;
}

