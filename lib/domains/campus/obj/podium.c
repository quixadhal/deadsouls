/* It works now, no define needed.
 * make sure someone does "setmc" or it won't do much.
 * setmc can only be done once, first come, first get i guess ;)
 * by Boy@frontiers.
 * Major corrections and modifications by Crat 25jul05
 */
#include <lib.h>
#include <vendor_types.h>
inherit LIB_STORAGE;

#define DEFAULT_BOUNCE_ROOM "/domains/town/room/adv_guild"
string bounce_room ;		/* File name of the bounce room */
string *voters ;		/* Array of names of users who have voted */
string *agenda ;		/* Array of agenda item strings.  */
mapping votes ;			/* Keys are names, data are votes cast */
string vote_str ;		/* The proposition being voted on  */
int endtime ;			/* Time at which the speaker/vote ends  */
int votelog ;			/* 1 if votes are being announced, else 0 */

string speaker;
string mc;
int x;
object ob;
static void create() {
    storage::create();
    SetShort("a podium");
    SetLong("This is the speaker's podium. It is about four feet tall "+
            "and made of some expensive-looking, deep grain wood. This podium "+
            "facilitates the running of meetings by giving the speaker the power "+
            "to recognize individual people to speak at a time. There is "+
            "some space in the podium to store things in, perhaps there is something "+
            "in there now. For more "+
            "details on running a meeting with the podium, type: help podium.");
    SetMass(10);
    SetId( ({"podium", "speaker's podium", "podium.c"}) );
    SetDollarCost(-110);
    SetKeyName("podium");
    SetId(({"podium","handler"}));
    SetAdjectives(({"wood","wooden","meeting","speaker's","Speaker's"}));
    SetDamagePoints(1000);
    SetPreventGet("You can't get that.");
    SetMaxCarry(20);

    SetInventory(([
                ]));

    mc = "";
    x = 0;
    speaker = "";

    vote_str = "none" ;
    voters = ({ }) ;
    votes = ([ ]) ;
    agenda = ({ }) ;
    bounce_room = DEFAULT_BOUNCE_ROOM ;

}
void init() {
    ::init();
    add_action("eventSay", "say",1);
    add_action("eventSay", "codesay");
    add_action("eventRaise", "raise");
    add_action("eventCallOn", "recognize");
    add_action("shaddap", "yell");
    add_action("shaddap", "shout");
    add_action("shaddap", "emote");
    add_action("help", "help");
    add_action("quiet", "quiet");
    add_action("changemc", "changemc");
    add_action("SetMc", "setmc");
    add_action ("echo", "echo") ;
    add_action ("vote", "vote") ;
    add_action ("call_for_vote", "call") ;
    add_action ("call_for_roll", "rollcall") ;
    add_action ("permit_entry", "permit") ;
    add_action ("localtime", "time") ;
    add_action ("reset_clock", "reset") ;
    add_action ("show_agenda", "agenda") ;
    add_action ("add_items", "add") ;
    add_action ("remove_item", "remove") ;
    add_action ("clear_items", "clear") ;
    add_action ("eject_player", "eject") ;
    add_action ("privacy", "privacy");
    add_action ("privacy", "priv");
    add_action ("localupdate", "update") ;
    add_action ("localupdate", "reload") ;
    add_action ("step_down", "step" );
    add_action ("RestrictedAction", "zap" );
    add_action ("RestrictedAction", "force" );
}
mixed CanGet(object ob) { return "The podium does not budge.";}

int RestrictedAction(){
    if(!archp(this_player()) && sizeof(mc) && mc != this_player()->GetKeyName()){
        write("That action is restricted here.");
        return 1;
    }
}

int eventSay(string args) {
    string foo;

    if(!sizeof(args)) return 0;
    args = replace_string(args,"\n","");
    if (mc == this_player()->GetKeyName()) {
        //this_player()->eventPrint("You say %^CYAN%^\"" + capitalize(args) + "\"");
        //say(this_player()->GetName() + " says %^CYAN%^\"" + capitalize(args) + "\"");
        return 0;
    }
    if ( mc != "" && this_player()-> GetKeyName() != speaker ) {
        this_player()->eventPrint("%^RED%^It is not polite to talk out of order.");
        this_player()->eventPrint("Raise your hand if you'd like to speak.");
        return 1;
    }
    if ((!args) || (args == " ")) {
        write ("You mutter to yourself.\n") ;
        return 1 ;
    }
    //foo = this_player()->GetCapName() + 
    //" says: %^CYAN%^\"" + capitalize(args)+"\"";
    //say (foo) ;
    //write("You say: %^CYAN%^\"" + capitalize(args)+"\"");
    return 0;

}


int eventCallOn(string args) {
    if (present((object)args)) {
        if (mc == this_player()->GetKeyName()) {
            speaker = args;
            write("You have called on " + speaker+".\n");
            find_living(args)->eventPrint("%^CYAN%^" + capitalize(mc) + " has called on you, you may speak.");
            return 1;
        }
        else {
            this_player()->eventPrint("Only the speaker can do that.");
            return 1;
        }
    }
    else {
        write("%^CYAN%^This person is not here to be called on.");
        return 1;
    }
}	


int eventRaise() {
    string dude;
    dude=this_player()->GetKeyName();
    if(dude != mc && dude != speaker) {
        tell_room(environment(this_player()),
                this_player()->GetName()+" raises "+
                possessive(this_player())+
                " hand.", ({this_player()}) );
        this_player()->eventPrint("%^CYAN%^You raise your hand.");
        return 1;
    }
    else {
        write("You can speak already. Say what's on your mind.");
        return 1;
    }
}


int shaddap() {
    if(mc != "" && this_player()->GetKeyName() != speaker){
        write("%^RED%^It would be impolite to do that at this time.");
        return 1;
    }
}


int help(string args) {
    if (args != "podium") {
        return 0;
    }
    else {
        write("%^GREEN%^This is the speakers podium, it is where the "
                "speaker stands during a speech.  This podium "
                "has special properties, it can prevent others "
                "from speaking out of turn, if you are the "+
                "speaker.%^RESET%^");
        if (this_player()->GetKeyName() == mc) {
            write("%^RED%^Available commands:");
            write("%^YELLOW%^recognize %^RESET%^: Calls on another to speak.");		
            write("%^YELLOW%^say %^RESET%^: As the speaker, you can say things whenever you like.");
            write("%^YELLOW%^quiet %^RESET%^: Revokes the speaking privilege to the person you last called on.");
            write("%^YELLOW%^changemc %^RESET%^: Removes yourself as mc, and let someone else take over.");
            write("%^YELLOW%^add <string>%^RESET%^: Add agenda item <string> to the bottom of the agenda.");
            write("%^YELLOW%^remove <int>%^RESET%^: Remove agenda item <int> from the agenda.");
            write("%^YELLOW%^clear agenda%^RESET%^: Clear the agenda.");
            write("%^YELLOW%^permit <name>%^RESET%^: Permit player <name> to enter the meeting room when locked.");
            write("%^YELLOW%^eject <name>%^RESET%^: Eject player <name> from the meeting room.");
            write("%^YELLOW%^time <num> [minutes/seconds]%^RESET%^: Set the clock to <num> minutes or seconds.");
            write("%^YELLOW%^reset clock%^RESET%^: Clear the clock.");
            write("%^YELLOW%^step down%^RESET%^: Step down as head speaker.");
            write("%^YELLOW%^rollcall <num> [minutes/seconds] <subject>%^RESET%^: Call for a roll call vote,\n"
                    "\tlasting num minutes or seconds, on <subject>.");
            write("%^YELLOW%^add after <int> <string>%^RESET%^: Add agenda item <string> after agenda item #<int>.\n"
                    "\tadd after 0 <string> adds to the top of the list.");
            write("%^YELLOW%^call <num> [minutes/seconds] <subject>%^RESET%^: Call for a vote, lasting num minutes\n"
                    "\tor seconds, on <subject>.");
            return 1;
        }
        else {
            write("%^RED%^Available commands:");
            write("%^YELLOW%^raise %^RESET%^: Raise your hand, to motion to the speaker that you would like to speak.");
            write("%^YELLOW%^say %^RESET%^ : Say something, you only may do this if the speaker has called on you.");
            if (x == 0) {
                write("%^YELLOW%^setmc%^RESET%^ : There is no mc currently, use this command to set one.");
            }
            return 1;
        }
    }
}


int quiet() {
    if (mc == this_player()->GetKeyName()) {
        write("%^CYAN%^You thank " + capitalize(speaker) + " for speaking.");
        find_living(speaker)->eventPrint("%^CYAN%^" + capitalize(mc) + " thanks you for you speaking.");
        speaker = mc;
        return 1;
    }
    write ("Only the mc may use this command.\n") ;
    return 1;
}


int changemc(string args) {
    if (args != 0) {
        if ( this_player()->GetKeyName() != mc ) {
            write("%^RED%^You are not the mc to begin with, you cannot give that position away.");
            return 1;
        }
        else {
            if (!present(args)) {
                write("%^CYAN%^" + args + " is not present, and therefore cannot be mc.");
                return 1;
            }
            else {
                write("%^CYAN%^You hand the podium over to " + args);
                mc = args;
                say("%^CYAN%^" + capitalize(args) + " is the new head speaker.");
                find_living(mc)->eventPrint("%^BLUE%^You are the new head speaker!");
                find_living(mc)->eventPrint("The command \"help podium\" can help you, if you don't know what to do.");
                return 1;
            }
        }
    }
    else {
        write("%^CYAN%^Syntax:");
        write("changemc <player>");
        return 1;
    }
}

int step_down(string args){
    if (args == "down") {
        if ( this_player()->GetKeyName() != mc ) {
            write("%^RED%^You are not the mc to begin with, you cannot give that position away.");
            return 1;
        }
        say("%^CYAN%^" + capitalize(mc) + " has stepped down as the head speaker.");
        write ( "You step down as the head speaker" );
        mc = "";
        x--;
        load_object(base_name(environment(this_object())))->AutoDeactivate();    
        return 1;
    }
    else {
        write("%^CYAN%^Syntax:");
        write("<step down>");
        return 1;
    }

}

int SetMc(string args) {
    object ob;
    if(args) ob = find_living(args);
    if(!args || !ob) ob = this_player();
    if(!member_group(ob, "MODERATORS")){
        write("That person is not a member of the moderators group.");
        write("An admin should use the admintool command to add the "+
                "appropriate people to that user group.");
        return 1;
    }
    if (x==0) {
        if (args != 0) {
            if (present(args) ) {
                mc = args;
                say("%^CYAN%^" + capitalize(mc) + " is the speaker.");	
                write("You set " + capitalize(args) + " as the speaker.");
                find_living(mc)->eventPrint("The command \"help podium\" can help you, if you don't know what to do.");
                x++;
                return 1;
            }
            else {
                write("You cannot set him/her as the speaker, he/she is not here!");
                return 1;
            }
        }
        else {
            write("%^CYAN%^Syntax:");
            write("setmc <player>");
            return 1;
        }
    }
    else {
        write("There is already a speaker, you cannot set another one.");
        return 1;
    }
}

int privacy(string str){

    if ( mc != this_player()->GetKeyName() ) {
        write ("Only the mc may use the shield.\n") ;
        return 1 ;
    }
    if(str=="on" || str == "1"){
        load_object(base_name(environment(this_object())))->set_privacy( 1 );
        write("You enable the privacy shield.\n");
        say(this_player()->GetName()+" enables a privacy force field around the room.");
        return 1;
    }
    if(str=="off" || str == "0"){
        load_object(base_name(environment(this_object())))->AutoDeactivate();
        write("You disable the privacy shield.\n");
        say(this_player()->GetName()+" disables a privacy force field around the room.");
        return 1;
    }
}

// Echoing is always forbidden. It's just too much of a hassle.
int echo (string str) {
    write ("Echoing is forbidden in the conference room at all times.\n") ;
    return 1 ;
}

// Permit_entry lets the mc bring someone into the conference room
// when it is locked.
int permit_entry (string name) {

    object user ;
    int oldlock ;

    if ( mc != this_player()->GetKeyName() ) {
        write ("Only the mc may permit entry into a locked conference.\n") ;
        return 1 ;
    }
    user = find_player(name) ;
    if (!user) {
        write ("There is no user by that name.\n") ;
        return 1 ;
    }
    if (present(user,environment(this_object()))) {
        write (capitalize(name)+" is already here!\n") ;
        return 1 ;
    }
    // We save the old locked status of the room, and restore it when we're
    // done. You can "permit" entry into an unlocked conference if you want
    // to: it saves the person the trouble of walking.
    oldlock = load_object(base_name(environment(this_object())))->get_privacy() ;
    load_object(base_name(environment(this_object())))->set_privacy( 0 );
    user -> eventMove(environment(this_object())) ;
    load_object(base_name(environment(this_object())))->set_privacy( oldlock );
    write ("You bring "+capitalize(name)+" into the conference.\n") ;
    tell_object (user, capitalize(mc)+" permits you to enter.\n") ;
    say (capitalize(name)+" has been permitted to enter the conference.\n", user) ;
    return 1 ;
}



// Eject_player lets the mc banish a player from the room and dump
// him in the bounce room.
int eject_player (string str) {
    object env = environment(this_player());
    object ob ;

    if ( mc != this_player()->GetKeyName() ) {
        write ("Only the mc may eject players.\n") ;
        return 1 ;
    }
    ob = find_player(str) ;
    if (!ob || !present (ob, environment(this_object()))) {
        write ("There is no player named "+capitalize(str)+" here.\n") ;
        return 1 ;
    }
    write ("You eject "+capitalize(str)+" from the room!\n") ;
    tell_object (ob, "You have been ejected from the room.\n") ;
    say (capitalize(str)+" has been ejected from the room.\n") ;
    ob->eventMove(bounce_room) ;
    if(env) env->AddEjected(ob);
    return 1 ;
}

// Localtime shows the user the amount of time left on the room clock.
// The name is chosen to avoid colliding with the time() efun.
// The mc may also use this function to set the time on the clock
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
    if ( mc != this_player()->GetKeyName() ) {
        write ("Only the mc may set the clock.\n") ;
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
    say (capitalize(mc)+" sets the clock to "+str+".\n") ;
    endtime = time()+i ;
    // We call_out to a function that prints a message when time runs out.
    call_out ("expire_time", i) ;
    return 1 ;
}

// Reset_clock lets the mc clear the clock before setting a new time
// on it.
int reset_clock (string str) {
    if (!str || str!="clock") {
        notify_fail ("Reset what?\n") ;
        return 0 ;
    }
    if ( mc != this_player()->GetKeyName() ) {
        write ("Only the mc may reset the clock.\n") ;
        return 1 ;
    }
    if (endtime==0) {
        write ("The clock isn't running.\n") ;
        return 1 ;
    }
    endtime = 0 ;
    write ("You clear the clock.\n") ;
    say (capitalize(mc)+" clears the clock.\n") ;
    // Clear any pending call_outs that may be left behind.
    remove_call_out("expire_time") ;
    remove_call_out("expire_vote") ;
    return 1 ;
}

// Expire_time lets the conference room know that time has expired. It
// doesn't force the speaker to shut up or anything like that: that's left
// to the mcs discretion.
int expire_time() {
    tell_room (environment(this_object()),"The clock runs out.\n") ;
    endtime = 0 ;
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



// Clear_speakers erases the speakers list or agenda, depending on argument.
int clear_items (string str) {
    if ( mc != this_player()->GetKeyName() ) {
        write ("Only the mc may clear the agenda.\n") ;
        return 1 ;
    }
    if (!str || str!="agenda") {
        write ("Usage: clear [agenda]\n") ;
        return 1 ;
    }
    agenda = ({ }) ;
    write ("The agenda has been cleared.\n") ;
    say ("The agenda has been cleared.\n") ;
    return 1 ;
}

// Add_agenda_item lets the mc add an agenda item. If the argument
// is of the form "after <int> <string>", then string is added to the
// agenda AFTER item int. Agenda items are numbered 1-N rather than 0-(N-1).
int add_items (string str) {

    int post ;
    string prop ;

    if ( mc != this_player()->GetKeyName() ) {
        notify_fail ("Only the mc may add agenda items.\n") ;
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

// Remove_item lets the mc take an item off the agenda.
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
    if (member_array( this_player()->GetName(),voters)!=-1) {
        write ("You have already voted!\n") ;
        return 1 ;
    }
    voters += ({ this_player()->GetName() }) ;
    votes[str] = votes[str]+1 ;
    write ("You vote "+str+" on "+vote_str+".\n") ;
    // If this is a roll call vote - ie, the votes are being logged - we announce
    // the vote to the log file and to the room.
    if (votelog) {
        say (capitalize(this_player()->GetName())+" votes "+capitalize(str)+".\n") ;
    }
    return 1 ;
}

// Call_for_vote allows the mc to call for a secret-ballot vote on
// a proposal.
int call_for_vote (string str) {

    string timestr, subjstr ;
    int i ;

    if ( mc != this_player()->GetKeyName() ) {
        write ("Only the mc may call for votes.\n") ;
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
    // This is going to reset the clock: so we want to force the mc to
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
    say (capitalize(mc)+" calls for a vote on "+vote_str+".\n") ;
    say (capitalize(mc)+" sets the clock to "+str+".\n") ;
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

    if ( mc != this_player()->GetKeyName() ) {
        write ("Only the mc may call for votes.\n") ;
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
    say (capitalize(mc)+" calls for a roll call vote on "+vote_str+".\n") ;
    say (capitalize(mc)+" sets the clock to "+str+".\n") ;
    endtime = time()+i ;
    call_out ("expire_vote",i) ;
    return 1 ;
}

// Expire_vote finishes the vote and tabulates the results. It does not
// announce the outcome because some votes required 2/3 or 3/4 to pass
// rather than a simple majority.
int expire_vote() {
    tell_room (environment(this_object()), "The clock runs out. Voting is over.\n") ;
    endtime = 0 ;
    tell_room (environment(this_object()), "The results of the vote were:\n"+
            "Yes:  "+votes["yes"]+"     No:   "+votes["no"]+"       Abstain:  "+
            votes["abstain"]+"\n") ;
    voters = ({ }) ;
    votes["yes"] = 0 ;
    votes["no"] = 0 ;
    votes["abstain"] = 0 ;
    vote_str="none" ;
}

int localupdate() {
    if(!archp(this_player())){
        write ("You may not update objects while in the conference room.\n") ;
        return 1;
    }
}

int eventDestruct(){
    if(sizeof(mc) && !this_player() || (this_player() && !archp(this_player()))){
        write("You may not tamper with the podium.");
        return 0;
    }
    else return ::eventDestruct();
}

int eventMove(mixed dest){
    if(sizeof(mc)){
        write("No.");
        return 0;
    }
    else return ::eventMove(dest);
}

