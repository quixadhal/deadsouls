/* It works now, no define needed.
 * make sure someone does "setmc" or it won't do much.
 * setmc can only be done once, first come, first get i guess ;)
 * by Boy@frontiers.
 * Major corrections and modifications by Crat 25jul05
 */
#include <lib.h>
#include <vendor_types.h>
inherit LIB_STORAGE;
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
	"/domains/campus/obj/key1" : 1
      ]));
    mc = "";
    x = 0;
    speaker = "";
}
void init() {
    ::init();
    add_action("CatchInput", "",1);
    //add_action("eventSay", "say",1);
    //add_action("eventRaise", "raise");
    //add_action("eventCallOn", "recognize");
    //add_action("shaddap", "yell");
    //add_action("shaddap", "shout");
    //add_action("shaddap", "emote");
    //add_action("help", "help");
    //add_action("quiet", "quiet");
    //add_action("changemc", "changemc");
    //add_action("SetMc", "setmc");

}
mixed CanGet(object ob) { return "The podium does not budge.";}



int eventSay(string args) {
    if (mc == this_player()->GetKeyName()) {
	this_player()->eventPrint("You say \"%^CYAN%^" + args + "\"");
	say(this_player()->GetName() + " says \"%^CYAN%^" + args + "\"");
	return 1;
    }

    if ( mc != "" && this_player()-> GetKeyName() != speaker) {
	this_player()->eventPrint("%^RED%^It is not polite to talk out of order.");
	this_player()->eventPrint("Raise your hand if you'd like to speak.");
	return 1;
    }

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
    write("%^CYAN%^You thank " + capitalize(speaker) + " for speaking.");
    find_living(speaker)->eventPrint("%^CYAN%^" + capitalize(mc) + " thanks you for you speaking.");
    speaker = mc;
    return 1;
}
int changemc(string args) {
    if (args != 0) {
	if (this_player()->GetKeyName() != mc) {
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
int SetMc(string args) {
    if (x==0) {
	if (args != 0) {
	    if (present(args)) {
		mc = args;
		say("%^CYAN%^" + capitalize(mc) + " is the speaker.");	
		write("You set " + capitalize(args) + " as the speaker.");
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

void CatchInput(string str){
    string cmd,args;
    string *cmds;
    cmds = ({ "setmc","say","help","quiet","changemc","recognize" });
    cmd=query_verb();
    write("cmd: "+cmd);
    if(str && str!= "") args = str;
    write("args: "+args);
    if(cmd == "emote" || member_array(cmd, "/daemon/soul"->GetEmotes()) != -1){
	shaddap();
	return;
    }
    //if(cmd == "setmc") SetMc(args);
    //if(cmd == "say") eventSay(args);
    //if(cmd == "help") help(args);
    //if(cmd == "quiet") quiet();
    //if(cmd == "changemc") changemc(args);
    //if(cmd == "recognize") eventCallOn(args);
    //if(cmd == "raise") eventRaise();
    //if(member_array(cmd,cmds) != -1) return;
    //if(cmd == "say") return;
}

