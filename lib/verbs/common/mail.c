/*    /verbs/common/mail.c
 *    from the Dead Souls V Object Library
 *    mail
 *    mail STR
 *    created by Descartes of Borg 950113
 */

#pragma save_binary

#include <lib.h>
#include <objects.h>
#include "include/mail.h"

inherit LIB_DAEMON;

static void create() {
    daemon::create();
    SetNoClean(1);
    parse_init();
    parse_add_rule("mail", "");
    parse_add_rule("mail", "STR");
}

mixed can_mail() { return can_mail_str(""); }

mixed can_mail_str(string str) {
    if( !str ) return 0;
    else if( str == "" ) str = 0;
    if( !creatorp(this_player()) ) {
	mixed tmp;

	tmp = (mixed)environment(this_player())->CanMail(this_player(), str);
	if( !tmp ) return "Does this place look like a post office?";
	else return tmp;
    }
    return 1;
}

mixed do_mail() { return do_mail_str(0); }

mixed do_mail_str(string str) {
    object ob;
    
    if( !creatorp(this_player()) )
      return (mixed)environment(this_player())->eventMail(this_player(), str);
    if( !(ob = new(OBJ_POST)) ) {
	this_player()->eventPrint("Failed to load postal object!");
	return 1;
    }
    if( !((int)ob->eventMove(this_player())) ) {
	this_player()->eventPrint("You can't seem to carry the postal "
				  "object.");
	return 1;
    }
    ob->start_post(str);
    return 1;
}

string GetHelp(string str) {
    return ("Syntax: <mail>\n"
	    "        <mail PLAYER>\n"
	    "        <mail GROUP>\n\n"
	    "        <mail PLAYER@MUD>\n\n"
	    "Allows you to send mail to another player on this game or "
	    "to a player on another game connected to the Intermud 3 "
	    "system.  Without arguments, you are simply set to read your "
	    "mail.  With arguments, you are creating mail to be sent.  "
	    "You may only read mail in your home town.  The mailer will "
	    "properly route any mail you send to the proper home town "
	    "for the player or players you intend it to go to.\n\n"
	    "See also: mudlist");
}
