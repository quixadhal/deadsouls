/*    /secure/daemon/bboard.c
 *    from Dead Souls 3.3
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

void create() {
    daemon::create();
    SetNoClean(1);
    __Owner = 0;
    __CurrentID = 0;
    __Posts = ({});
}

static private void save_board() {
    if(!__CurrentID) return;
    if(!unguarded((: file_exists,DIR_BOARDS+"/"+__CurrentID+__SAVE_EXTENSION__ :))){
        int i;
        
        if(!sizeof(__Posts)) return;
        i = strlen(__CurrentID);
        while(i--) 
          if((__CurrentID[i] < 'a' || __CurrentID[i] > 'z') && __CurrentID[i] != '_')
            error("Illegal bulletin board id.");
    }
    unguarded((: save_object, DIR_BOARDS+"/"+__CurrentID :));
}

static private void restore_board() {
    if(!__CurrentID) return;
    if(!unguarded((: file_exists, DIR_BOARDS+"/"+__CurrentID+__SAVE_EXTENSION__ :))) {
        __Owner = query_privs(previous_object(0));
        __Posts = ({});
    }
    else unguarded((: restore_object, DIR_BOARDS+"/"+__CurrentID :));
}

static private int valid_access() {
    string str;

    if(__Owner == PRIV_SECURE && !((int)master()->valid_access(({}))))
      return 0;
    str = query_privs(previous_object(0));
    if(member_array(PRIV_SECURE, explode(str, ":")) != -1) return 1;
    return (__Owner == str);
}

void add_post(string id, string who, string subj, string msg) {
    if(__CurrentID != id) {
        __CurrentID = id;
        restore_board();
    }
    if(!valid_access()) return;
    if(!stringp(who)) return;
    if(!subj || subj == "") subj = "[No Subject]";
    if(!msg || msg == "") return;
    __Posts += ({ ([ "author" : who, "subject" : subj, "time" : time(),
      "post" : msg, "read" : ({ convert_name(who) }) ]) });
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
