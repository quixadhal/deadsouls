/*    /secure/cmds/player/tell.c
 *    from the Dead Souls LPC Library
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
    object ob;
    int i, maxi;
    string who, msg, tmp;
 
    if(!str) return notify_fail("Syntax: <tell [who] [message]>\n");
    mud = 0;
    if((maxi=sizeof(words = explode(str, "@"))) > 1) {
        who = convert_name(words[0]);
        if(maxi > 2) words[1] = implode(words[1..maxi-1], "@");
        maxi = sizeof(words = explode(words[1], " "));
        for(i=0; i<maxi; i++) {
            string *mts;
            tmp = lower_case(implode(words[0..i], " "));
 
            if( (string)INTERMUD_D->GetMudName(tmp) ) {
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
            if(ob=find_living(tmp=convert_name(implode(words[0..i], " ")))) {
                who = tmp;
                if(i+1 < maxi) msg = implode(words[i+1..maxi-1], " ");
                else msg = "";
                break;
            }
        }
        if(!who) {
            if(!mud) return notify_fail("Tell whom what?\n");
            else return notify_fail(mud_name()+" is not aware of that mud.\n");
        }
        if(msg == "") return notify_fail("What do you wish to tell?\n");
    }
    else {
        SERVICES_D->eventSendTell(who, mud, msg);
        return 1;
    }
    if(ob) {
        string frm;
        mixed err;
 
        if( (err = (mixed)this_player()->CanSpeak(ob, "tell", msg)) != 1)
          return err || "Tell whom what?";
        if( ob->GetInvis() && creatorp(ob) && !archp(this_player()) ) {
            ob->eventPrint("%^BLUE%^%^BOLD%^" +
              (string)this_player()->GetName() + " unknowingly "
              "tells you, %^RESET%^\"" + msg + "\"");
            return "Tell whom what?";
        }
        else this_player()->eventSpeak(ob, TALK_PRIVATE, msg);
        ob->SetProperty("reply", (string)this_player()->GetKeyName());
        if(userp(ob) && (query_idle(ob) > 60))
          message("my_action", (string)ob->GetName()+
            " is idle and may not have been paying attention.", this_player());
        else if(in_edit(ob) || in_input(ob))
          message("my_action", (string)ob->GetCapName()+" is in input "+
            "and may not be able to respond.", this_player());
    }
    return 1;
}
 
string GetHelp(string str) {
    return ("help",
      "Syntax: <tell [player] [message]>\n"
      "        <tell [player]@[mud] [message]>\n\n"
      "Sends the message to the player named either on this mud if no "
      "mud is specified, or to the player named on another mud when "
      "another mud is specified.  For muds with more than one word in their "
      "names, use . (periods) to take place of spaces.  Example: tell "
      "descartes@realms.of.chaos hi\n\n"
      "See also: say, shout, yell, emote");
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
