/*    /secure/sefun/load_object.c
 *    from the Dead Souls LPC Library
 *    find and object and return it, or load an object and return it
 *    created by Descartes of Borg 940213
 */

object global_load_ob;

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

    //tc("previous obs: "+identify(previous_object(-1)),"red");
    if(!file_exists(file)) file += ".c";
    if(!file_exists(file)) return 1;
    //tc("file: "+file);
    if(last(file,2) == ".c") file = truncate(file,2);
    //tc("file: "+file);
    if(ob = find_object(file)) {
	global_load_ob = ob;
	//tc("ob = find_object(file) is true. about to dest file.");
	unguarded( (: global_load_ob->eventDestruct() :) );
    }
    if(find_object(file)) {
	//tc("hmmm1. it's still there. let's try to destruct it again.");
	if(find_object(file)) destruct(ob);
    }
    if(find_object(file)) {
	//tc("hmmm2, wtf. ok let's try to reap it");
	if(find_object(file)) reap_other();
    }
    if(find_object(file)) {
	//tc("hmmm3, well i dont get it. we're gonna give up destroying it.");
	return 2;
    }
    //tc("ok so it's destroyed. now let's load it.");
    catch(call_other(file, "???"));
    if(!find_object(file)) {
	//tc("the call_other didn't load it. odd. let's try the load_object sefun");
	load_object(file);
    }
    if(!find_object(file)) {
	//tc("erm....wtf. it's still not loaded. fuck this.");
	return 3;
    }
    //tc("success!!","green");
    return 4;
}

