/*    /secure/cmds/player/chfn.c
 *    from the Foundation II LPC Library
 *    just when you thought you were safe from lisp
 *    created by Descartes of Borg 950517
 */

#include <lib.h>

inherit LIB_DAEMON;

mixed cmd(string args) {
    function f;
    string rname;

    message("system", "Changing user information for " +
	    (string)previous_object()->GetCapName() + ": ", previous_object());
    rname = (string)previous_object()->GetRealName();
    message("prompt", "Real name [" + rname + "] ", previous_object());
    f = function(string str, string old, object ob) {
	function g;

	if( !str || str == "" ) str = old;
	if( str != old ) {
	    ob->SetRealName(str);
	    message("system", "Real name changed to " + str, ob);
	}
	else message("system", "Real name unchanged.", ob);
	str = (string)ob->GetEmail();
	message("prompt", "Email (preceed with # to make admin only) ["
		+ str + "] ", ob);
	g = function(string str, string old, object ob) {
	    function h;
	    
	    if( !str || str == "" ) str = old;
	    if( str != old ) {
		string a, b;

		if( sscanf(str, "%s@%s", a, b) != 2 ) {
		    message("system", "Invalid email.", ob);
		    return;
		}
		ob->SetEmail(str);
		message("system", "Email changed to " + str, ob);
	    }
	    else message("system", "Email unchanged.", ob);
	    str = (ob->GetWebPage() || "");
	    message("prompt", "Home Page [" + str + "]: ", ob);
	    h = function(string str, object who) {
		    if( str == "" ) {
			str = 0;
		    }
		    else if( str ) {
			if( strlen(str) < 5 ) {
			    str = "http://" + str;
			}
			else {
			    if( str[0..3] != "http" ) {
				str = "http://" + str;
			    }
			}
		    }
		    if( !str ) {
			message("system", "Home paged unchanged.", who);
		    }
		    else {
			who->SetWebPage(str);
		    message("system", "Home page changed.", who);
		    }
	        };
	    input_to(h, 0, ob);
	};
	input_to(g, 0, str, ob);
    };
    input_to(f, "" + rname, previous_object());
    return 1;
}

void help() {
    message("help", "Syntax: <chfn>\n\n"
	    "Allows you to change your real life information.\n\n"
	    "See also: passwd", this_player());
}
