#include <lib.h>
#include <daemons.h>

inherit LIB_DAEMON;

int cmd(string str) {
    if(!member_group(previous_object(), "SECURE")) {
    if(!member_group(previous_object(), "LAW")) return 0;
    }
    if(!str) {
        notify_fail("Correct syntax: <unallow [str]>\n");
        return 0;
    }
    if(member_array(str = lower_case(str), 
      (string *)BANISH_D->query_allowed()) == -1) {
        notify_fail(capitalize(str)+" is not set to be let in.\n");
        return 0;
    }
    BANISH_D->unallow_name(str);
    log_file("watch/players", (string)previous_object()->GetKeyName()+
      " UNLETIN "+str+": "+ctime(time())+"\n");
    return 1;
}

void help() {
    write("Syntax: <unallow [name]>\n\n"
      "Removes a name from the list of names which may be used by\n"
      "players from banned sites to create a new character.\n"
      "\nSee also:\nletin, wholetin, banish, whobanished, unbanish,\n"
      "register, unregister, whoregistered\n"
    );
}
