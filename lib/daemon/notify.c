/*  /daemon/notify.c
 *  created by Blitz@Dead Souls 960115
 *  notification daemon which displays notices to all cres who login
 */
 
#include <lib.h>
#include <save.h>
#include "include/notify.h"
 
inherit LIB_DAEMON;
 
#define MaxTime  (3600 * 24 * 60)
 
private mixed * Notes;
 
static void create() {
    int x;
    daemon::create();
    SetNoClean(1);
    Notes = ({});
    if( unguarded((: file_size(SAVE_NOTIFY __SAVE_EXTENSION__) :)) > 0 )
      unguarded((: restore_object(SAVE_NOTIFY) :));
    x = sizeof(Notes);
    while( sizeof(Notes) && (time() - Notes[0][Date]) > MaxTime )
      Notes -= ({ Notes[0] });
    if( x != sizeof(Notes) ) eventSaveNotices();
}
 
static int eventSaveNotices() {
    if( !archp(this_player()) ) return 0;
    else return unguarded((: save_object(SAVE_NOTIFY) :));
}
 
int eventAddNotice(object who, string msg) {
    object * obs;
    int x = time();
    if( !who || !sizeof(msg) ) return 0;
    Notes += ({ ({ time(), (string)who->GetName(), msg }) });
    if( sizeof(obs = filter(users() - ({ who }), (: creatorp :))) )
      obs->eventPrint("["+(string)who->GetName()+" added a new notice]");
    if( eventSaveNotices() ) return (sizeof(Notes));
    else return 0;
}
 
int eventRemoveNotice(int x) {
    mixed var = copy(Notes);
    if( x < 0 || x > sizeof(Notes) - 1 ) return 0;
    Notes -= ({ Notes[x] });
    if( !eventSaveNotices() ) return (Notes = var), 0;
    else return 1;
}
 
int eventPrintNotices(object who, int start_time) {
    mixed str = ({ "[ %^YELLOW%^"+mud_name()+" Creator Notices %^RESET%^]", "" });
    int y = sizeof(Notes);
    int x = y - 1;
    if( x < 0 || start_time > Notes[x][Date] ) return 0;
    while( x > 0 && Notes[x - 1][Date] > start_time ) x--;
    do str += ({ sprintf("%sAdded %s by %s [id #%d]:\n\t%s%s",
                 "%^RESET%^", ctime(Notes[x][Date]), Notes[x][Author], x,
                 "%^CYAN%^", Notes[x][Message]), "" });
    while( ++x < y );
    who->eventPage(str);
    return 1;
}
 
int eventWriteNotices(string file, int start_time) {
    string str = "Dead Souls Notices Text Dump.\n\n";
    mixed var;
    int x = sizeof(Notes) - 1;
    if( !archp(this_player()) ) return 0;
    if( x < 0 || start_time > Notes[x][Date] ) return 0;
    while( x > 0 && Notes[x - 1][Date] > start_time ) x--;
    foreach(var in Notes[x..])
      str += sprintf("Added %s by %s\n\t%s\n\n", ctime(var[Date]),
                      var[Author], var[Message]);
    return write_file(file, str);
}
