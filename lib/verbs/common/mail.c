/*    /verbs/common/mail.c
 *    from the Dead Souls Mud Library
 *    mail
 *    mail STR
 *    created by Descartes of Borg 950113
 */



#include <lib.h>
#include <objects.h>
#include "include/mail.h"

inherit LIB_VERB;

static void create() {
    verb::create();
    SetVerb("mail");
    SetRules("","STR");
}

mixed can_mail() { return can_mail_str(""); }

mixed can_mail_str(string str) {
    if( !str ) return 0;
    else if( str == "" ) str = 0;
    if( !creatorp(this_player()) ) {
        mixed tmp;

        tmp = environment(this_player())->CanMail(this_player(), str);
        if( !tmp ) return "Does this place look like a post office?";
        else return tmp;
    }
    return 1;
}

mixed do_mail() { return do_mail_str(0); }

mixed do_mail_str(string str) {
    object ob;

    if( !creatorp(this_player()) )
        return environment(this_player())->eventMail(this_player(), str);
    if( !(ob = new(OBJ_POST)) ) {
        this_player()->eventPrint("Failed to load postal object!");
        return 1;
    }
    if( !(ob->eventMove(this_player())) ) {
        this_player()->eventPrint("You can't seem to carry the postal "
                "object.");
        return 1;
    }
    this_player()->eventPrint("%^RED%^Remember!%^RESET%^ To end a post, enter a single period on an otherwise blank line, and then hit return.");
    ob->start_post(str);
    return 1;
}

string GetHelp(string str) {
    return ("Syntax: mail\n"
            "        mail <PLAYER>\n"
            "        mail <GROUP>\n"
            "        mail <PLAYER@MUD>\n\n"
            "Allows you to send mail to another player on this game. "
            "Without arguments, you are simply set to read your "
            "mail.  With arguments, you are creating mail to be sent.  "
            "You may only read mail in your home town.  The mailer will "
            "properly route any mail you send to the proper home town "
            "for the player or players you intend it to go to.\n"
            "See also: mudlist");
}
