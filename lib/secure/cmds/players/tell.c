/*    /secure/cmds/player/tell.c
 *    from the Dead Souls LPC Library
 *    the tell command
 *    created by Descartes of Borg 950523
 */

#include <lib.h>
#include <talk_type.h>
#include <message_class.h>
#include <daemons.h>

inherit LIB_DAEMON;

int CheckMud(string name){
    int ret = 3;
    string *imc2list, tmpname;
    if(!(tmpname = INTERMUD_D->GetMudName(name)) ) ret = 0;
    if(tmpname && !INTERMUD_D->GetMudList()[tmpname][0]) ret = 0;
    if(ret){
        return ret;
    }
    if(sizeof(IMC2_D->GetMudName(name, 1))) ret = 2;
    else ret = 0;
    return ret;
}

mixed cmd(string str) {
    string *words;
    mixed mud;
    object ob, machine;
    int i, maxi, insttell;
    string who, msg, tmp, tmp2, machine_message, retname, me;

    if(!str) return notify_fail("Syntax: tell <who> <message>\n");
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
    words = explode(str, " ");
    if(sizeof(words)) who = convert_name(words[0]);
    maxi = sizeof(words = explode(str, "@"));
    if(maxi > 1 && !find_player(who)){
        string tmpmsg, tmpmud;
        who = convert_name(words[0]);
        if(maxi > 2) words[1] = implode(words[1..maxi-1], "@");
        maxi = sizeof(words = explode(words[1], " "));
        if(CheckMud(words[0]) == 2){
            tmpmud = words[0];
            tmpmsg = implode(words[1..]," ");
        }
        for(i=0; i<maxi; i++) {
            tmp = lower_case(implode(words[0..i], " "));
            tmp2 = lower_case(implode(words[0..i+1], " "));

            mud = tmp;

            if( CheckMud(tmp) && !CheckMud(tmp2) ){ 
                if(i+1 < maxi) msg = implode(words[i+1..maxi-1], " ");
                else msg = "";
                break;
            }
            if( IMC2_D->GetMudName(tmp) && !(IMC2_D->GetMudName(tmp2)) ){
                msg = tmpmsg;
                break;
            }
        }
        if(!CheckMud(mud) && !(IMC2_D->GetMudName(mud))){
            write("No such mud found.");
            return 1;
        }
        if(!sizeof(msg)) return notify_fail("Syntax: tell <who> <message>\n");
        if(!mud) mud = -1;
    }
    if(!mud || mud == -1){
        int memb;
        object npc;
        maxi = sizeof(words = explode(str, " "));
        who = 0;
        for(i=0; i<maxi; i++){
            retname = words[0];
            tmp = convert_name(implode(words[0..i], " "));
            memb = member_array(tmp, remote_users());
            ob = find_player(tmp);
            if(!ob) npc = find_living(tmp);
            if(ob || memb != -1 || npc){
                if(!ob && memb == -1 && npc) ob = npc;
                who = tmp;
                if(i+1 < maxi) msg = implode(words[i+1..maxi-1], " ");
                else msg = "";
                break;
            }
        }
        if(!who){
            if(!mud){
                words -= ({ retname });
                msg = implode(words," ");
                this_player()->eventTellHist("You tried to tell "+retname+": "+
                        "%^BLUE%^%^BOLD%^"+ msg + "%^RESET%^");
                insttell = 1;
                INSTANCES_D->SendTell(retname, msg);
                write("Tell whom what?");
                return 1;
            }
            else {
                if(grepp(who, "@")){
                    write("Malformed message.");
                }
                else {
                    write(mud_name()+" is offline or doesn't exist.\n");
                }
                return 1;
            }
        }
        if(msg == ""){
            write("What do you wish to tell?\n");
            return 1;
        }
    }
    else {
        if(CheckMud(mud) == 3){
            if(!creatorp(this_player())) this_player()->AddMagicPoints(-15);
            SERVICES_D->eventSendTell(who, mud, msg);
        }
        else {
            IMC2_D->command("tell "+who+"@"+mud+" "+msg);
        }
        return 1;
    }
    if(ob){
        mixed err;
        if(!insttell) INSTANCES_D->SendTell(who, msg);
        insttell = 1;
        if(archp(ob) || (!archp(this_player()) && creatorp(ob))) 
            me = capitalize(this_player()->GetKeyName());
        else me = this_player()->GetName(); 

        machine=present("answering machine",ob);
        if(ob && !creatorp(ob)) this_player()->AddMagicPoints(-15);
        if(machine && base_name(machine) == "/secure/obj/machine"){
            int parse_it;
            parse_it=machine->query_answer();
            if(parse_it){
                machine->get_message(me+" tells you: "+msg+"\n");
                machine_message=machine->send_message();
            }
        }
        if( (err = this_player()->CanSpeak(ob, "tell", msg)) != 1){
            if(ob && !creatorp(ob)) this_player()->AddMagicPoints(15);
            this_player()->eventTellHist("You tried to tell "+retname+": "+
                    "%^BLUE%^%^BOLD%^"+ msg + "%^RESET%^");
            return err || "Tell whom what?";
        }
        if( ob->GetInvis() && ( ( archp(ob) && !archp(this_player()) ) 
                    || ( creatorp(ob) && !creatorp(this_player()) ) ) ){
            string inv_ret = "%^BLUE%^%^BOLD%^" + me + 
                " unknowingly tells you, %^RESET%^\"" + msg + "\"";
            if(!machine_message){
                ob->eventPrint(inv_ret);
            }
            ob->eventTellHist(inv_ret);
            ob->SetProperty("reply", lower_case(me));
            ob->SetProperty("reply_time", time());
            if(!insttell) INSTANCES_D->SendTell(who, msg);
            insttell = 1;
            this_player()->eventTellHist("You tried to tell "+retname+": "+
                    "%^BLUE%^%^BOLD%^"+ msg + "%^RESET%^");
            if(query_verb() == "tell") return "Tell whom what?";
            else {
                write("Tell whom what?");
                return 1;
            }
        }
        if(machine_message) message("info", machine_message, this_player());
#ifdef BLOCK_TELLS_TO_AFK
        if(ob->GetProperty("afk")) {
            message("my_action", ob->GetName()+
                    " is afk and cannot receive your message.", this_player()); 
        }
#endif
        else this_player()->eventSpeak(ob, TALK_PRIVATE, msg);
        ob->SetProperty("reply", lower_case(me));
        ob->SetProperty("reply_time", time());
        if(!archp(ob) && userp(ob) && (query_idle(ob) > 60))
            message("my_action", ob->GetName()+
                    " is idle and may not have been paying attention.", this_player());
        else if((in_edit(ob) || in_input(ob))
#ifdef __DSLIB__
                && !query_charmode(ob)
#endif
               )
            message("my_action", ob->GetCapName()+" is in input "+
                    "and may not be able to respond.", this_player());
        else if(ob->GetSleeping())
            message("my_action", ob->GetCapName()+" is sleeping "+
                    "and is unable to respond.", this_player());
    }
    else {
        string ret;
        ret = "%^BOLD%^RED%^You tell " + capitalize(who) +
            ":%^RESET%^ " + msg;
        this_player(1)->eventPrint(ret, MSG_CONV);
        this_player(1)->eventTellHist(ret);
        if(!insttell) INSTANCES_D->SendTell(who, msg);
    }
    return 1;
}

string GetHelp(){
    return ("Syntax: tell <player> <message>\n"
            "        tell <player>@<mud> <message>\n\n"
            "Sends the message to the player named either on this mud if no "
            "mud is specified, or to the player named on another mud when "
            "another mud is specified. If the other mud is on an IMC2 network "
            "rather than an Intermud-3 network, use \"imc2 tell\""
            "\nSee also: imc2, say, shout, yell, emote");
}
