#include <sockets.h>
#include <objects.h>

private string *glist;
private object thingy;

string *ExemptArray = ({ LIB_CONNECT, OBJ_SNOOPER, LIB_DOOR, LIB_ROOM, LIB_SERVER, LIB_FTP_DATA_CONN, LIB_SOCKET, SOCKET_HTTP });

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
    // destroys any non-special objects and clones that
    // do not have an environment
    //
    string s2;
    int reap_again, fail;
#ifndef __FLUFFOS__
    object *others = objects((: $1 && clonep($1) && !userp($1) &&
        !inherits(LIB_ROOM, $1) && !environment($1) &&
        !inherits(LIB_SHADOW, $1) &&
        member_array(base_name($1), ExemptArray) == -1 :))[0..6300];
#else
    object *others = get_garbage()[0..63000];
#endif
    if(sizeof(others) > 62000){
        reap_again = 1;
    }

    foreach(thingy in others){
        int err;
        //tc("thinggy: "+identify(thingy));
        if(thingy) err = catch( unguarded( (: destruct(thingy) :) ) );
        //else tc("wtf");
        //if(err) tc("err: "+err);
        if(thingy) fail = 1;
    }
    //if(fail) tc("fail! "+sizeof(filter(others,(: $1 :))),"red");
    if(reap_again) call_out("reap_other",5);
}

mixed reap_list(){
    //
    // returns a list of cloned objects without an environment
    //
    string s1,s2;
    object *clones = objects((: $1 && clonep($1) && !environment($1) :))[0..6300];
    glist = ({});
    filter(clones, (: glist += ({ file_name($1) }) :));


    //rm("/tmp/lost_object_list.txt");
    //foreach(object clone in clones){
    //    write_file("/tmp/lost_object_list.txt", file_name(clone)+"\n");
    //}
    //write("Total size of list: "+sizeof(clones)+" lost objects.");
    //return this_player()->eventPage("/tmp/lost_object_list.txt");
    //if(this_player()) this_player()->eventPage( ({ list }) );
    return glist;
}
