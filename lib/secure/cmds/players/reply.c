//     /secure/cmds/players/reply.c
//     from the Dead Souls mudlib
//     replies to the person who previously told to you
//     created by Descartes of Borg 06 february 1993

#include <lib.h>
#include <commands.h>
#include <talk_type.h>
#include <daemons.h>

inherit LIB_DAEMON;

int cmd(string str) {
    string reply,a,b,msg,machine_message,frm;
    object ob, machine;
    mixed err;

    reply = this_player()->GetProperty("reply");
    if(!reply) reply = "no one";
    if(!str) {
        notify_fail("Current reply addressee: "+capitalize(reply)+"\n");
        return 0;
    }
    if(reply == "no one") {
        notify_fail("No current reply addressee.\n");
        return 0;
    }
    this_player()->SetProperty("reply_time", time());
    load_object(CMD_TELL)->cmd(reply+" "+str);
    return 1;
}

string GetHelp(){
    return ("Syntax: reply [message]\n\n"
            "With a message, it replies to the person who last told or replied "
            "to you.  Without a message, it shows who the current object of "
            "your reply would be.\nSee also: "
            "finger, idle, muds, say, shout, tell, yell");
}
