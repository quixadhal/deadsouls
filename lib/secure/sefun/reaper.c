#include <sockets.h>
#include <objects.h>

private string *glist;
private object thingy;

string *ExemptArray = ({ LIB_CONNECT, OBJ_SNOOPER, LIB_DOOR, LIB_ROOM, LIB_SERVER, LIB_FTP_DATA_CONN, LIB_SOCKET, SOCKET_HTTP, SEFUN });

void  reap_dummies(){
    //
    // destroys any dummy objects that do not
    // have an environment
    //
    object *dummies = objects((: $1 && base_name($1) == LIB_DUMMY :))[0..30000];
    dummies += objects((: inherits(LIB_DUMMY, $1) :))[0..30000];

    foreach(object dummy in dummies){
        if(!environment(dummy)) catch( dummy->eventDestruct() );
    }
}

varargs void reap_other(string s1){
    //
    // Destroys any non-special objects and clones that
    // do not have an environment, as well as temporary objects.
    //
    string s2;
    int fail;
    object *garbage;
    object *others;

    if(!this_object()) return;

    garbage = ({});
    others = objects((: ($1 && clonep($1) && !userp($1) &&
                    !inherits(LIB_ROOM, $1) && !environment($1) &&
                    !inherits(LIB_SHADOW, $1) &&
                    member_array(base_name($1), ExemptArray) == -1) ||
                !strsrch(base_name($1), "/open/") :))[0..6300];
#ifdef __FLUFFOS__
    garbage = get_garbage()[0..63000];
#endif

    if(sizeof(others) > 62000 || sizeof(garbage) > 62000){
        call_out("reap_other",5);
    }

    foreach(thingy in others){
        int err;
        if(thingy) err = catch( unguarded( (: destruct(thingy) :) ) );
        if(thingy) fail = 1;
    }

    foreach(thingy in garbage){
        int err;
        if(thingy) err = catch( unguarded( (: destruct(thingy) :) ) );
        if(thingy) fail = 1;
    }
}

mixed reap_list(){
    //
    // returns a list of cloned objects without an environment
    //
    string s1,s2;
    object *clones = objects((: $1 && clonep($1) && !environment($1) :))[0..6300];
    glist = ({});
    filter(clones, (: glist += ({ file_name($1) }) :));
    return glist;
}
