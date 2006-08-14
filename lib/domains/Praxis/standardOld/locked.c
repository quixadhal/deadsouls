#include <config.h>

inherit "/std/room";

void init() {
    ::init();
    add_action("any_hook", "", 1);
}

void create() {
    ::create();
    set_no_clean(1);
    set_short("the locked room");
    set_properties(([ "no scry" : 1, "light" : 1, "indoors" : 1 ]));
    set_long(
      "You have been assimilated.\n"
      "From this time on, you will service the Borg.");
    set_exits( 
      (["square" : "/domains/Praxis/square"]) );
}

static int any_hook(string str) {
    message("prompt", sprintf("\n(%s) Password: ", mud_name()),
      this_player());
    return 1;
}
int receive_objects(object ob) {
    if(!ob) ob = previous_object();
    if(!((int)ob->query_locked())) return 0;
    else return ::receive_objects(ob);
}
