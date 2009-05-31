#include <lib.h>
#include <function.h>

inherit LIB_DAEMON;

mapping Guards = ([]);
mapping Guarded = ([]);
string *go_verbs = ({ "go", "crawl", "fly", "swim", "enter", "jump", 
        "boost" });

static void create() {
    daemon::create();
    Guards = ([]);
    Guarded = (["rooms" : ([]), "objects" : ([]), 
            "livings" : ([]) ]);
    SetNoClean(1);
}

varargs mixed AddGuard(object guard, mixed what, mixed action){
    string which;
    if(!what || !objectp(guard) || interactive(guard)){
        return 0;
    }
    if(!objectp(what)){
        int err = catch( what = load_object(what) );
        if(err || !what) return -1;
    }

    if(inherits(LIB_ROOM,what)) which = "rooms";
    else if(living(what)) which = "livings";
    else which = "objects";
    if(!Guarded[which]) Guarded[which] = ([]);

    if(Guards[guard] && member_array(what, keys(Guards[guard][which])) != -1){
        if(member_array(what, keys(Guarded[which])) != -1 &&
                member_array(guard, Guarded[which][what]) != -1){ 
            return -2;
        }
    }
    if(!Guards[guard]){
        Guards[guard] = (["rooms" : ([]), "objects" : ([]), "livings" : ([]) ]);
    }
    Guards[guard][which][what] = (action || -1);
    if(!Guarded[which][what]) Guarded[which][what] = ({});
    Guarded[which][what] += ({guard});
    Guarded[which][what] = distinct_array(Guarded[which][what]);
    return 1;
}

mapping GetGuards(){
    return copy(Guards);
}

mapping GetGuardeds(){
    return copy(Guarded);
}

varargs mixed GetGuard(object ob, string which){
    mixed ret;
    if(!which && ob){
        if(inherits(LIB_ROOM,ob)) which = "rooms";
        if(living(ob)) which = "livings";
        else which = "objects";
    }
    if(!Guarded[which] || !(ret = Guarded[which][ob])){
        return 0;
    }
    ret = filter(ret, (: $1 && objectp($1) :) );
    if(sizeof(ret)) return ret;
    else return 0;
}

varargs mixed GetGuarding(object guard, string which){
    mixed ret = ({});
    if(!Guards || !Guards[guard]){
        return 0;
    }
    foreach(mixed key, mixed val in Guards[guard]){
        ret += keys(val);
    }
    ret = filter(ret, (: $1 && objectp($1) :) );
    return ret;
}

int ClearGuards(){
    if(!this_player() || !archp(this_player())) return 0;
    Guards = ([]);
    Guarded = ([ "livings" : ([]), "rooms" : ([]), "objects" : ([]) ]);
    return 1;
}

varargs int CheckMove(object who, mixed dest, mixed dir){
    mixed guards;
    if(!objectp(dest)){
        int err = catch( dest = load_object(dest) );
        if(err || !dest) return 0;
    }
    if(!(guards = Guarded["rooms"][dest])){
        return 1;
    }

    //This was meant to have guards only prevent "go" type movement,
    //but it wound up allowing add_actions that should have been stopped.
    //if(member_array(query_verb(),go_verbs) == -1) return 1;

    //Instead, we will assume that if a direction is specified by
    //eventMoveLiving(), then it's a physical movement the guard
    //should prevent.
    if(!sizeof(dir)) return 1;

    guards = filter(guards, (: $1 && objectp($1) && environment($1) &&
                environment($1) == environment($(who)) :) );
    if(!sizeof(guards)){
        return 1;
    }
    foreach(object guard in guards){
        mixed f;
        int x;
        if(Guards[guard] && Guards[guard]["rooms"] &&
                Guards[guard]["rooms"][dest]){
            f = Guards[guard]["rooms"][dest];
        }
        if(!(guard->AllowPass(who, dest))){
            if((x = functionp(f)) && !(x & FP_OWNER_DESTED)){
                evaluate(f,who);
            }
            else if(stringp(f)){
                who->eventPrint(f);
            }
            return 0;
        }
    }
    return 1;
}

varargs int CheckGet(object who, object what){
    mixed guards;
    if(!(guards = Guarded["objects"][what])){
        return 1;
    }
    guards = filter(guards, (: $1 && objectp($1) && environment($1) &&
                environment($1) == environment($(who)) :) );
    if(!sizeof(guards)) return 1;
    foreach(object guard in guards){
        mixed f;
        int x;
        if(Guards[guard] && Guards[guard]["objects"] &&
                Guards[guard]["objects"][what]){
            f = Guards[guard]["objects"][what];
        }
        if(!(guard->AllowGet(who, what))){
            if((x = functionp(f)) && !(x & FP_OWNER_DESTED)){
                evaluate(f,who);
            }
            else if(stringp(f)){
                who->eventPrint(f);
            }
            return 0;
        }
    }
    return 1;
}
