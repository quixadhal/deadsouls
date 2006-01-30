#include <lib.h>

mixed *findobs(mixed arg){
    object target;
    object *targets = ({});

    if(objectp(arg)) target = arg;
    if(stringp(arg) ) {
	if(grepp(arg,"/")){
	    if(!file_exists(arg)) arg += ".c";
	    if(!file_exists(arg)) return ({});
	    if(!target = find_object(arg)) return ({});
	}
	else {
	    foreach(object ob in objects()){
		string tmpname;
		string *tmpid;
		if(base_name(ob) == LIB_DOOR || inherits(LIB_DOOR,ob)) tmpid = ({"door"});
		if(base_name(ob) != LIB_DOOR && !inherits(LIB_DOOR,ob)) tmpid = ob->GetId();
		if(!tmpid ) tmpid = ({ generate_tmp() });
		if(! tmpname = ob->GetName() ) tmpname = generate_tmp();
		if(lower_case(tmpname) == lower_case(arg) ||
		  member_array(arg, tmpid) != -1) targets += ({ ob });
	    }
	}
    }
    if(target){
	foreach(object ob in objects()){
	    if(base_name(ob) == base_name(target)) targets += ({ ob });
	}
    }
    return targets;
}
