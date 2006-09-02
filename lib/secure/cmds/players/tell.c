/*    /secure/cmds/player/tell.c
 *    from the Nightmare IV LPC Library
 *    the tell command
 *    created by Descartes of Borg 950523
 */

#include <lib.h>
#include <talk_type.h>
#include <daemons.h>

inherit LIB_DAEMON;

mixed cmd(string str) {
    string *words;
    mixed mud;
    object ob, machine;
    int i, maxi;
    string who, msg, tmp, tmp2, machine_message, retname;

    if(!str) return notify_fail("Syntax: <tell [who] [message]>\n");

    if(str == "hist" || str == "history"){
	string ret = "Your tell history: \n\n"; 
	ret += implode(this_player()->GetTellHistory(),"\n");
	print_long_string(this_player(), ret);
	return 1;
    }

    if(!creatorp(this_player()) && this_player()->GetMagicPoints() < 15) {
	write("You lack sufficient magic to tell to anyone right now.");
	return 1;
    }
    mud = 0;
    if((maxi=sizeof(words = explode(str, "@"))) > 1) {
	who = convert_name(words[0]);
	if(maxi > 2) words[1] = implode(words[1..maxi-1], "@");
	maxi = sizeof(words = explode(words[1], " "));
	for(i=0; i<maxi; i++) {
	    tmp = lower_case(implode(words[0..i], " "));
	    tmp2 = lower_case(implode(words[0..i+1], " "));

	    if( (string)INTERMUD_D->GetMudName(tmp) 
	      && !((string)INTERMUD_D->GetMudName(tmp2)) ) {
		mud = tmp;
		if(i+1 < maxi) msg = implode(words[i+1..maxi-1], " ");
		else msg = "";
		break;
	    }
	}
	if(msg == "") return notify_fail("Syntax: <tell [who] [message]>\n");
	if(!mud) mud = -1;
    }
    if(!mud || mud == -1) {
	maxi = sizeof(words = explode(str, " "));
	who = 0;
	for(i=0; i<maxi; i++) {
	    retname = words[0];
	    if(ob=find_living(tmp=convert_name(implode(words[0..i], " ")))) {
		who = tmp;
		if(i+1 < maxi) msg = implode(words[i+1..maxi-1], " ");
		else msg = "";
		break;
	    }
	}
	if(!who) {
	    if(!mud){
		words -= ({ retname });
		msg = implode(words," ");
		this_player()->eventTellHist("You tried to tell "+retname+": "+
		  "%^BLUE%^%^BOLD%^"+ msg + "%^RESET%^");
		return notify_fail("Tell whom what?\n");
	    }
	    else return notify_fail(mud_name()+" is not aware of that mud.\n");
	}
	if(msg == "") return notify_fail("What do you wish to tell?\n");
    }
    else {
	if(!creatorp(this_player())) this_player()->AddMagicPoints(-15);
	SERVICES_D->eventSendTell(who, mud, msg);
	return 1;
    }
    if(ob) {
	string frm;
	mixed err;

	machine=present("answering machine",ob);
	if(archp(ob)) frm = (string)this_player()->GetCapName();
	else frm = (string)this_player()->GetName();
	if(ob && !creatorp(ob)) this_player()->AddMagicPoints(-15);
	if(machine && base_name(machine) == "/secure/obj/machine"){
	    int parse_it;
	    parse_it=machine->query_answer();
	    if(parse_it){
		machine->get_message(frm+" tells you: "+msg+"\n");
		machine_message=machine->send_message();
		message("info", machine_message, this_player());
		return 1;
	    }
	}
	if( (err = (mixed)this_player()->CanSpeak(ob, "tell", msg)) != 1){
	    if(ob && !creatorp(ob)) this_player()->AddMagicPoints(15);
	    this_player()->eventTellHist("You tried to tell "+retname+": "+
	      "%^BLUE%^%^BOLD%^"+ msg + "%^RESET%^");
	    return err || "Tell whom what?";
	}
	if( ob->GetInvis() && creatorp(ob) && !archp(this_player()) ) {
	    string inv_ret = "%^BLUE%^%^BOLD%^" + (string)this_player()->GetName() + 
	    " unknowingly tells you, %^RESET%^\"" + msg + "\"";
	    ob->eventPrint(inv_ret);
	    ob->eventTellHist(inv_ret);
	    this_player()->eventTellHist("You tried to tell "+retname+": "+
	      "%^BLUE%^%^BOLD%^"+ msg + "%^RESET%^");
	    return "Tell whom what?";
	}
	else this_player()->eventSpeak(ob, TALK_PRIVATE, msg);
	ob->SetProperty("reply", (string)this_player()->GetKeyName());
	if(!archp(ob) && userp(ob) && (query_idle(ob) > 60))
	    message("my_action", (string)ob->GetName()+
	      " is idle and may not have been paying attention.", this_player());
	else if(in_edit(ob) || in_input(ob))
	    message("my_action", (string)ob->GetCapName()+" is in input "+
	      "and may not be able to respond.", this_player());
    }
    return 1;
}

void help(string str) {
    message("help",
      "Syntax: <tell [player] [message]>\n"
      "        <tell [player]@[mud] [message]>\n\n"
      "Sends the message to the player named either on this mud if no "
      "mud is specified, or to the player named on another mud when "
      "another mud is specified.  For muds with more than one word in their "
      "names, use . (periods) to take place of spaces.  Example: tell "
      "descartes@realms.of.chaos hi\n\n"
      "See also: say, shout, yell, emote",this_player());
}

string morse(string msg) {
    mapping __Morse;
    string tmp;
    int x, i;
    __Morse = ([ "a" : ".-", "b" : "-...", "c" : "-.-.",
      "d" : "-..", "e" : ".", "f" : "..-.", "g" : "--.", "h" : "....", "i" : "..",
      "j" : ".---", "k" : "-.-", "l" : ".-..", "m" : "--", "n" : "-.", "o" : "---",
      "p" : ".--.", "q" : "--.-", "r" : " .-.", "s" : "...", "t" : "-", "u" : "..-",
      "v" : "...-", "w" : ".--", "x" : "-..-", "y" : "-.--", "z" : "--..",
      "1" : ".----", "2" : "..---", "3" : "...--", "4" : "....-", "5" : ".....",
      "6" : " -....", "7" : "--...", "8" : "---..", "9" : "----.","0" : " -----" ]);
    for(tmp = "", x = strlen(msg), i=0; i< x; i++) {
	if(__Morse[msg[i..i]]) tmp += __Morse[msg[i..i]]+" ";
	else tmp += msg[i..i]+ " ";
    }
    return tmp;
}
