/*    /secure/cmds/player/bug.c
 *    from the Dead Soulsr1 Object Library
 *    the command interface to the NM IV bug tracking system
 *    created by Descartes of Borg 950925
 */

#include <lib.h>
#include <daemons.h>
#include <message_class.h>

inherit LIB_DAEMON;

void PreMenu(string str);
static varargs void MainMenu(string str);
static void Assign(string *args);
static void EndAssign(string *args);
static void Complete(string *args);
void EndComplete(int x);
static void Delete(string *args);
static void Report(string *args);
static void EndReport(string type, string data, string file);
varargs static void View(string *args, int print);
static string GetBugString(int id, mapping bugs);

static void create() {
    daemon::create();
    SetNoClean(1);
}

mixed cmd(string str) {
    string *args;
    int i;

    if( !str || str == "" ) args = ({});
    else args = explode(str, " ");
    if( !(i = sizeof(args)) ) MainMenu();
    else {
	string opt;

	opt = args[0];
	if( i == 1 ) args = ({});
	else args = args[1..];
	switch(opt) {
	    case "-a": Assign(args); break;
            case "-c": Complete(args); break;
            case "-d": Delete(args); break;
            case "-r": Report(args); break;
            case "-p": View(args, 1); break;
            case "-v": View(args); break;
            default: return "To report a bug, use \"bug -r\".";
	}
    }
    return 1;
}

void PreMenu(string str) { 
    if( str == "q" ) {
	message("system", "Exiting the bug tracking system.", this_player());
	return;
    }
    MainMenu(); 
}

varargs static void MainMenu(string str) {
    string tmp;
    int cols;

    if( str && str != "" ) {
	switch(str) {
	    case "a": Assign(({})); return;
            case "c": Complete(({})); return;
	    case "d": Delete(({})); return;
            case "r": Report(({})); return;
            case "p": View(({}), 1); return;
	    case "v": View(({})); return;
	    case "q": 
	        message("system", "Exiting the bug tracking system.", 
			this_player());
		return;
	}
    }
    cols = ((int *)this_player()->GetScreen())[0] || 80;
    tmp = center("Dead Souls Bug Tracking System", cols) + "\n\n";
    if( creatorp(this_player()) ) {
	tmp += "a)ssign bug to creator\n";
	tmp += "c)omplete work on a bug\n";
    }
    if( archp(this_player()) ) tmp += "d)elete a bug from the system\n";
    tmp += "r)eport a new bug to the system\n";
    tmp += "v)iew an existing bug or a list of existing bugs\n";
    tmp += "\nq)uit the bug tracking system\n";
    message("system", tmp, this_player());
    message("prompt", "Enter your choice: ", this_player());
    input_to( (: MainMenu :) );
}

static void Assign(string *args) {
    int i;

    if( !creatorp(this_player()) ) {
	message("system", "Only creators may use the -a option.",
		this_player());
	return;
    }
    if( !(i = sizeof(args)) ) {
	message("prompt", "Enter in the bug ID: ", this_player());
	input_to(function(string str) { Assign(({ str })); });
	return;
    }
    else if( i == 1 ) {
	int x;

	if( (x = to_int(args[0])) > 0 ) { /* assume a bug id for now */
	    if( !archp(this_player()) ) 
	      Assign( ({ args[0], (string)this_player()->GetCapName() }) );
	    else {
		message("prompt", "Enter the creator to assign it to [" +
			(string)this_player()->GetCapName() + "]: ",
			this_player());
		input_to(function(string str, string id) {
		    if( !str || str == "" ) 
		      str = (string)this_player()->GetCapName();
		    Assign( ({ str, id }) );
		}, args[0]);
	    }
	    return;
	}
	else {
	    message("prompt", "Enter in the bug ID to assign to " +
		    capitalize(args[0]) + ": ", this_player());
	    input_to(function(string id, string str) { 
		Assign( ({ id, str }) );
	    }, args[0]);
	    return;
	}
    }
    else if( i == 2 ) {
	message("prompt", "Do you wish to comment? [n]: ", this_player());
	input_to(function(string str, string *args) {
	    if( !str || str == "" ) str = "n";
	    else str = lower_case(str[0..0]);
	    if( str == "y" ) {
		string file;

		message("system", "Enter comments on the bug...", 
			this_player());
		file = DIR_TMP "/" + (string)this_player()->GetKeyName();
		rm(file);
		this_player()->eventEdit(file, (: EndAssign, args :));
		return;
	    }
	    else Assign(args + ({ "" }));
	}, args);
	return;
    }
    else {
	string who, comments;
	int x;

	if( (x = to_int(args[0])) < 1 ) {
	    who = args[0];
	    if( (x = to_int(args[1])) < 1 ) {
		message("system", "Invalid bug ID " + x + ".", this_player());
		message("prompt", "Hit return: ", this_player());
		input_to( (: PreMenu :) );
		return;
	    }
	}
	else who = args[1];
	comments = args[2];
	if( !archp(this_player()) && (convert_name(who) !=
				      (string)this_player()->GetKeyName()) ) {
	    message("system", "Only arches may assign bugs to other people.",
		    this_player());
	    message("prompt", "Hit return: ", this_player());
	    input_to( (: PreMenu :) );
	    return;
	}
	if( !user_exists(convert_name(who)) ) {
	    message("system", "No such creator: " + who, this_player());
	    message("prompt", "Hit return: ", this_player());
	    input_to( (: PreMenu :) );
	    return;
	}
	if( !((int)BUGS_D->eventAssign(x, who)) ) {
	    message("system", "Failed to assign bug.", this_player());
	    return;
	}
	if( comments != "" ) BUGS_D->AddComment(x, comments);
	message("system", "Assigned bug to " + who + ".", this_player());
	return;
    }
}

static void EndAssign(string *args) {
    string file, contents;

    file = DIR_TMP "/" + (string)this_player()->GetKeyName();
    contents = (read_file(file) || "");
    rm(file);
    Assign(args + ({ contents }));
}
    
static void Complete(string *args) {
    string file;
    int x;

    if( !sizeof(args) ) {
	message("prompt", "Enter the bug ID: ", this_player());
	input_to(function(string str) { Complete( ({ str }) ); });
	return;
    }
    else if( !creatorp(this_player()) ) {
	message("system", "Invalid command.", this_player());
	message("prompt", "Hit return: ", this_player());
	input_to( (: PreMenu :) );
	return;
    }	
    else if( (x = to_int(args[0])) < 1 ) {
	message("system", "Invalid bug ID.", this_player());
	message("prompt", "Hit return: ", this_player());
	input_to( (: PreMenu :) );
	return;
    }
    message("system", "Enter in your comments:", this_player());
    file = DIR_TMP "/" + (string)this_player()->GetKeyName();
    if( file_exists(file) ) rm(file);
    this_player()->eventEdit(file, (: EndComplete, x :));
}

void EndComplete(int x) {
    string file, stuff;

    if( previous_object() != this_player(1) ) return;
    file = DIR_TMP "/" + (string)this_player()->GetKeyName();
    if( !(stuff = read_file(file)) ) {
	message("system", "Edit aborted.", this_player());
	rm(file);
	return;
    }
    rm(file);
    if( !((int)BUGS_D->eventComplete(x, stuff)) ) {
	message("system", "Failed to set the bug completed.", this_player());
	return;
    }
    message("system", "Bug marked completed!", this_player());
}

static void Delete(string *args) {
    if( !archp(this_player()) ) {
	message("system","You must be an arch to delete bugs.", this_player());
	return;
    }
    if( !sizeof(args) ) {
	message("prompt", "Delete which bug? ", this_player());
	input_to(function(string str) { Delete( ({ str }) ); });
	return;
    }
    else {
	int x;

	if( (x = to_int(args[0])) < 1 ) 
	  message("system", "Invalid bug ID.", this_player());
	else if( !((int)BUGS_D->eventDelete(x)) ) 
	  message("system", "Delete failed.", this_player());
	else message("system", "Deletion succeeded.", this_player());
	return;
    }
}

static void Report(string *args) {
    if( archp(this_player()) && sizeof(args) ) {
	string data;
	string bug;
	int x;

	data = "Room: " + file_name(environment(this_player()));
	bug = implode(args, " ");
	if( x = (int)BUGS_D->eventReport((string)this_player()->GetCapName(), 
				     "approval", bug, data) ) {
	    BUGS_D->eventAssign(x, query_privs(environment(this_player())));
	    message("system", "Bug reported.", this_player());
	    return;
	}
	else {
	    message("system", "Error in reporting bug.", this_player());
	    return;
	}
    }
    else EndReport(0, "Room: " + file_name(environment(this_player())), 0);
}

static void EndReport(string type, string data, string file) {
    string tmp;
    int x;

    if( !type ) {
	message("system", "Choose a bug type from among the following:\n",
		this_player());
	message("system", "\tidea (some nifty idea to add to the game)",
		this_player());
	message("system", "\ttypo (misspelling, lexigraphical weirdness)",
		this_player());
	message("system", "\tunexplained behaviour (something "
		"contrary to how you would expect it)", this_player());
	message("system", "\truntime (one of those nasty error messages)\n",
		this_player());
	message("system", "\tother\n", this_player());
	message("prompt", "Enter type: ", this_player());
	input_to( (: EndReport :), data, 0);
	return;
    }
    if( !file ) {
	file = DIR_TMP "/" + (string)this_player()->GetKeyName();
	rm(file);
	message("system", "Enter in a description of the bug.  When done, "
		"enter a period on a line by itself.", this_player());
	this_player()->eventEdit(file, (: EndReport, type, data, file :));
	return;
    }
    if( !(tmp = read_file(file)) ) {
	message("system", "Bug report aborted.", this_player());
	rm(file);
	return;
    }
    rm(file);
    if( type == "runtime" ) {
	mapping last_error;

	if( last_error = (mapping)this_player()->GetLastError() )
	  data += "\n" + (string)master()->standard_trace(last_error) + "\n";
    }
    if( !(x = (int)BUGS_D->eventReport((string)this_player()->GetCapName(),
				   type, tmp, data)) ) {
	message("system", "Bug report failed.", this_player());
	return;
    }
    message("system", "Bug reported, thank you!  Your tracking id is " +
	    x + ".", this_player());
}

varargs static void View(string *args, int print) {
    mapping bugs;
    function f;
    string whose;
    int x;

    f = function() { 
	message("prompt", "\nHit return: ", this_player());
	input_to( (: PreMenu :) );
    };
    if( !sizeof(args) ) {
	message("system", "View:\n\t1) all bugs\n"
		"\t2) unassigned bugs only\n"
		"\t3) assigned bugs only\n"
		"\t4) completed bugs only\n", this_player());
	if( creatorp(this_player()) )
	  message("prompt", "Enter a choice [3]: " , this_player());
	else message("prompt", "Enter a choice [1]: ", this_player());
	input_to(function(string str, string it_sucks, int print) {
	    if( !str || str == "" ) {
		if( creatorp(this_player()) ) str = "3";
		else str = "1";
	    }
	    if( str < "1" || str > "4" ) {
		message("system", "Invalid selection", this_player());
		message("prompt", "Hit return: ", this_player());
		input_to( (: PreMenu :) );
		return;
	    }
	    View( ({ str }), print );
	}, "", print);
	return;
    }
    else if( sizeof(args) == 1 && !creatorp(this_player()) ) {
	mapping bug;
	string tmp = "";
	int bug_id;

	bugs = (mapping)BUGS_D->GetBugs();
	foreach( bug_id, bug in bugs ) {
	    if( bug["who"] != (string)this_player()->GetCapName() )
	      continue;
	    if( args[0] == "1" || (args[0] == "2" && !bug["assigned"]) ||
	       (args[0] == "3" && bug["assigned"] && !bug["date fixed"]) ||
	       (args[0] == "4" && bug["date fixed"]) )
	      tmp += GetBugString(bug_id, bugs) + "\n*****\n\n";
	}
	if( tmp == "" ) {
	    message("system", "No bugs meet your query criteria.", 
		    this_player());
	    message("prompt", "Hit return: ", this_player());
	    input_to( (: PreMenu :) );
	    return;
	}
	this_player()->eventPage(explode(tmp, "\n"), MSG_SYSTEM, f);
	return;
    }
    else if( sizeof(args) == 1 ) {
	message("system", "View:\n\t1) all bugs\n"
		"\t2) bugs assigned to me\n"
		"\t3) bugs reported by me\n", this_player());
	message("prompt", "Enter choice [2]: ", this_player());
	input_to(function(string str, string one, int print) {
	    if( !str || str == "" ) str = "2";
	    else if( str < "1" || str > "3" ) {
		message("system", "Invalid selection.", this_player());
		message("prompt", "Hit return: ", this_player());
		input_to( (: PreMenu :) );
		return;
	    }
	    View( ({ one, str }), print );
	}, args[0], print);
	return;
    }
    else {
	mapping bug;
	string nom, tmp = "";
	int bug_id;
	
	nom = (string)this_player()->GetKeyName();
	bugs = (mapping)BUGS_D->GetBugs();
	if( !creatorp(this_player()) ) {
	    View( ({ args[0] }), print );
	    return;
	}
	else foreach(bug_id, bug in bugs) {
	    string opt1, opt2;

	    opt1 = args[0];
	    opt2 = args[1];
	    if( opt1 == "1" && opt2 == "1" ) 
	      tmp += GetBugString(bug_id, bugs) + "\n*****\n\n";
	    else {
		if( opt2 == "2" && (!bug["assigned"] || 
				    convert_name(bug["assigned"]) != nom) ) 
		  continue;
		else if(opt2 == "3" && convert_name(bug["who"]) != nom )
		  continue;
		if( opt1 == "2" && !bug["assigned"] )
		  tmp += GetBugString(bug_id, bugs) + "\n*****\n\n";
		else if( opt1 == "3" && bug["assigned"] && !bug["date fixed"] )
		  tmp += GetBugString(bug_id, bugs) + "\n*****\n\n";
		else if( opt1 == "4" && bug["date fixed"] )
		  tmp += GetBugString(bug_id, bugs) + "\n*****\n\n";
	    }
	}
	if( tmp == "" ) {
	    message("system", "No bugs match your query.", this_player());
	    message("prompt", "Hit return: ", this_player());
	    input_to( (: PreMenu :) );
	    return;
	}
	if( print && creatorp(this_player()) ) {
	    string file;
	    
	    rm(file = user_path((string)this_player()->GetKeyName()) + "bugs");
	    write_file(file, strip_colours(tmp));
	}
	else this_player()->eventPage(explode(tmp, "\n"), MSG_SYSTEM, f);
	return;
    }
}

static string GetBugString(int id, mapping bugs) {
    string tmp;

    tmp = "%^YELLOW%^Bug ID:%^RESET%^ " + id + "\n";
    tmp += "%^YELLOW%^Reported by:%^RESET%^ " + 
      bugs[id]["who"] + "\n";
    if( bugs[id]["assigned"] ) {
	tmp += "%^YELLOW%^Status: %^RESET%^";
	if( !bugs[id]["date fixed"] ) 
	  tmp += "assigned to " + bugs[id]["assigned"] + "\n";
	else tmp += "completed " + ctime(bugs[id]["date fixed"]) + "\n";
    }
    else tmp += "%^YELLOW%^Status:%^RESET%^ unassigned\n";
    tmp += "%^YELLOW%^Type:%^RESET%^ " + bugs[id]["type"] + "\n";
    if( bugs[id]["date fixed"] )
      tmp += "%^YELLOW%^Notes:%^RESET%^\n" + bugs[id]["resolution"] + "\n";
    if( creatorp(this_player()) ) 
      tmp += "\n%^YELLOW%^Creator info:%^RESET%^\n" + bugs[id]["data"] + "\n";
    tmp += "\n%^YELLOW%^Bug info:%^RESET%^\n" + bugs[id]["bug"] + "\n";
    return tmp;
}

string GetHelp(string str) {
    string tmp;

    tmp = "Syntax: <bug>\n";
    if( creatorp(this_player()) ) {
	tmp += "        <bug -a (BUG_ID CREATOR>)\n";
	tmp += "        <bug -c (BUG_ID)>\n";
    }
    if( archp(this_player()) ) tmp += "        <bug -d (BUG_ID)>\n";
    tmp += "        <bug -r>\n        <bug -v ([1-4] [1-3])>\n\n";
    tmp += "The command interface to the Dead Souls Bug Tracking System.  "
      "You can simply type \"bug\" and be prompted for further options, "
      "or, if you understand the system, pass command line arguments "
      "to the bug command to make things go faster.  This system allows "
      "players to report bugs or ideas and periodically see what has "
      "been done about their report.  It also allows creators a way "
      "to track bugs which have been reported to them and give feedback "
      "to the players who have reported them.  It gives admins a way to "
      "track and assign mudlib level bugs.  The options above correspond "
      "to assigning, completing, deleting, reporting, and viewing bugs "
      "respectively.\n\n"
      "See also: praise";
    return tmp;
}
    
