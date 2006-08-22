// conf_room.c
// A conference room object for holding structured discussions.
// Written by Mobydick, 1-14-92.
// This object is similar to a conference room object
// that was written by Gman@TMI in 1991, which no longer exists at TMI.
// It does not use any of the code from that object although
// the objects are very similar.

// This controls the default directory in which logs are placed. You can
// override this default by calling set_log_dir in the inheriting room.
// It is not secure: you can beat it by using ../.. etc. It does
// save some typing though.

#define DEFAULT_LOG_DIR "/domains/Fooland/data/"

// This controls the default room to echo the proceedings to. You can 
// override this by calling set_echo_room in the inheriting room.

#define DEFAULT_ECHO_ROOM "/domains/Fooland/tower1"

// This is the default room to which users are bounced if they try to enter
// the conference room when it is locked. You can override the default by
// calling set_bounce_room in the inheriting room.
// get teleported to.

#define DEFAULT_BOUNCE_ROOM "/domains/Fooland/tower1"

// The conference room comes with three help files. One gives general help
// on the room features, including the user commands. The second gives the
// moderator-only commands. The third gives the suggested discussion rules;
// there is more detail on those rules in that file.

#define MEETING_HELP "/doc/etc/meeting.help"
#define MODERATE_HELP "/doc/etc/moderate.help"
#define RULES_HELP "/doc/etc/rules.help"

#include <mudlib.h>

inherit ROOM ;

string moderator, speaker ; 	/* Names of the moderator and current speaker */
string echo_room ;   		/* File name of the observation room */
string bounce_room ;		/* File name of the bounce room */
string *silenced ;              /* Array of names of silenced users. */
string *commenters ;            /* Array of names of users granted comments */
string *voters ;		/* Array of names of users who have voted */
string *speakers ;		/* Array of names of speaker's list   */
string *agenda ;		/* Array of agenda item strings.  */
mapping votes ;			/* Keys are names, data are votes cast */
string vote_str ;		/* The proposition being voted on  */
int locked ;			/* 1 if the room is locked, else 0 */
int echoing ;			/* 1 if currently echoing, else 0  */
int logging ;			/* 1 if currently logging, else 0 */
int votelog ;			/* 1 if votes are being logged, else 0 */
string logdir ;			/* Directory to which things are logged */
string votefile ;		/* Name of the file to which votes are logged */
string logfile ;		/* Name of the log file   */
int endtime ;			/* Time at which the speaker/vote ends  */


void create() {
	::create() ;
	set ("light", 1) ;
	set ("short", "The wizard conference room") ;
	set ("long",
"This is the wizard's conference room. It has a number of handy features,\n"+
"such as speaking privilege control, logging, a vote counter, and remote\n"+
"echoing which let the wizards conduct business in an orderly fashion.\n"+
" you can type \"help meeting\" to learn how it all works.\n") ;
	set ("exits", ([ "north" : "/domains/Fooland/tower1" ]) ) ;
	seteuid(getuid(this_object())) ;
	add ("item_func", ([
		"sign" : "read_sign"
	]) ) ;
// The quiet property prevents users from using soul commands in the conference
// room. Otherwise they can spam up the log.
// We allow the use of soul commands by default. We set the quiet property
// to 1 when we give the floor to a speaker and set it back to 0 when the
// floor is opened. The drawback is that the speaker may not use soul
// commands even when he has the floor. Alas.
	set ("quiet", 0) ;
	vote_str = "none" ;
	locked = 0 ;
	votelog = 0 ;
	votefile = "vote.record" ;
	echoing = 1 ;
	commenters = ({ }) ;
	voters = ({ }) ;
	votes = ([ ]) ;
	speakers = ({ }) ;
	agenda = ({ }) ;
	silenced = ({ }) ;
	speaker = "none" ;
	moderator = "none" ;
	logfile = "meeting.log" ;
	logdir = DEFAULT_LOG_DIR ;
	echo_room = DEFAULT_ECHO_ROOM ;
	bounce_room = DEFAULT_BOUNCE_ROOM ;
}

// The init procedure checks the locked status and bounces people out if
// the room is locked. It also does the room command add_actions.

void init() {
	if (locked==1) {
		write ("The conference room is locked. Please ask "+
		   capitalize(moderator)+" for permission to enter.\n\n") ;
		this_player()->move(bounce_room) ;
		return ;
	}
	add_action ("localsay", "say") ;
	add_action ("emote", "emote") ;
	add_action ("echo", "echo") ;
	add_action ("become_moderator", "moderate") ;
	add_action ("set_speaker", "speaker") ;
	add_action ("remove_speaker", "open") ;
	add_action ("lock_room", "lock") ;
	add_action ("vote", "vote") ;
	add_action ("call_for_vote", "call") ;
	add_action ("call_for_roll", "rollcall") ;
	add_action ("unlock_room", "unlock") ;
	add_action ("localupdate", "update") ;
	add_action ("add_commenter", "grant") ;
	add_action ("comment", "comment") ;
	add_action ("request_comment", "request") ;
	add_action ("permit_entry", "permit") ;
	add_action ("toggle_logging", "logging") ;
	add_action ("set_log_file", "logfile") ;
	add_action ("set_vote_file", "votefile") ;
	add_action ("localtime", "time") ;
	add_action ("reset_clock", "reset") ;
	add_action ("read_sign", "read") ;
	add_action ("show_speakers", "speakers") ;
	add_action ("add_speakers", "list") ;
	add_action ("show_agenda", "agenda") ;
	add_action ("add_items", "add") ;
	add_action ("remove_speakers", "unlist") ;
	add_action ("remove_item", "remove") ;
	add_action ("clear_items", "clear") ;
	add_action ("eject_player", "eject") ;
	add_action ("print_status", "logstatus") ;
	add_action ("time_dump", "logtime") ;
	add_action ("silence_player", "silence") ;
	add_action ("unsilence_player", "unsilence") ;
	add_action ("toggle_observe", "observe") ;
	add_action ("help", "help") ;
}

// Log_room_status dumps a fair bit of stuff into the log file without
// echoing it to the room. Useful for periodically recording the status
// of the meeting and the participants, since entrance and exits are not
// logged.
// The moderator can trigger it with the "logstatus" command.

int log_room_status() {

        string file ;
        object *audience ;
        int i ;
        string name ;

	if (!logging) return 1 ;
        file = logdir + logfile ;
        if (!moderator || moderator=="none") {
                write_file (file, "No one is currently moderating.\n") ;
        } else {
                write_file (file, capitalize(moderator)+" is moderating.\n") ;
	}
	if (!speaker || speaker=="none") {
		write_file (file, "The floor is open for discussion.\n") ;
	} else {
		write_file (file, capitalize(speaker)+" has the floor.\n") ;
	}
	if (locked) {
		write_file (file, "The room is locked.\n") ;
	} else {
		write_file (file, "The room is not locked.\n") ;
	}
	write_file (file, "Local host time is "+ctime(time())+".\n") ;
	write_file (file, "The following users are in attendance:\n") ;
	audience = all_inventory (this_object()) ;
	for (i=0;i<sizeof(audience);i++) {
		if (userp(audience[i])) {
			name = getuid(audience[i]) ;
			write_file (file, "   "+capitalize(name)+"\n") ;
		}
	}
	write_file (file, "\n") ;
	return 1 ;
}

// time_dump allows the moderator to make a time stamp in the log.

int time_dump() {

	string file ;

	if (getuid(this_player())!=moderator) {
		notify_fail ("Only the moderator may timestamp the log.\n") ;
		return 0 ;
	}
	file = logdir + logfile ;
	write_file (file, "\nLocal host time is "+ctime(time())+".\n\n") ;
	return 1 ;
}
// Localsay is the add_action that overrides the say command in the
// conference room. This code is called when a player types "say <message>"
// in the conference room. It makes sure the player is permitted to speak
// and if so, invokes the say efun directly.

int localsay (string str) {

	string foo ;

	if (speaker!="none" && getuid(this_player()) != speaker &&
		getuid(this_player()) != moderator) {
		write (capitalize(speaker)+" has the floor now. You may not speak.\n") ;
		return 1 ;
	}
	if ((!str) || (str == " ")) {
		write ("You mutter to yourself.\n") ;
		return 1 ;
	}
	if (!silenced) silenced = ({ }) ;
	if (member_array (getuid(this_player()),silenced)!=-1) {
		write ("You have been silenced and may not speak.\n") ;
		return 1 ;
	}
	foo = wrap((string)this_player()->query("cap_name") + 
	" says: " + str) ;
	say (foo) ;
	if (logging) write_file (logdir+logfile,foo) ;
	if (echoing) tell_room (echo_room,"Conf: "+foo) ;
	write(wrap("You say: " + str));
	return 1;
}

// Emote overrides the emote command in a manner similar to the localsay
// command above. It prevents users from emoting when they do not have
// permission to speak.

int emote (string str) {

    string first,tail,insert,foo;

	if (speaker!="none" && getuid(this_player()) != speaker) {
		write (capitalize(speaker)+" has the floor now. You may not speak.\n") ;
                return 1 ;
        }
	if (member_array (getuid(this_player()),silenced)!=-1) {
                write ("You have been silenced and may not speak.\n") ;
                return 1 ;
        }
    if(!str) {
      write("You look emotional.\n");
      say(this_player()->query("cap_name")+" looks emotional.\n");
      if (logging) write_file (logdir+logfile,this_player()->query("cap_name")+" looks emotional.\n") ;
      return 1;
    }
    if (!wizardp(this_player())) first = "-> ";
    else first = "";
    if (sscanf(str," %s",tail)==1) insert = "";
    else if (sscanf(str,"'%s",tail) == 1) insert = "";
    else insert = " ";
    foo = wrap(first + this_player()->query("cap_name") + insert + str) ;
    write (foo) ;
    say (foo) ;
    if (logging) write_file (logdir+logfile,foo) ;
    if (echoing) tell_room (echo_room , "Conf: "+foo) ;
    return 1;
}

// Echoing is always forbidden. It's just too much of a hassle and can cause
// serious problems with the integrity of the log:
// You echo: "Mobydick says: The speaker is a complete fool!"...
// you get the idea.

int echo (string str) {
	write ("Echoing is forbidden in the conference room at all times.\n") ;
	return 1 ;
}

// Become_moderator lets a person become the moderator if there is none or
// if the current moderator is out of the room. It also lets the current
// moderator resign.

int become_moderator (string str) {
	if (str && str == "off" && getuid(this_player()) == moderator) {
		write ("You stop moderating.\n") ;
		say (capitalize(moderator)+" is no longer moderating.\n") ;
		write_file (logdir+logfile, capitalize(moderator)+" is no longer moderating.\n") ;
		moderator = "none" ;
		return 1 ;
	}
	if (moderator && moderator != "none" && present(moderator, this_object())) {
		write (capitalize(moderator)+" is already serving as moderator.\n") ;
		return 1 ;
	}
	moderator = getuid(this_player()) ;
	write ("You become moderator.\n") ;
	say (capitalize(moderator)+" has become the moderator.\n") ;
	if (logging) write_file (logdir+logfile, capitalize(moderator)+" is now the moderator.\n") ;
	if (echoing) tell_room (echo_room , "Conf: "+capitalize(moderator)+" is now the moderator.\n") ;
	return 1 ;
}

// Localupdate overrides the update command in the conference room. This
// makes updating impossible. This is done because if a user updates the
// conference room, then the logging gets turned off, the file name gets
// reset, the moderator and speaker are lost, all sorts of bad things
// happen. This isn't secure either: one can always leave the room and
// update the file. It's here mostly to prevent this from happening by
// accident or by ignorance than to prevent deliberate malice. You may
// want to alter or remove this.

int localupdate() {
	write ("You may not update objects while in the conference room.\n") ;
	return 1 ;
}

// Set_speaker allows the moderator to give someone the floor.
// You can open the floor by typing "speaker none", or you can type "open
// floor" which is equivalent.

int set_speaker (string str) {

	object ob ;

	if (getuid(this_player())!=moderator) {
		write ("Only the moderator can change the speaker.\n") ;
		return 1 ;
	}
	if (str=="none") {
		speaker="none" ;
		set ("quiet", 0) ;
		write ("You open the floor for discussion.\n") ;
		say ("The floor is open for discussion.\n") ;
		if (logging) write_file (logdir+logfile,"The floor is now open for discussion.\n") ;
		return 1 ;
	}
	ob = present (str, this_object()) ;
	if (!ob || !userp(ob)) {
		write ("There is no such player here.\n") ;
		return 1 ;
	}
	speaker = str ;
	set ("quiet", 1) ;
	write ("You give the floor to "+capitalize(str)+".\n") ;
	say (capitalize(str)+" has the floor.\n") ;
	if (logging) write_file (logdir+logfile, capitalize(str)+" has the floor.\n") ;
	return 1 ;
}

// Remove_speaker is invoked by the "open" add_action. It allows the
// moderator to open the floor for discussion.

int remove_speaker (string str) {
	if (str!="floor") return 0 ;
	if (getuid(this_player())!=moderator) {
		write ("Only the moderator may open the floor for discussion.\n") ;
		return 1 ;
	}
	speaker="none" ;
	set ("quiet", 0) ;
        write ("You open the floor for discussion.\n") ;
        say ("The floor is open for discussion.\n") ;
	if (logging) write_file (logdir+logfile,"The floor is now open for discussion.\n") ;
        return 1 ;
}

// Add_commenter lets the moderator add a named user to the list of
// commenters. It is invoked with the "grant" command. See the help
// documents for more information.

int add_commenter (string str) {

	object ob ;

	if (getuid(this_player())!=moderator) {
		write ("Only the moderator may grant comments.\n") ;
		return 1 ;
	}
	ob = present(str,this_object()) ;
	if (!ob || !userp(ob)) {
		write ("There is no one here by that name.\n") ;
		return 1 ;
	}
	if (!commenters) commenters = ({ }) ;
// You cannot have more than 1 outstanding comment.
	if (member_array(ob,commenters)!=-1) {
		write (capitalize(str)+" already has permission to comment.\n") ;
		return 1 ;
	}
	write ("You grant "+capitalize(str)+" a comment.\n") ;
	tell_object (ob, capitalize(moderator)+" grants you a comment.\n") ;
	if (logging) write_file (logdir+logfile, capitalize(str)+" is granted a comment.\n") ;
	commenters += ({ str }) ;
	return 1 ;
}

// Comment allows a player who has been granted a comment to actually make
// the comment. It writes out the line:
// Mobydick comments: <Message>
// so that you can tell comments from speeches.

int comment (string str) {

	string name, foo ;

	name = getuid(this_player()) ;
	if (!commenters) commenters = ({ }) ;
	if (member_array(name,commenters)==-1) {
		write ("You do not have permission to make a comment.\n") ;
		return 1 ;
	}
	foo = wrap((string)this_player()->query("cap_name") +" comments: " + str) ;
	say (foo) ;
	if (logging) write_file (logdir+logfile, foo) ;
	if (echoing) tell_room (echo_room , "Conf: "+foo) ;
        write(wrap("You say: " + str));
// The user has used up his/her comment.
	commenters -= ({ name }) ;
        return 1;
}

// Request_comment lets a player request a comment from the moderator.
// The moderator can choose to grant it, or not... ;)

int request_comment() {
	if (!moderator || moderator=="none") {
		write ("There is no moderator from whom to request a comment.\n") ;
		return 1 ;
	}
	write ("You ask "+capitalize(moderator)+" for a comment.\n") ;
	tell_object(find_player(moderator), this_player()->query("cap_name")+
		" requests a comment.\n") ;
	return 1 ;
}

// Permit_entry lets the moderator bring someone into the conference room
// when it is locked.

int permit_entry (string name) {

	object user ;
	int oldlock ;

	if (getuid(this_player()) != moderator) {
		write ("Only the moderator may permit entry into a locked conference.\n") ;
		return 1 ;
	}
	user = find_player(name) ;
	if (!user) {
		write ("There is no user by that name.\n") ;
		return 1 ;
	}
	if (present(user,this_object())) {
		write (capitalize(name)+" is already here!\n") ;
		return 1 ;
	}
// We save the old locked status of the room, and restore it when we're
// done. You can "permit" entry into an unlocked conference if you want
// to: it saves the person the trouble of walking.
	oldlock = locked ;
	locked = 0 ;
	user -> move_player (this_object()) ;
	locked = oldlock ;
	write ("You bring "+capitalize(name)+" into the conference.\n") ;
	tell_object (user, capitalize(moderator)+" permits you to enter.\n") ;
	say (capitalize(name)+" has been permitted to enter the conference.\n", user) ;
	return 1 ;
}

// Lock_room lets the moderator lock the room.

int lock_room (string str) {
	if (!str || str!="room") return 0 ;
	if (getuid(this_player())!=moderator) {
		write ("Only the moderator may lock the room.\n") ;
		return 1 ;
	}
	if (locked) {
		write ("The room is already locked.\n") ;
		return 1 ;
	}
	locked = 1 ;
	write ("You lock the room.\n") ;
	say ("The room is now locked.\n") ;
	write_file (logdir + logfile, capitalize(moderator)+" locks the room.\n") ;
	return 1 ;
}

// Unlock_room lets the moderator unlock the room.

int unlock_room (string str) {
	if (!str || str!="room") return 0 ;
	if (getuid(this_player())!=moderator) {
		write ("Only the moderator may lock the room.\n") ;
		return 1 ;
	}
	if (!locked) {
		write ("The room is already unlocked.\n") ;
		return 1 ;
	}
	locked = 0 ;
	write ("You unlock the room.\n") ;
	say ("The room is now unlocked.\n") ;
	if (logging) write_file (logdir+logfile,capitalize(moderator)+" unlocks the room.\n") ;
	return 1 ;
}

// Toggle_logging lets the moderator turn logging on or off.
// When logging is turned on or off, we dump the room status to the log.

int toggle_logging (string str) {
	if (!str || (str!="off" && str!="on") ) {
		notify_fail ("Turn logging on or off.\n") ;
		return 0 ;
	}
	if (getuid(this_player())!=moderator) {
		write ("Only the moderator may toggle the logging.\n") ;
		return 1 ;
	}
	if (str=="off") {
		if (!logging) {
			write ("Logging is not enabled.\n") ;
			return 1 ;
		}
		write ("You disable logging.\n") ;
		say ("Logging has been disabled.\n") ;
		if (logging) write_file (logdir+logfile, "\nLogging is being disabled.\n") ;
		log_room_status() ;
		logging = 0 ;
		return 1 ;
	}
	if (logging) {
		write ("Logging is already enabled.\n") ;
		return 1 ;
	}
	logging = 1 ;
	write ("You enable logging.\n") ;
	say ("Logging has been enabled.\n") ;
	if (logging) write_file (logdir+logfile, "Logging is being enabled.\n") ;
	log_room_status() ;
	return 1 ;
}

// Set_log_file lets the moderator set the name of the log file. The
// log directory (see above) is prepended to the typed file name, so
// you don't have the specify the full path each time.

int set_log_file (string str) {
	if (getuid(this_player())!=moderator) {
		write ("Only the moderator may set the log file.\n") ;
		return 1 ;
	}
	if (!str) {
		write ("Currently logging to "+logdir+logfile+".\n") ;
		return 1 ;
	}
	if (logging) {
		write ("You must turn off logging before changing the log file.\n") ;
		return 1 ;
	}
	logfile = str ;
	write ("The log file is now set to "+logdir+str+".\n") ;
	return 1 ;
}

// Set_vote_file lets the moderator set the vote log file, to which votes
// are recorded. Like the log file, the logdir is prepended.

int set_vote_file (string str) {
	if (getuid(this_player())!=moderator) {
		write ("Only the moderator may set the vote log file.\n") ;
		return 1 ;
	}
	if (!str) {
		write ("Votes are being logged to "+logdir+votefile+"\n") ;
		return 1 ;
	}
	votefile = str ;
	write ("The vote log file is now set to "+logdir+str+".\n") ;
	return 1 ;
}

// Print_status dumps the status of the room and the users present into
// the log file. Can be invoked by the moderator with the "logstatus"
// add_action.

int print_status() {
	if (getuid(this_player())!=moderator) {
		write ("Only the moderator may print the room status to the log.\n") ;
		return 1 ;
	}
	if (!logging) {
		write ("Logging is not turned on.\n") ;
		return 1 ;
	}
	write_file (logdir+logfile, "\n") ;
	log_room_status() ;
	write ("Room status written to the log file.\n") ;
	return 1 ;
}

// Eject_player lets the moderator banish a player from the room and dump
// him in the bounce room. Not very effective if the room is unlocked,
// except perhaps as a warning.

int eject_player (string str) {

	object ob ;

	if (getuid(this_player())!=moderator) {
		write ("Only the moderator may eject players.\n") ;
		return 1 ;
	}
	ob = find_player(str) ;
	if (!ob || !present (ob, this_object())) {
		write ("There is no player named "+capitalize(str)+" here.\n") ;
		return 1 ;
	}
	write ("You eject "+capitalize(str)+" from the room!\n") ;
	tell_object (ob, "You have been ejected from the room.\n") ;
	say (capitalize(str)+" has been ejected from the room.\n") ;
	if (logging) write_file (logdir+logfile, capitalize(moderator)+" has ejected "+capitalize(str)+" from the room.\n") ;
	ob->move_player(bounce_room) ;
	return 1 ;
}

// Localtime shows the user the amount of time left on the room clock.
// The name is chosen to avoid colliding with the time() efun.
// The moderator may also use this function to set the time on the clock
// and start it running. See the help documents for more information on
// how the clock works.

varargs int localtime (string str) {

	int i, min, sec ;
	string foo ;

// If no string, then we just indicate how much time is left on the clock.
	if (!str) {
		if (!endtime || endtime == 0) {
			write ("Time is not running at the moment.\n") ;
			return 1 ;
		}
		i = time() ;
		i = endtime - i ;
		min = (i/60) ;
		sec = i - (min*60) ;
		if (min==1) {
			write ("The clock shows 1 minute and "+sec+" seconds remaining.\n") ;
		} else {
			write ("The clock shows "+min+" minutes and "+sec+" seconds remaining.\n") ;
		}
		return 1 ;
	}
// If there is a string, then the user is trying to set the clock to some
// number of minutes or seconds.
	if (getuid(this_player())!=moderator) {
		write ("Only the moderator may set the clock.\n") ;
		return 1 ;
	}
// You cannot set a new time if the clock is running. This is for safety.
// You must reset the clock first. See below.
	if (endtime!=0) {
                write ("The clock is running. You must reset the clock first.\n"
) ;
                return 1 ;
        }
	if (sscanf(str, "%d min%s", i, foo) == 2) {
		i=i*60 ;
	} else {
		if (sscanf(str, "%d seconds", i) != 1) {
			write ("You must set a number of minutes or seconds: ie, 3 minutes or 90 seconds.\n") ;
			return 1 ;
		}
	}
	write ("You set the clock to "+str+".\n") ;
	say (capitalize(moderator)+" sets the clock to "+str+".\n") ;
	endtime = time()+i ;
// We call_out to a function that prints a message when time runs out.
	call_out ("expire_time", i) ;
	return 1 ;
}

// Reset_clock lets the moderator clear the clock before setting a new time
// on it.

int reset_clock (string str) {
	if (!str || str!="clock") {
		notify_fail ("Reset what?\n") ;
		return 0 ;
	}
	if (getuid(this_player())!=moderator) {
		write ("Only the moderator may reset the clock.\n") ;
		return 1 ;
	}
	if (endtime==0) {
		write ("The clock isn't running.\n") ;
		return 1 ;
	}
	endtime = 0 ;
	write ("You clear the clock.\n") ;
	say (capitalize(moderator)+" clears the clock.\n") ;
// Clear any pending call_outs that may be left behind.
	remove_call_out("expire_time") ;
	remove_call_out("expire_vote") ;
	return 1 ;
}

// Expire_time lets the conference room know that time has expired. It
// doesn't force the speaker to shut up or anything like that: that's left
// to the moderators discretion.

int expire_time() {
	tell_room (this_object(),"The clock runs out.\n") ;
	endtime = 0 ;
}

// Silence_player allows the moderator to silence a user. A silenced user
// cannot speak or emote. He may use comment, if the moderator grants him
// a comment.

int silence_player (string str) {

	object ob ;

	if (getuid(this_player())!=moderator) {
		write ("Only the moderator may silence participants.\n") ;
				return 1 ;
	}
	if (!str) {
		write ("Silence whom?\n") ;
		return 1 ;
	}
	ob = find_player(str) ;
	if (!ob || !present(ob,this_object())) {
		write (capitalize(str)+" is not here.\n") ;
	}
	if (!silenced) silenced = ({ }) ;
	if (member_array(str,silenced)!=-1) {
		write (capitalize(str)+" has already been silenced.\n") ;
		return 1 ;
	}
	write ("You silence "+capitalize(str)+".\n") ;
	tell_object (ob, "You have been silenced!\n") ;
	say (capitalize(str)+" has been silenced!\n") ;
	silenced += ({ str }) ;
	return 1 ;
}

// Unsilence_player lets the moderator restore the speaking/emoting rights
// of a previously silenced player.

int unsilence_player (string str) {

	object ob ;
	if (getuid(this_player())!=moderator) {
		write ("Only the moderator may unsilence participants.\n") ;
				return 1 ;
	}
	if (!str) {
		write ("Silence whom?\n") ;
		return 1 ;
	}
	ob = find_player(str) ;
	if (!ob || !present(ob,this_object())) {
		write (capitalize(str)+" is not here.\n") ;
	}
	if (!silenced) silenced = ({ }) ;
	if (member_array(str,silenced)==-1) {
		write (capitalize(str)+" is not silenced.\n") ;
		return 1 ;
	}
	write ("You unsilence "+capitalize(str)+".\n") ;
	tell_object (ob, "You have been unsilenced.\n") ;
	say (capitalize(str)+" has been unsilenced.\n") ;
	silenced -= ({ str }) ;
	return 1 ;
}

// Toggle_observe lets the moderator toggle echoing to the observation room.
// This is kind of impolite and should only be done if something has to be
// private: in which case why are you doing it in the meeting room?

int toggle_observe (string str) {
	if (!str || (str!="off" && str!="on") ) {
		notify_fail ("Turn observation on or off.\n") ;
		return 0 ;
	}
	if (getuid(this_player())!=moderator) {
		write ("Only the moderator may toggle observation.\n") ;
		return 1 ;
	}
	if (str=="off") {
		if (!echoing) {
			write ("Observation is not enabled.\n") ;
			return 1 ;
		}
		write ("You disable observation.\n") ;
		say ("Observation has been disabled.\n") ;
		if (logging) write_file (logdir+logfile, "\nObservation is being disabled.\n") ;
		echoing = 0 ;
		return 1 ;
	}
	if (echoing) {
		write ("Observation is already enabled.\n") ;
		return 1 ;
	}
	echoing = 1 ;
	write ("You enable observation.\n") ;
	say ("Observation has been enabled.\n") ;
	if (logging) write_file (logdir+logfile, "Observation is being enabled.\n") ;
	return 1 ;
}

// Set_echo_room set the echo_room string. It cannot be invoked by the
// conference room participants. It mostly exists so that, if you have
// several rooms that inherit this one, you can have them echo to different
// rooms. You should not need to call this unless you anticipate having
// several rooms going at once.

int set_echo_room (string str) {
	echo_room = str ;
	return 1 ;
}

// Read_sign lets the user see the status of the meeting room.

varargs int read_sign (string str) {
	if (str && str!="sign") return 0 ;
	if (moderator && moderator!="none") {
		write (capitalize(moderator)+" is the moderator.\n") ;
	} else {
		write ("There is no moderator.\n") ;
	}
	if (speaker && speaker!="none") {
		write (capitalize(speaker)+" is the speaker.\n") ;
	} else {
		write ("The floor is open.\n") ;
	}
	if (logging) {
		write ("Currently logging to "+logdir+logfile+".\n") ;
	} else {
		write ("Not currently logging.\n") ;
	}
	if (echoing) {
		write ("The meeting may be observed from "+echo_room +".\n") ;
	} else {
		write ("The observation room has been closed off.\n") ;
	}
	if (locked) {
		write ("The conference room is locked.\n") ;
	} else {
		write ("The conference room is unlocked.\n") ;
	}
	localtime() ;
	write ("\n"+
"To read the rules of order for this meeting, type \"help rules\".\n"+
"For a guide to the meeting room functions, type \"help meeting\".\n"+
"For help in serving as moderator, type \"help moderate\".\n") ;
	return 1 ;
}

// Show_speakers lets users see the current speakers list.

int show_speakers() {

	int i ;

	if (!speakers || sizeof(speakers)==0) {
		write ("The speakers list is empty.\n") ;
		return 1 ;
	}
	write ("The current speakers list is:\n") ;
	for (i=0;i<sizeof(speakers);i++) {
		write (capitalize(speakers[i]) + "\n") ;
	}
	return 1 ;
}

// Show_agenda lets users see the current agenda.

int show_agenda() {

	int i ;

	if (!agenda || sizeof(agenda)==0) {
		write ("The agenda has not been set.\n") ;
		return 1 ;
	}
	write ("The current agenda is:\n") ;
	for (i=0;i<sizeof(agenda);i++) {
		write ((i+1)+". "+agenda[i]+"\n") ;
	}
	return 1 ;
}

// Add_speakers lets the moderator add a user to the bottom of the speakers
// list.

int add_speakers (string str) {

	object ob ;

	if (getuid(this_player())!=moderator) {
		write ("Only the moderator may add speakers.\n") ;
		return 1 ;
	}
	if (!str) {
		write ("Add who to the speakers list?\n") ;
		return 1 ;
	}
	if (!speakers || member_array(str,speakers)!=-1) {
		write (capitalize(str)+" is already on the speakers list.\n") ;
		return 1 ;
	}
	ob = find_player(str) ;
	if (!ob || !present(ob,this_object())) {
		write (capitalize(str)+" is not present.\n") ;
		return 1 ;
	}
	write ("Adding "+capitalize(str)+" to the speakers list.\n") ;
	tell_object (ob, "You have been added to the speakers list.\n") ;
	speakers += ({ str }) ;
	return 1 ;
}

// Remove_speakers lets the moderator remove someone from the speakers list.
// Generally you should do this when you give someone the floor. It is not
// done automatically because you may wish to give someone the floor without
// taking them off the list - to ask a long question, perhaps.

int remove_speakers (string str) {

	object ob ;

	if (getuid(this_player())!=moderator) {
		write ("Only the moderator may remove speakers.\n") ;
		return 1 ;
	}
	if (!str) {
		write ("Remove who from the speakers list?\n") ;
		return 1 ;
	}
	ob = find_player(str) ;
	if (!speakers || member_array(str,speakers)==-1) {
		write (capitalize(str)+" is not on the speakers list.\n") ;
		return 1 ;
	}
	write ("Removing "+capitalize(str)+" from the speakers list.\n") ;
	tell_object (ob, "You have been removed from the speakers list.\n") ;
	speakers -= ({ str }) ;
	return 1 ;
}

// Clear_speakers erases the speakers list or agenda, depending on argument.

int clear_items (string str) {
	if (getuid(this_player())!=moderator) {
		write ("Only the moderator may clear the speakers list.\n") ;
		return 1 ;
	}
	if (!str || (str!="speakers" && str!="agenda")) {
		write ("Usage: clear [speakers|agenda]\n") ;
		return 1 ;
	}
	if (str=="speakers") {
		speakers = ({ }) ;
		write ("You clear the speakers list.\n") ;
		say ("The speakers list has been cleared.\n") ;
		return 1 ;
	}
	agenda = ({ }) ;
	write ("The agenda has been cleared.\n") ;
	say ("The agenda has been cleared.\n") ;
	return 1 ;
}

// Add_agenda_item lets the moderator add an agenda item. If the argument
// is of the form "after <int> <string>", then string is added to the
// agenda AFTER item int. Agenda items are numbered 1-N rather than 0-(N-1).

int add_items (string str) {

	int post ;
	string prop ;

	if (getuid(this_player())!=moderator) {
		notify_fail ("Only the moderator may add agenda items.\n") ;
		return 0 ;
	}
	if (!str) {
		write ("Usage: add item  or  add after N item\n") ;
		return 1 ;
	}
	if (sscanf(str,"after %d %s", post, prop)!=2) {
		agenda += ({ str }) ;
		write ("Added the following item to the agenda\n"+str+"\n") ;
		return 1 ;
	}
	if (post<0 || post>=sizeof(agenda)) {
		notify_fail ("Item number out of range.\n") ;
		return 0 ;
	}
	if (post==0) agenda = ({ prop }) + agenda ; else
	agenda = agenda[0..post-1] + ({ prop }) + agenda[post..sizeof(agenda)] ;
	write ("Added the following agenda item after item "+post+":\n"+
			prop+"\n") ;
	return 1 ;
}

// Remove_item lets the moderator take an item off the agenda.

int remove_item (string str) {

	int agitem ;

	if (!str) {
		notify_fail ("Usage: remove <number of agenda item>\n") ;
		return 0 ;
	}
	if (sscanf(str,"%d",agitem)!=1) {
		notify_fail ("Usage: remove <number of agenda item>\n") ;
		return 0 ;
	}
	if (agitem<0 || agitem>sizeof(agenda)) {
	write ("Item number out of range.\n") ;
		return 0 ;
	}
// Convert to 0-(N-1) numbering.
	agitem = agitem-1 ;
	write (agitem+"\n") ;
	write ("Removing the following agenda item:\n"+agenda[agitem]+"\n") ;
	if (agitem==0) {
		agenda = agenda[1..sizeof(agenda)-1] ;
	} else {
		if (agitem == sizeof(agenda)-1) {
			agenda = agenda[0..sizeof(agenda)-2] ;
		} else {
			agenda = agenda[0..agitem-1] + agenda[agitem+1..sizeof(agenda)-1] ;
		}
	}
	return 1 ;
}

// Vote lets a player cast a vote on the proposed issue. See the help
// document for details on how voting works.

int vote (string str) {
// With no argument, we print the proposal, if there is one.
	if (!str) {
		if (!vote_str || vote_str=="none") {
			write ("No vote is in progress.\n") ;
			return 1 ;
		}
		write ("Voting on: "+vote_str+"\n") ;
		return 1 ;
	}
// If there is a argument, we interpret that string as a vote cast.
	if (!vote_str || vote_str=="none") {
		write ("No vote is in progress at this time.\n") ;
		return 1 ;
	}
	if (str!="yes" && str!="no" && str!="abstain") {
		write ("Please vote yes, no, or abstain..\n") ;
		return 1 ;
	}
	if (member_array(getuid(this_player()),voters)!=-1) {
		write ("You have already voted!\n") ;
		return 1 ;
	}
	voters += ({ getuid(this_player()) }) ;
	votes[str] = votes[str]+1 ;
	write ("You vote "+str+" on "+vote_str+".\n") ;
// If this is a roll call vote - ie, the votes are being logged - we announce
// the vote to the log file and to the room.
	if (votelog) { 
		write_file (logdir+votefile,capitalize(getuid(this_player()))+" votes "+capitalize(str)+".\n") ;
		say (capitalize(getuid(this_player()))+" votes "+capitalize(str)+".\n") ;
	}
	return 1 ;
}

// Call_for_vote allows the moderator to call for a secret-ballot vote on
// a proposal.

int call_for_vote (string str) {

	string timestr, subjstr ;
	int i ;

	if (getuid(this_player())!=moderator) {
		write ("Only the moderator may call for votes.\n") ;
		return 1 ;
	}
// Must specify an amount of time for which votes may be cast, and a subject
// which people are voting on.
	if (!str) {
		write ("Usage: call <num> <minutes|seconds> <subject>\n") ;
		return 1 ;
	}
	if (sscanf(str,"%d %s %s",i,timestr,subjstr)!=3) {
		write ("Usage: call <num> <minutes|seconds> <subject>\n") ;
		return 1 ;
	}
// This is going to reset the clock: so we want to force the moderator to
// clear the clock first.
	if (endtime!=0) {
		write ("The clock is running. You must reset the clock first.\n") ;
		return 1 ;
	}
	if (timestr=="minutes") {
		i = i * 60 ;
	} else {
		if (timestr!="seconds") {
			write ("Enter the time in minutes or seconds.\n") ;
			return 1 ;
		}
	}
	vote_str = subjstr ;
// This is a secret ballot so we don't want to log the votes.
	votelog = 0 ;
	write ("You call for a vote on "+vote_str+".\n") ;
	say (capitalize(moderator)+" calls for a vote on "+vote_str+".\n") ;
	say (capitalize(moderator)+" sets the clock to "+str+".\n") ;
	endtime = time()+i ;
// Call out to a function which totals the votes at the end of the vote time.
        call_out ("expire_vote",i) ;
        return 1 ;
}

// Call_for_roll is exactly like call_for_vote EXCEPT that it calls for
// a roll-call vote: that is, all votes are announced and logged.

int call_for_roll (string str) {

	string timestr, subjstr ;
	int i ;

	if (getuid(this_player())!=moderator) {
		write ("Only the moderator may call for votes.\n") ;
		return 1 ;
	}
	if (!str) {
		write ("Usage: rollcall <num> <minutes|seconds> <subject>\n") ;
		return 1 ;
	}
	if (sscanf(str,"%d %s %s",i,timestr,subjstr)!=3) {
		write ("Usage: rollcall <num> <minutes|seconds> <subject>\n") ;
		return 1 ;
	}
	if (endtime!=0) {
		write ("The clock is running. You must reset the clock first.\n") ;
		return 1 ;
	}
	if (timestr=="minutes") {
		i = i * 60 ;
	} else {
		if (timestr!="seconds") {
			write ("Enter the time in minutes or seconds.\n") ;
			return 1 ;
		}
	}
	vote_str = subjstr ;
// This is a roll call vote so we log the votes and announce them.
	votelog = 1 ;
	write ("You call for a roll call vote on "+vote_str+".\n") ;
	say (capitalize(moderator)+" calls for a roll call vote on "+vote_str+".\n") ;
	write_file (logdir+votefile,"\n"+wrap(capitalize(moderator)+" calls for a roll call vote on "+vote_str)) ;
	say (capitalize(moderator)+" sets the clock to "+str+".\n") ;
	endtime = time()+i ;
        call_out ("expire_vote",i) ;
        return 1 ;
}

// Expire_vote finishes the vote and tabulates the results. It does not
// announce the outcome because some votes required 2/3 or 3/4 to pass
// rather than a simple majority.

int expire_vote() {
	tell_room (this_object(), "The clock runs out. Voting is over.\n") ;
	endtime = 0 ;
	tell_room (this_object(), "The results of the vote were:\n"+
"Yes:  "+votes["yes"]+"     No:   "+votes["no"]+"       Abstain:  "+
votes["abstain"]+"\n") ;
	if (votelog) {
		write_file (logdir+votefile,"The results of the vote were:\n"+
"Yes:  "+votes["yes"]+"     No:   "+votes["no"]+"       Abstain:  "+votes["abstain"]+"\n\n") ;
	}
	voters = ({ }) ;
	votes["yes"] = 0 ;
	votes["no"] = 0 ;
	votes["abstain"] = 0 ;
	vote_str="none" ;
}

// Help lets people in the room access the various kinds of help.

int help (string str) {
	if (!str) {
		return 0 ;
	}
	switch (str) {
		case "meeting" : {
			this_player()->more(MEETING_HELP) ;
			return 1 ;
		}
		case "rules" : {
			this_player()->more(RULES_HELP) ;
			return 1 ;
		}
		case "moderate" : {
			this_player()->more (MODERATE_HELP) ;
			return 1 ;
		}
		default : {
			return 0 ;
		}
	}
}
