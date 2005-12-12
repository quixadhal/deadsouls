#include <lib.h>

void  reap_dummies(){
    //
    // destroys any dummy objects that do not
    // have an environment
    //
    object *objects;
    object *dummies;

    objects = objects();
    dummies = ({});

    foreach(object ob in objects){
	if(base_name(ob) == "/lib/std/dummy"){
	    dummies += ({ ob });
	}
    }

    foreach(object dummy in dummies){
	if(!environment(dummy)) dummy->eventDestruct();
    }
}

void reap_other(string str){
    //
    // destroys any cloned objects with base_name of str that
    // do not have an environment
    //
    string s1,s2;
    object *objects;
    object *others;

    objects = objects();
    others = ({});

    foreach(object ob in objects){
	if(sscanf(file_name(ob),"%s#%s",s1,s2) > 0) {
	    others += ({ ob });
	}
    }

    foreach(object thingy in others){
	if(str == base_name(thingy) && !environment(thingy)) {
	    thingy->eventDestruct();
	}
    }
}

string reap_list(){
    //
    // returns a list of cloned objects without an environment
    //
    string s1,s2,list;
    object *objects;
    object *clones;

    objects = objects();
    clones = ({});
    list = "";

    foreach(object thing in objects){
	if(sscanf(file_name(thing),"%s#%s",s1,s2) > 0) {
	    if(!environment(thing)) clones += ({ thing });
	}
    }

    foreach(object clone in clones){
	if(!clone->GetName()){
	    list += "ERROR "+file_name(clone)+"\n";
	}
	else list += clone->GetName()+" "+file_name(clone)+"\n";
    }

    return list;
}

