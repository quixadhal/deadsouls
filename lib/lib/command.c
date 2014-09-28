/*    /lib/command.c
 *    from the Dead Souls Object Library
 *    handles commands of living objects
 *    created by Descartes of Borg 950323
 *    Version: @(#) command.c 1.2@(#)
 *    Last modified: 96/12/07
 */


#include <lib.h>
#include <daemons.h>
#include "include/command.h"

#define OLD_STYLE_PLURALS 1

int Paused = 0;
private static int Forced = 0;
private static int StillTrying = 0;
private static int ParseRecurse = 0;
private static string CommandFail;
private static string *SearchPath;
private static int last_cmd_time = 0;
private static int cmd_count = 1;
private string *localcmds = ({});
private string parsed_command = "";
private static string *QueuedCommands = ({});
static string current_command = "";
static string original_command = "";
static int Charmode = 0;
static string Charbuffer = "";

int direct_force_liv_str(){ return 1; }
int direct_force_liv_to_str(){ return 1; }


/*  ***************  /lib/command.c driver applies  ***************  */

static void create(){
    SetCommandFail(0);
    SearchPath = ({ DIR_PLAYER_CMDS, DIR_SECURE_PLAYER_CMDS, DIR_CLAN_CMDS,
            DIR_COMMON_CMDS, DIR_SECURE_COMMON_CMDS });
}

static string process_input(string args){ 
    string verb, real_verb, tmpalias, orig;
    object env = environment(this_object());
    string *talks = ({ "emote", "tell", "reply", "say", "whisper",
            "yell", "shout", "speak" });
    string *filecmds = ({ "ced", "clone", "goto", "rehash", "reset",
            "showtree", "bk", "cat", "cp", "diff", "ed", "grep", "head", "indent",
            "longcat", "more", "mv", "rm", "sed", "showfuns", "source",
            "tail", "update", "cd", "ls", "mkdir", "rmdir" });
    string *specialcmds = ({ "modify", "mudconfig", "describe", "read", "title", "register" });
    string *exempts = talks + filecmds + specialcmds;
    exempts += this_object()->GetChannels();
    exempts += SOUL_D->GetEmotes();

    orig = args;

    localcmds = ({});
    filter(commands(), (: localcmds += ({ $1[0] }) :));

    if(sizeof(args)){
        string *tmpargs = explode(args, " ");
        if(sizeof(tmpargs)) verb = tmpargs[0];
        else verb = "";
    }
    if(verb && tmpalias = this_object()->GetAlias(verb)){
        real_verb = explode(tmpalias, " ")[0];
    }
    if(verb && tmpalias = this_object()->GetXverb(verb[0..0])){
        real_verb = explode(tmpalias, " ")[0];
    }
    if(!real_verb) real_verb = verb;
    if(Paused && (member_array(real_verb, exempts) == -1)){
        this_object()->eventPrint("You are paused.");
        return "";
    }
    if(!archp(this_object()) && MAX_COMMANDS_PER_SECOND){
        if(last_cmd_time == time()) cmd_count++;
        else {
            last_cmd_time = time();
            cmd_count = 1;
        }
        if(cmd_count > MAX_COMMANDS_PER_SECOND){
            this_object()->eventPrint("You have exceeded the " +
                    MAX_COMMANDS_PER_SECOND + " commands per second limit.");
            return "";
        }
    }
    if(this_object()->GetSleeping() > 0){
        if(verb != "wake"){
            this_object()->eventPrint("You are asleep.");
            return "";
        }
    }
    if(OLD_STYLE_PLURALS && args && (member_array(real_verb, exempts) == -1 ||
                (member_array(verb, exempts) != -1 && !strsrch(trim(args),"to ")))){
        int numba, i, tmp_num;
        string tmp_ret;
        string *line = explode(args," ");
        for(i = 1; i < sizeof(line); i++){
            string element;
            if(!line[i]) error("String handling error in old style plural parser.");
            element = line[i];
            exempts = sort_array(exempts, 1);
            if(sscanf(element,"%d.%d",numba,tmp_num) != 2 &&
                    sscanf(element,"%d.%s",numba,tmp_ret) == 2 &&
                    (member_array(real_verb, exempts) == -1)){
                args = replace_string(args, numba + ".", 
                        numba + ordinal(numba) + " ");
                continue;
            }
            if(numba = atoi(element)){
                int j;
                object o1;
                string e1, e2, tmp_thing = "";
                e1 = numba+ordinal(numba);
                j = member_array(element, line);
                while(j > 0){
                    string old_tmp = tmp_thing;
                    j--;
                    e2 = line[j];
                    if(sizeof(tmp_thing)) tmp_thing = e2 + " " + tmp_thing;
                    else tmp_thing = e2;
                    if(member_array(tmp_thing, exempts) != -1) break;
                    o1 = present(tmp_thing);
                    if(!o1){
                        o1 = present(old_tmp);
                        if(o1){
                            tmp_ret = e1 + " " + old_tmp;
                            args=replace_string(args,old_tmp+" "+numba,tmp_ret);
                        }
                    }
                }
            }//end single number check
        }
    }
    parsed_command = args;
    if(member_array(verb, localcmds) != -1){
        current_command = orig;
    }
    else {
        current_command = parsed_command;
        parsed_command = "";
    }
    return current_command;
}

/*  ***************  /lib/command.c command lfuns  ***************  */

static int cmdAll(string args){
    object old_agent, env;
    mixed err;
    string verb, file;

    if(grepp(parsed_command," ")){
        sscanf(parsed_command,"%s %s", verb, args);
        parsed_command = "";
    }

    if(!verb) verb = query_verb();

    if(!this_object()->GetCharmode()){
        if(!args) this_object()->Push(verb);
        else this_object()->Push(verb+" "+args);
    }
    else {
        this_object()->Push(this_object()->GetTempbuffer());
    }

    old_agent = this_agent(this_object());

    env = environment();
    if(BARE_EXITS && env){
        localcmds = ({});
        filter(commands(), (: localcmds += ({ $1[0] }) :));
        if(member_array(verb,CMD_D->GetCommands()) == -1 &&
                member_array(verb,keys(VERBS_D->GetVerbs())) == -1 &&
                member_array(verb,localcmds) == -1 ){
            string dir;
            if(args) dir = verb + " " + args;
            else dir = verb;
            if(member_array(dir, (env->GetExits() || ({}))) != -1) 
                verb = "go "+verb;
            if(member_array(dir, (env->GetEnters() || ({}))) != -1) 
                verb = "enter "+verb;
        }
    }

    if(COMMAND_MATCHING && sizeof(match_command(verb))){
        verb = match_command(verb);
    }

    if(query_custom_command(verb) && query_custom_command(verb) != "" && !creatorp(this_player()) ){
        this_player()->eventPrint("How clever of you. Or lucky. In any case, this command is unavailable to you.");
        return 1;
    }
    if( !(file = (query_custom_command(verb) )) || query_custom_command(verb) == ""){
        if( !(file = CMD_D->GetCommand(verb, GetSearchPath())) ){
            string cmd;
            int dbg;

            if( verb && args ) cmd = verb + " " + args;
            else if(verb) cmd = verb;
            else if(args) cmd = args;
            if( this_object()->GetProperty("parse debug") ) dbg = 1;
            else if( this_object()->GetProperty("debug") ) dbg = 1;
            else dbg = 0;
            if( (err = parse_sentence(cmd, dbg)) == 1 ){
                this_agent(old_agent || 1);
                return 1;
            }
            if( err ){
                if( err == -1 ){
                    if( !(err = VERBS_D->GetErrorMessage(verb)) &&
                            !(err = SOUL_D->GetErrorMessage(verb)) ){
                        err = "Such a command exists, but no default "
                            "syntax is known.";
                    }
                }
                if( intp(err) )  /* MudOS bug */ err = "What?";
                SetCommandFail(err);
            }
            message("error", GetCommandFail(), this_object());
            this_agent(old_agent || 1);
            return 1;
        }
    }

    if( (err = call_other(file, "cmd", args)) != 1 ){
        string cmd;

        if( err ) SetCommandFail(err);
        if( !args || args == "" ) cmd = verb;
        else cmd = verb + " " + args;
        if( (err = parse_sentence(cmd)) == 1 ){
            this_agent(old_agent || 1);
            return 1;
        }
        if( !err ) err = GetCommandFail();
        message("error", err, this_object());
        this_agent(old_agent || 1);
        return 1;
    }
    this_agent(old_agent || 1);
    return 1;
}

int cmdDebugAll(string args){
    object old_agent;
    mixed err;
    string verb, file;

    old_agent = this_agent(this_object());
    verb = query_verb();
    if( !(file = CMD_D->GetCommand(verb, GetSearchPath())) ){
        string cmd;

        if( args ) cmd = verb + " " + args;
        else cmd = verb;
        if( (err = parse_sentence(cmd, 3)) == 1 ){
            this_agent(old_agent || 1);
            return 1;
        }
        if( err ) SetCommandFail(err);
        message("error", GetCommandFail(), this_object());
        this_agent(old_agent || 1);
        return 1;
    }
    if( (err = call_other(file, "cmd", args)) != 1 ){
        string cmd;

        if( err ) SetCommandFail(err);
        if( !args || args == "" ) cmd = verb;
        else cmd = verb + " " + args;
        if( (err = parse_sentence(cmd, 3)) == 1 ){
            this_agent(old_agent || 1);
            return 1;
        }
        if( !err ) err = GetCommandFail();
        message("error", err, this_object());
        this_agent(old_agent || 1);
        return 1;
    }
    this_agent(old_agent || 1);
    return 1;
}

/*  ***************  /lib/command.c lfuns  ***************  */

int Setup(){
    enable_commands();
    add_action( (: cmdAll :), "", 1);
}

int eventForce(string cmd){
    string err;
    int res;
#if 0
    if(this_player() && interactive(this_object()) 
            && this_player() != this_object()){
        string forcer = this_player()->GetKeyName();
        string forcee = this_object()->GetKeyName();
        log_file("adm/force", 
                timestamp()+" "+forcer+" forced "+forcee+" to "+cmd+"\n");
    }
#endif
    if(!cmd) return 0;

    cmd = process_input(cmd);
    if(!cmd) return 0;
    Forced = 1;
    err = catch(res = command(cmd));
    Forced = 0;
    if(err){
        error(err);
    }
    return res;
}

int eventForceQueuedCommand(string cmd){
    tell_object(this_object(),"%^RED%^Executing queued command: %^RESET%^"+cmd);
    eventForce(cmd);
}

int eventExecuteQueuedCommands(){
    int i = 0;
    foreach(string tmp in QueuedCommands){
        i++;
        //tmp = process_input(tmp);
        call_out("eventForceQueuedCommand", i, tmp);
        QueuedCommands -= ({ tmp });
    }
}

int eventQueueCommand(string line){
    if(!line || !sizeof(line) || !stringp(line)) return 0;
    if(!this_player()) return 0;
    if(interactive(this_object())){
        if(this_player() && this_player() != this_object()) return 0;
    }
    if(line != "") QueuedCommands += ({ line });
    return 1;
}

int DoneTrying(){
    return StillTrying = 0;
}

//Here we determine if the string corresponds unambiguously
//to a limb possessed by the command issuer.
int LimbCertain(string str){
    object env = environment(this_player());
    if(!env) env = this_player();
    if(!str){
        return 0;
    }
    foreach(mixed envs in ({ env, this_player() })){
        if(present(str, env)){
            return 0;
        }
    }
    if(member_array(str, this_player()->GetLimbs()) != -1){
        return 1;
    }
    return 0;
}

varargs int eventRetryCommand(string lastcmd, int errtype, mixed args){
    string virb, wrd, prep, rest,ret;
    string *tmp_arr = ({});
    string *prep_arr = MASTER_D->parse_command_prepos_list();
    object tmpob;
    mixed err;
    int i;
    string tmpret, act, direct, indirect, odirect, oindirect;
    mixed direct2, indirect2;

    if(previous_object() != master()) return 0;

    if(StillTrying){
        StillTrying = 0;
        original_command = 0;
        return 1;
    }

    StillTrying = 1;

    if(!original_command || !sizeof(original_command)) original_command = lastcmd;

    tmpret = (lastcmd || "");
    foreach(string foo in prep_arr){
        if(grepp(tmpret," "+foo+" ")){
            tmpret = replace_string(tmpret," "+foo+" "," PREPO ");
        }
    }
    tmpret = implode(explode(tmpret," ")," ");
    i = sscanf(tmpret,"%s %s PREPO %s",act, direct, indirect);
    if(i != 3) i = sscanf(tmpret,"%s %s PREPO PREPO %s",act, direct, indirect);
    if(i != 3) i = sscanf(tmpret,"%s %s %s",act, direct, indirect);
    if(i != 3) i = sscanf(tmpret,"%s %s",act, direct);
    odirect = direct;
    oindirect = indirect;

    //read is a special case. takes a str as first arh sometimes.
    if(act == "read" && !grepp(tmpret, " in a ")){
        tmpret = replace_string(lastcmd, " in ", " in a ");
    }
    else if(i > 1 ){
        object ob1, ob2;
        string tmpstr, s1, s2, article1, article2;
        if(StillTrying){
            tmpstr = remove_article(direct);
            if(args && sizeof(args) && answers_to(tmpstr, args[0]) &&
                    present(args[0]))
                ob1 = args[0];
            else ob1 = to_object(tmpstr, this_object());
        }
        if(StillTrying && indirect){
            tmpstr = remove_article(indirect);
            if(args && sizeof(args) && answers_to(tmpstr, args[0]) &&
                    present(args[0]))
                ob2 = args[0];
            else ob2 = to_object(tmpstr, this_object());
        }
        if(ob1) direct = ob1->GetUniqueId();
        else direct = remove_article(direct);
        if(ob2) indirect = ob2->GetUniqueId();
        else indirect = remove_article(indirect);
        if(!ob1 && direct && grepp(direct," ")){
            if(sscanf(direct,"%s %s",s1, s2) == 2 && 
                    !ordinalp(s1,1)){
                direct2 = "a "+direct;
            }
        }
        else if(!ob1 && direct){
            if(first(direct,2) == "a " || first(direct,3) == "an "){
                direct2 = direct;
            }
            else direct2 = "a "+direct;
        }
        if(!ob2 && indirect && grepp(indirect," ")){
            if(sscanf(indirect,"%s %s",s1, s2) == 2 && 
                    !ordinalp(s1,1)){
                if(!LimbCertain(indirect)) indirect2 = "a "+indirect;
            }
        }
        else if(!ob2 && indirect){
            if(first(indirect,2) == "a " || first(indirect,3) == "an "){
                if(!LimbCertain(indirect)) indirect2 = indirect;
            }
            else if(!LimbCertain(indirect)) indirect2 = "a "+indirect;
        }
        if(direct && !direct2) direct2 = direct;
        if(indirect && !indirect2) indirect2 = indirect;
        tmpret = replace_string(lastcmd, odirect, direct2);
        if(indirect2 && oindirect) tmpret = replace_string(tmpret, oindirect, indirect2); 
    }

    ret = tmpret;
    err = parse_sentence(ret);
    StillTrying = 0;
    original_command = 0;
    if( !stringp(err) && err > -1 ){
        return 1;
    }
    write("It seems you'll have to be more specific.");
    return 1;
}


/*  **********  /lib/command.c data manipulation functions  ********** */

string *AddSearchPath(mixed val){
    if(stringp(val)){
        if(!strsrch(val,"/secure/cmds/admins") || !strsrch(val,"/cmds/admins")){
            if(!master()->valid_apply(({ "SECURE", "ASSIST", "LIB_CONNECT" })) ){
                tell_creators("Security violation in progress: "+identify(previous_object(-1)) + ", "+get_stack());
                error("Illegal attempt to modify path data: "+identify(previous_object(-1)) + ", "+get_stack());

            }
        }
        val = ({ val });
    }

    else if(!pointerp(val)) error("Bad argument 1 to AddSearchPath()\n");
    return (SearchPath = distinct_array(SearchPath + val));
}

string *RemoveSearchPath(mixed val){
    if(stringp(val)) val = ({ val });
    else if(!pointerp(val)) error("Bad argument 1 to RemoveSearchPath()\n");
    return (SearchPath -= val);
}

string *GetSearchPath(){ return SearchPath; }

int GetForced(){ return Forced; }

int GetClient(){ return 0; }

string GetCurrentCommand(){
    if(!this_player()) return "";
    if(this_player() != this_object()) return "";
    return current_command;
}

int SetPlayerPaused(int i){
    if( base_name(previous_object()) != LIB_CONNECT &&
            (!this_player() || !archp(this_player())) ){
        error("Illegal attempt to pause a player: "+get_stack()+" "+identify(previous_object(-1)));
        log_file("adm/pause",timestamp()+" Illegal attempt to access SetPlayerPaused on "+identify(this_object())+" by "+identify(previous_object(-1))+"\n");
    }
    Paused = i;
    return Paused;
}

int GetPlayerPaused(){
    return Paused;
}

string SetCommandFail(string str){ 
    if( !str || str == "" ){
        if(!creatorp(this_object())) CommandFail = "Try \"help commands\" for a list of some commands.";
        if(creatorp(this_object())) CommandFail = "Try \"help creator commands\" for a list of some creator commands.";
        return CommandFail;
    }
    else return (CommandFail = str);
}

string GetCommandFail(){ return CommandFail; }
