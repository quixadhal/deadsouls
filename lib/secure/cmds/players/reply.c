//     /bin/user/_reply.c
//     from the Nightmare mudlib
//     replies to the person who previously told to you
//     created by Descartes of Borg 06 february 1993

#include <lib.h>
#include <talk_type.h>
#include <daemons.h>

inherit LIB_DAEMON;

int cmd(string str) {
    string reply,a,b,msg,machine_message,frm;
    object ob, machine;
    mixed err;

    reply = (string)this_player()->GetProperty("reply");
    if(!reply) reply = "no one";
    if(!str) {
	notify_fail("Current reply addressee: "+capitalize(reply)+"\n");
	return 0;
    }
    if(reply == "no one") {
	notify_fail("No current reply addressee.\n");
	return 0;
    }
    load_object("/secure/cmds/players/tell")->cmd(reply+" "+str);
    return 1;
}

void help() {
    message("help",
      "Syntax: <reply [(message)]>\n\n"
      "With a message, it replies to the person who last told or replied "
      "to you.  Without a message, it shows who the current object of "
      "your reply would be.\n\nSee also: "
      "finger, idle, muds, say, shout, tell, yell", this_player()
    );
}
