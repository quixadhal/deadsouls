#include <daemons.h>
#include <lib.h>
#include <socket.h>
#include <message_class.h>

inherit LIB_DAEMON;

void remote_finger(object me, string target, string mud);

mixed cmd(string str) {
    object ob;
    string wer, wo;

    if(!str) {
	string ret;

	ret = (string)FINGER_D->GetFinger(0);
	if( !ret ) return "General finger appears broken.";
	this_player()->eventPage(explode(ret, "\n"), MSG_SYSTEM);
	return 1;
    }
    else if(sscanf(str, "%s@%s", wer, wo)) 
      remote_finger(this_player(), (wer ? wer : ""), wo);
    else {
	string ret;

	ret = (string)FINGER_D->GetFinger(convert_name(str));
	if( !ret ) return "Finger of "+ capitalize(str) + " failed.";
	this_player()->eventPage(explode(ret, "\n"), MSG_SYSTEM);
    }
    return 1;
}
 
void remote_finger(object ob, string who, string mud) {
    int tc_flag, ud_flag, id;

    if( !(mud = (string)INTERMUD_D->GetMudName(mud)) ) {
	message("system", mud_name() + " is blissfully unaware of the MUD " 
		"you seek.", this_player());
	return;
    }
    SERVICES_D->eventSendFingerRequest(convert_name(who), mud);
    message("system", "Remote finger sent to " + mud + ".", this_player());
}
 
void help() {
    message("help",
      "Syntax: <finger (([player])(@)([mud]))>\n\n"
      "Gives you information about a player named.  If you do not mention "
      "a particular mud, it searches for that player info here.  If you "
      "do not mention a player or a mud, it gives general finger info for "
      "everyone on line here.  If you mention another mud but no player, "
      "it gives you general info on the players on that mud.\n\n"
      "See also: gmuds, mail, muds, rwho, tell, users, who", this_player()
    );
}
