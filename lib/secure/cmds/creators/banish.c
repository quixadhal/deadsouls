//      /bin/dev/_banish.c
//      from the Dead Souls Mudlib
//      prevents a name being used by a new player
//      created by Descartes of Borg ???

#include <lib.h>
#include <daemons.h>

inherit LIB_DAEMON;

int cmd(string str) {
    if(!user_exists(str = lower_case(str))) {
	write(capitalize(str)+" is now banished.\n");
        //seteuid(UID_BANISH);
        catch(call_other(BANISH_D, "banish_name", str));
        //seteuid(getuid());
    }
    else {
	write("A player by that name already exists.\n");
        this_player()->eventPrint((string)FINGER_D->GetFinger(str));
    }
    return 1;
}

void help() {
    write("Syntax: <banish [name]>\n\n"
      "Protects a name from being used by a new player.\n"
      "For example, if you have created a monster named Cassandra, in order\n"
      "to avoid problems with player complaining \"I typed 'kill\n"
      "cassandra' meaning to kill the evil enchantress, but Cassandra\n"
      "walked in and I accidentally killed her.\"  It is also to be\n"
      "used to keep people from using offensive names.\n"
    );
}
