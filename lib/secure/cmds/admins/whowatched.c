#include <lib.h>
#include <daemons.h>

inherit LIB_DAEMON;

int cmd(string str) {
    string *who;

    if(!str) {
        notify_fail("Check site or name?\n");
        return 0;
    }
    if(!archp(previous_object())) return 0;
    if(str == "name") who = (string *)BANISH_D->query_watched_names();
    else if(str == "site") who = (string *)BANISH_D->query_watched_sites();
    else {
        notify_fail("Whowatched what?\n");
        return 0;
    }
    message("info", sprintf("These %ss are being watched:", str),this_player());
    this_player()->more(explode(format_page(who, 5), "\n"));
    return 1;
}
