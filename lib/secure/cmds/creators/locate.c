#include <lib.h>
#include <daemons.h>

inherit LIB_DAEMON;

int cmd(string who) {
    object ob;

    if(!who) return notify_fail("Locate whom?\n");
    SERVICES_D->send_locate_q(who);
    message("info", "Locate query sent.", this_player());
    return 1;
}
