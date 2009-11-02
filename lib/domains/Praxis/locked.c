#include <lib.h>

inherit LIB_ROOM;

void init() {
    ::init();
    add_action("any_hook", "", 1);
}

void create() {
    ::create();
    SetNoClean(1);
    SetShort("the locked room");
    SetProperties(([ "no scry" : 1, "light" : 1, "indoors" : 1 ]));
    SetLong(
            "You have been assimilated.\n"
            "From this time on, you will service the Borg.");
    SetExits( 
            (["square" : "/domains/Praxis/square"]) );
}

static int any_hook(string str) {
    message("prompt", sprintf("\n(%s) Password: ", mud_name()),
            this_player());
    return 1;
}
int CanReceive(object ob) {
    if(!ob) ob = previous_object();
    if(!(ob->query_locked())) return 0;
    else return ::CanReceive(ob);
}
