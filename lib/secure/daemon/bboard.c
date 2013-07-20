/*    /secure/daemon/bboard.c
 *    from Nightmare 3.3
 *    bulletin board daemon
 *    created by Descartes of Borg 940920
 */

#include <lib.h>
#include <dirs.h>
#include <privs.h>
#include "bboard.h"

inherit LIB_DAEMON;

private string __Owner;
private mapping *__Posts;
static private string __CurrentID;
string list_new_posts(string id);
string Location;

void create() {
    daemon::create();
    SetNoClean(1);
    __Owner = 0;
    __CurrentID = 0;
    __Posts = ({});
}

static private void save_board() {
    if(!__CurrentID){
        return;
    }
    if(!unguarded((: file_exists,save_file(DIR_BOARDS+"/"+__CurrentID) :))){
        int i;

        if(!sizeof(__Posts)){
            //tc("__Owner: "+__Owner);
            __Owner = query_privs(previous_object(0));
            __Posts = ({});
            //tc("__Owner: "+__Owner);
        }
        i = strlen(__CurrentID);
        while(i--) 
            if((__CurrentID[i] < 'a' || __CurrentID[i] > 'z') && __CurrentID[i] != '_')
                error("Illegal bulletin board id.");
    }
    SaveObject(save_file(DIR_BOARDS+"/"+__CurrentID));
}

static private int restore_board() {
    if(!__CurrentID){
        return 0;
    }
    if(!unguarded((: file_exists, save_file(DIR_BOARDS+"/"+__CurrentID) :))){
        //tc("o: "+__Owner);
        __Owner = query_privs(previous_object(0));
        //tc("o: "+__Owner);
        __Posts = ({});
        return 0;
    }
    else {
        RestoreObject(save_file(DIR_BOARDS+"/"+__CurrentID));
    }
    return 1;
}

static private int valid_access() {
    string str;
    //tc("stack: "+get_stack(1));
    if(this_player() && adminp(this_player())) true();
    else if(__Owner == PRIV_SECURE && !(master()->valid_apply(({}))))
        return 0;
    str = query_privs(previous_object(0));
    if(member_array(PRIV_ASSIST, explode(str, ":")) != -1) return 1;
    return (__Owner == str);
}

void add_post(string id, string who, string subj, string msg) {
    //tc("1");
    if(__CurrentID != id) {
        __CurrentID = id;
        restore_board();
    }
    //tc("2");
    if(!valid_access()){
        //tc("x: "+valid_access());
        return;
    }
    //tc("3");
    if(!stringp(who)) return;
    if(!subj || subj == "") subj = "[No Subject]";
    if(!msg || msg == "") return;
    __Posts += ({ ([ "author" : who, "subject" : subj, "time" : time(),
                "post" : msg, "read" : ({ convert_name(who) }) ]) });
    save_board();
}

void RegisterLocation(string id, string location){
    if(__CurrentID != id) {
        __CurrentID = id;
        restore_board();
    }
    if(!valid_access()){
        return;
    }
    if(!location || !stringp(location)){
        return;
    }
    Location = location;
    save_board();
}

void remove_post(string id, int post) {
    if(__CurrentID != id) {
        __CurrentID = id;
        restore_board();
    }
    if(!valid_access()) return;
    if(post < 0 || post >= sizeof(__Posts)) return;
    if( !post && sizeof(__Posts) ) __Posts = __Posts[1..];
    else if( post == sizeof(__Posts) - 1 ) __Posts = __Posts[0..<2];
    else __Posts = __Posts[0..(post - 1)] + __Posts[(post + 1)..];
    save_board();
}

void mark_read(string id, int post, string reader) {
    if(__CurrentID != id) {
        __CurrentID = id;
        restore_board();
    }
    if(!valid_access()) return;
    if(post < 0 || post > sizeof(__Posts)) return;
    if(reader && reader != "")
        __Posts[post]["read"] = distinct_array(__Posts[post]["read"]+({reader}));
    save_board();
}

mapping query_post(string id, int post) {
    if(__CurrentID != id) {
        __CurrentID = id;
        restore_board();
    }
    if(!valid_access()) return 0;
    if(post < 0 || post > sizeof(__Posts)) return 0;
    return copy(__Posts[post]);
}

mapping *query_posts(string id) { 
    if(__CurrentID != id) {
        __CurrentID = id;
        restore_board();
    }
    if(!valid_access()) return 0;
    return copy(__Posts);
}

int query_number_posts(string id) {
    if(__CurrentID != id) {
        __CurrentID = id;
        restore_board();
    }
    if(!valid_access()) return 0;
    return sizeof(__Posts);
}

varargs string list_new_posts(string id, int location){
    string mag;
    int i;
    mixed count;

    if(__CurrentID != id) {
        __CurrentID = id;
        Location = 0;
        i = restore_board();
    }

    count = 0;

    for(i = 0; i < sizeof(__Posts); i++){
        if(member_array(convert_name(this_player()->GetKeyName()),
                    __Posts[i]["read"]) == -1) count++;
    }

    id = replace_string(id, "_", " ");
    if(location) id += " ( "+Location+" ) ";
    mag = "";
    mag += capitalize(id) + " has "+(count ? count : "no") + " new message"+
        (count == 1 ? "" : "s")+ " posted.";
    return mag;
}
