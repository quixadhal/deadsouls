#include <daemons.h>
#include <lib.h>
#include <socket.h>
#include <message_class.h>

inherit LIB_DAEMON;

void remote_finger(object me, string target, string mud);

mixed cmd(string str) {
    string wer, wo;

    if(!str) return "Finger whom?";

    if(!str) {
        string ret;

        ret = FINGER_D->GetFinger(0);
        if( !ret ) return "General finger appears broken.";
        this_player()->eventPage(explode(ret, "\n"), MSG_SYSTEM);
        return 1;
    }
    else if(sscanf(str, "%s@%s", wer, wo))
        remote_finger(this_player(), (wer ? wer : ""), wo);
    else {
        string ret;

        ret = FINGER_D->GetFinger(convert_name(str));
        if( !ret ) return "Finger of "+ capitalize(str) + " failed.";
        this_player()->eventPage(explode(ret, "\n"), MSG_SYSTEM);
    }
    return 1;
}

void remote_finger(object ob, string who, string mud) {
    // If/Else If/Else construct didn't work for some strange reason
    // when I tried to add the IMC2 finger, so i just split it up with a return;
    // Shadyman, 2006-Sept-14

    if ( mud = INTERMUD_D->GetMudName(mud) ) {
        SERVICES_D->eventSendFingerRequest(convert_name(who), mud);
        message("system", "Remote finger sent to " + mud + ".", this_player());
        return;
    }

    if ( mud = IMC2_D->find_mud(mud) ) {
        IMC2_D->finger(who+"@"+mud, ob);
        message("system", "Remote finger sent to " + mud + " on the IMC2 network.", this_player());
        return;
    }

    message("system", mud_name() + " is blissfully unaware of that mud on either the I3 or IMC2 networks.", this_player());
}

string GetHelp(){
    return "Syntax: finger [[player]@[mud]]\n\n"
        "Gives you information about a player named. If you do not mention "
        "a particular mud, it searches for that player info here. "
        " If you mention another mud but no player, "
        "it may give you general info on the players on that mud.\n"
        "See also: mail, rwho, tell, users, who";
}
