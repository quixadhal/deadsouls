//      /bin/adm/_unbanish.c
//      from the Dead Souls Mudlib
//      unbanishes a previously banished name
//      created by Shadowwolf@Dead Souls
//      rewritten for new security system by Descartes of Borg 930809

#include <lib.h>
#include <daemons.h>

inherit LIB_DAEMON;

int cmd(string str) {
    if(!archp(previous_object())) return 0;
    if(!str) return 0;
    write(capitalize(str = lower_case(str))+" is no longer banished.\n");
    catch(call_other(BANISH_D, "unbanish_name", str));
    return 1;
}

void help() {
    write("Syntax: <unbanish [name]>\n\n"
      "Removes a name from the list of banished names.\n"
    );
}
