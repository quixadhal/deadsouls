/*    /lib/post_office.c
 *    from the Dead Souls Object Library
 *    an inheritable post office
 *    created by Descartes of Borg 951007
 */


#include <lib.h>
#include <objects.h>
#include <post.h>
#include "include/post_office.h"

inherit LIB_ROOM;

static void create(){
    room::create();
}

mixed CanMail(object who, string args){
    if( !interactive(who) ) return 0;
    if( GetTown() != who->GetTown() )
        return "Any mail you might have will be at your home post office.";
    return 1;
}

mixed eventMail(object who, string args){
    object ob;

    if( !(ob = new(OBJ_POST)) ){
        who->eventPrint("Failed to load postal object!");
        return 1;
    }
    if( !(ob->eventMove(who)) ){
        who->eventPrint("You can't seem to carry the postal object.");
        return 1;
    }
    ob->start_post(args);
    return 1;
}

int CanReceive(object ob){
    if( !ob && !(ob = previous_object()) ) return 0;
    if( living(ob) && !interactive(ob) ) return 0;
    else return room::CanReceive(ob);
}

int eventReleaseObject(object foo){
    object ob;

    if( !(ob = previous_object()) ) return room::eventReleaseObject();
    if( !room::eventReleaseObject() ) return 0;
    if( (ob = present(POSTAL_ID, ob)) ) ob->eventDestruct();
    return 1;
}
