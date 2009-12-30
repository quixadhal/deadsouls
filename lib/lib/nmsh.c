/*    /lib/user/nmsh.c
 *    from Dead Souls
 *    the new Dead Souls shell
 *    created by Descartes of Borg 940216
 *    Version: @(#) nmsh.c 1.3@(#)
 *    Last Modified: 96/10/15
 */

#include <daemons.h>
#include <lib.h>
#include "include/nmsh.h"

inherit LIB_HISTORY;

#define DIRECTORY_STACK_SIZE     5
#define MAX_CMD_ALIASES          192
#define FAIL                     0
#define CMD_EDITING              1
#define CHAR_LIMIT               1024

private mapping Nicknames, Aliases, Xverbs, Directories; 
private static int CWDCount, CWDBottom, CWDTop, CmdNumber; 
private string Prompt; 
private static string *Stack; 
static int histmatch = 0; 
static string recalled_command = ""; 
static string recalled_command_sub = ""; 
static int recalled_command_num = 0; 
private static int noecho = 1; 
private static mapping Termstuff;

static int *GetScreen(){ return ({ 79, 24 }); }

static void create(){
    Nicknames = ([]); 
    Termstuff = ([]); 
    Directories = ([ "current" : "/", "previous" : "/", "home" : 0 ]);
    recalled_command_num = sizeof(this_object()->GetCommandHist()) - 1;
    if(recalled_command_num > -1){
        recalled_command=this_object()->GetCommandHist()[recalled_command_num];
    }
    Aliases = ([ "l" : "look $*", "bio" : "biography", "i" : "inventory",
            "n" : "go north", "s" : "go south", "e" : "go east", "w" : "go west",
            "ne" : "go northeast", "nw" : "go northwest", "se" : "go southeast",
            "sw" : "go southwest", "d" : "go down", "u" : "go up", "out": "go out",
            "exa" : "look at $*", "p" : "people", "sc" : "status", "inf" : "score",
            "eq" : "equipment", "prac" : "skills", 
            "sco" : "score", "practice" : "skills", "trophy" : "kills",
            "northwest" : "go northwest", "northeast" : "go northeast", 
            "southwest" : "go southwest", "southeast" : "go southeast",
            "north" : "go north", "south": "go south", "east" : "go east",
            "west" : "go west", "up" : "go up", "down": "go down", 
            "ig" : "intergossip $*", "c" : "cre $*", "lp" : "lpuni $*",
            "inv" : "inventory", "x" : "look at $*", "examine" : "look at $*",
            "ic" : "intercre $*", "loot" : "get all from $*", "chat" : "newbie $*",
            "dc" : "dchat $*", "exit" : "go out", "t" : "tell $*", "k" : "kill $*",
            "alist" : "ls /domains", "zlist" : "domains", "mwhere" : "findobj $*",
            "owhere" : "findobj $*", "peace" : "quell", "vnum" : "help find",
            ]);
    Xverbs = (["]":"] $*", "'":"say $*",":":"emote $*","\"":"say $*",]);
    if(this_object()->GetKeyName() == "cratylus"){
        Aliases["crat"] = "source /secure/scripts/crat.src"; 
    }
} 

int Setup(){ 
    if(this_player() != this_object()) return 0; 
    reset_prompt(); 
    if(!Nicknames) Nicknames = ([]);
    if(!Aliases) Aliases = ([]);
    if(!Xverbs) Xverbs = ([]);
    add_action("cmd_alias", "alias",1); 
    add_action("cmd_unalias", "unalias",1); 
    add_action("cmd_nickname", "nickname",1); 
#if FAIL
    add_action("fail","fail");
#endif
    if(creatorp(this_object())){ 
        Stack = allocate(DIRECTORY_STACK_SIZE); 
        CWDBottom = CWDTop = CWDCount = 0; 
        add_action("cmd_cd", "cd"); 
        add_action("cmd_nmsh", "nmsh"); 
        add_action("cmd_pushd", "pushd"); 
        add_action("cmd_popd", "popd"); 
        add_action("cmd_pwd", "cwd");
        add_action("cmd_pwd", "pwd");
        add_action("cmd_work", "work");
    } 
    return 1;
} 

nomask static int cmd_alias(string str){ 
    string *a, *b;
    string key, thing; 
    int i;

    if(this_player() != this_object()) return 0; 
    if(!str){
        i = sizeof(a = keys(Aliases));
        while(i--)
            a[i] = sprintf("%s%s", arrange_string(a[i],15),Aliases[a[i]]);
        i = sizeof(b = keys(Xverbs));
        while(i--)
            b[i] = sprintf("$%s%s", arrange_string(b[i],14), Xverbs[b[i]]);
        this_player()->eventPage(a+b);
        return 1;
    }
    if(sscanf(str, "%s %s", key, thing) != 2){ 
        if(str[0] == '$'){ 
            str = str[1..strlen(str)-1]; 
            if(Xverbs[str]){ 
                write(str+": "+Xverbs[str]);
                return 1;
            } 
            else message("system", sprintf("No such alias $%s.", str), this_player()); 
            return 1; 
        } 
        if(Aliases[str]){ 
            write(str+": "+Aliases[str]);
            return 1;
        } 
        else message("system", sprintf("No such alias %s.", str), this_player()); 
        return 1; 
    } 
    if(sizeof(Xverbs) + sizeof(Aliases) >= MAX_CMD_ALIASES){ 
        message("system", "You must remove an alias before adding another.", 
                this_player()); 
        return 1; 
    } 
    if(key == "alias") return notify_fail("That would be a bad idea.\n");
    if(key[0] == '$'){ 
        key = key[1..strlen(key)]; 
        if(Xverbs[key])  
            message("system", sprintf("Alias for $%s altered to (%s).", 
                        key, thing), this_player()); 
        else message("system", sprintf("Alias $%s (%s) added.", key, thing), 
                this_player());      
        Xverbs[key] = thing; 
    } 
    else { 
        if(Aliases[key]) 
            message("system", sprintf("Alias for %s altered to (%s).", key, thing), 
                    this_player()); 
        else message("system", sprintf("Alias %s (%s) added.", key, thing),this_player()); 
        Aliases[key] = thing; 
    } 
    return 1; 
} 

nomask static int cmd_unalias(string str){ 
    if(this_player() != this_object()) return 0; 
    if(!str){
        write("Unalias what?");
        return 1;
    }
    if(str[0] == '$'){ 
        str = str[1..strlen(str)-1]; 
        if(Xverbs[str]){ 
            map_delete(Xverbs, str); 
            message("system", sprintf("Alias $%s removed.", str), this_player()); 
            return 1;
        } 
        else message("system", sprintf("No such alias $%s.", str), this_player()); 
        return 1; 
    } 
    if(Aliases[str]){ 
        map_delete(Aliases, str); 
        message("system", sprintf("Alias %s removed.", str), this_player()); 
        return 1;
    } 
    else message("system", sprintf("No such alias %s.", str), this_player()); 
    return 1; 
} 

nomask static int cmd_cd(string str){ 
    if(this_player() != this_object()) return 0; 
    set_cwd(str); 
    return 1; 
} 

nomask static int cmd_nickname(string str){ 
    string *cles;
    string key, thing; 
    int i;

    if(this_player() != this_object()) return 0;
    if(!str){
        i = sizeof(cles = keys(Nicknames));
        while(i--){
            cles[i] = sprintf("%s%s", arrange_string(cles[i], 15),
                    Nicknames[cles[i]]);
        }
        this_player()->eventPage( cles + ({}) );
        return 1;
    }
    if(sscanf(str, "%s %s", key, thing) != 2){ 
        if(Nicknames[str]){ 
            message("system", sprintf("Nickname %s removed.", str), 
                    this_player()); 
            map_delete(Nicknames, str); 
        } 
        else message("system", sprintf("No such nickname %s.", str), 
                this_player()); 
    } 
    else { 
        if(Nicknames[key])  
            message("system", sprintf("Nickname %s altered to (%s).", key, thing), 
                    this_player()); 
        else message("system", sprintf("Nickname %s (%s) added.", key, thing), 
                this_player()); 
        Nicknames[key] = thing; 
    } 
    return 1; 
} 

nomask static int cmd_nmsh(string str){ 
    string *lines; 
    string tmp;
    int i, maxi; 

    if(!str) return 0; 
    if(this_player() != this_object()) return 0; 
    if(this_player()->GetForced()) return 0; 
    if(!(tmp = read_file(absolute_path(query_cwd(), str)))) 
        return notify_fail(sprintf("nmsh: script %s not found.\n")); 
    maxi = sizeof(lines = explode(tmp, "\n")); 
    for(i=0; i < maxi; i++){ 
        if(lines[i][0] == '#') continue; 
        if(!command(lines[i])){ 
            message("system", sprintf("nmsh: error in executing %s.", str), 
                    this_player()); 
            return 1; 
        } 
    } 
    return 1; 
} 

nomask static int cmd_pushd(string str){ 
    if(this_player() != this_object()) return 0; 
    if(!set_cwd(str)) return 0; 
    pushd(str); 
    return 1; 
} 

nomask static int cmd_popd(){ 
    if(this_player() != this_object()) return 0; 
    set_cwd(popd()); 
    return 1; 
} 

nomask static int cmd_pwd(){
    if(!query_cwd()) message("system", "No current directory.", this_object());
    else message("system", query_cwd()+":", this_object());
    return 1;
}

nomask static int cmd_work(string str){
    string *tmp;
    object ob;
    string file;
    int flag;

    if(!str || str == "") ob = environment(this_object());
    else if(str == "!"){
        flag = 1;
        ob = environment(this_object());
    }
    else if(str[0] == '!'){
        flag = 1;
        str = str[1..strlen(str)];
    }
    if(!ob && !(ob = present(str, this_object())))
        ob = present(str, environment(this_object()));
    if(!ob) return notify_fail("No target object found.\n");
    tmp = explode(file = base_name(ob), "/");
    set_cwd("/"+implode(tmp[0..sizeof(tmp)-2], "/"));
    if(flag){
        message("system", file+".c, "+file_size(file+".c")+" bytes:",
                this_object());
        this_object()->eventEdit(file+".c");
    }
    return 1;
}

varargs int erase_prompt(int x){
    if(!this_object() || this_object()->GetCedmode()) return 0;
    /* receive("\r\e[6n"); */ /* ask cursor pos */
    return receive("\e[2K\e["+GetScreen()[1]+";"+1+"H\e[2K");
}

string GetPromptString(){
    string ret = Prompt;
    if(!sizeof(ret)) return "";
    ret = replace_string(ret,"%^","%%^^");
    return ret;
}

varargs nomask string GetPrompt(int withbuff){
    string ret, ret2 = "";
    mixed tmp;
    int x, y;

    if(withbuff) ret2 = this_object()->GetCharbuffer();

    if( (y = query_ed_mode()) != -1 ){
        if( !y ){
            string n = "\n";
            if(this_object()->GetProperty("reprompt")) n = ". ";
            ret = "\tQ)uit without saving, save and ex)it, h)elp"+n+"Command: ";
        }
        else if( y == -2 ) ret = "Help: ";
        else ret = "*\b";
        return ret;
    }
    if((ret = Prompt) == DEFAULT_PROMPT){
        return ret + ret2;
    }
    if((ret = Prompt) == "cwd"){
        if(creatorp(this_object())){
            ret = query_cwd() + " > " + ret2;
        }
        else {
            ret = DEFAULT_PROMPT + ret2;
        }
        return ret;
    }
    if((ret = Prompt) == "status"){
        ret = this_object()->eventDisplayStatus(1) + " > ";
        return ret + ret2;
    }
    if(grepp(ret,"$g")) 
        ret = replace_string(ret,"$g",itoa(this_object()->GetMagicPoints()));
    if(grepp(ret,"$G")) 
        ret = replace_string(ret,"$G",itoa(this_object()->GetMaxMagicPoints()));
    if(grepp(ret,"$M"))
        ret = replace_string(ret, "$M", mud_name());
    if(grepp(ret,"$m"))
        ret = replace_string(ret, "$m", lower_case(mud_name()));
    if(grepp(ret,"$N"))
        ret = replace_string(ret,"$N",capitalize(this_object()->GetKeyName()));
    if(grepp(ret,"$n"))
        ret = replace_string(ret, "$n", GetKeyName());

    if(grepp(ret,"$V")){
        if(GetInvis())
            ret = replace_string(ret,"$V","INVIS");
        else
            ret = replace_string(ret,"$V","");
    }
    if(grepp(ret,"$P")){
        tmp = query_cwd();
        if(!tmp || !sizeof(tmp)) tmp = "No working directory.";
        ret = replace_string(ret,"$P",tmp);
    }
    if(grepp(ret,"$C")) ret = replace_string(ret,"",itoa( CmdNumber+1 ));
    if(grepp(ret,"$h")) ret = replace_string(ret,"$h",itoa( this_object()->GetHealthPoints() ));
    if(grepp(ret,"$H")) ret = replace_string(ret,"$H",itoa( this_object()->GetMaxHealthPoints() ));
    if(grepp(ret,"$i")) ret = replace_string(ret,"$i",itoa( this_object()->GetStaminaPoints() ));
    if(grepp(ret,"$I")) ret = replace_string(ret,"$I",itoa( to_int(this_object()->GetMaxStaminaPoints() )));
    if(grepp(ret,"$D")){
        string mo, yr;
        tmp = local_time();
        mo = sprintf("%02d", (tmp[4]+1));
        yr = last(sprintf("%02d", tmp[5]),2);

        if(grepp(ret,"$D0"))
            ret = replace_string(ret,"$D0", mo+"/"+tmp[3]+"/"+yr);
        if(grepp(ret,"$D1"))
            ret = replace_string(ret,"$D1", mo+"/"+tmp[3]+"/"+yr);
        if(grepp(ret,"$D2"))
            ret = replace_string(ret,"$D2", tmp[3]+"/"+mo+"/"+yr);
        if(grepp(ret,"$D"))
            ret = replace_string(ret,"$D", tmp[3]+system_month(tmp[4],1)+yr); 
    }
    if(grepp(ret,"$T")){
        string min, hr, sec, tz = this_object()->GetProperty("timezone");
        tmp = local_time(tz);
        min = sprintf("%02d", tmp[1]);
        hr = sprintf("%02d", tmp[2]);
        sec = sprintf("%02d", tmp[0]);
        ret = replace_string(ret,"$T",hr+":"+min+":"+sec);
    }
    ret += " ";
    return ret + ret2;
}

varargs nomask string write_prompt(string str){
    string ret, uncolor;
    int diff, tmp, bottom = GetScreen()[1];
    int side = GetScreen()[0];

    if(!this_object()) return "";

    if(origin() == "driver"){
        if(this_object()->GetProperty("reprompt")){
            //Avoids double pumped prompt when fancy stuff is on.
            return "";
        }
    }

    if(!Termstuff) Termstuff = ([]);
    if(!Termstuff["Terminal"]){
        Termstuff["Terminal"] = this_object()->GetTerminal();
    }
    if(!Termstuff["TermInfo"]){         
        Termstuff["TermInfo"] = 
            TERMINAL_D->query_term_info(Termstuff["Terminal"]);
    }
    if(str){
        receive(str);
        return str;
    } 
    ret = GetPrompt(this_object()->GetCharmode());
    uncolor = strip_colours(ret);
    ret = terminal_colour(ret + "%^RESET%^", Termstuff["TermInfo"]);
    tmp = sizeof(uncolor);
    if(tmp > side){
        /* Calculate which row to put the prompt on based on wrapping.
         * Without this, horrible horizontal redraw/scrolling happens.
         */
        bottom = bottom - (( tmp / side ) + ((tmp % side) ? 1 : 0 )); /* lol */
    }
    if(!this_object()->GetCedmode()){
        if(!(tmp % side)){
            /* If the line hits a wrap point, scroll up screen data */
            if(tmp == side) receive("\r\eD");
            receive("\r\eD");
        }
        receive("\r\e["+bottom+";"+1+"H");
        receive(ret);
    }
    return ret;
}

string process_input(string str){ 
    string tmp, xtra, request; 
    if(!str || str == "") return ""; 
    else if(GetClient() &&
            member_array(GetClient(), SUPPORTED_CLIENTS) != -1){
        if(sscanf(str, "<%s>%s", request, xtra)){
            process_request(request, xtra ? xtra : "");
            return "";
        }
        else return str;
    }
    else {
        tmp = eventHistory(str);
        if(tmp == ""){
            return "";     
        }
    }
    if(tmp != str) message("system", tmp, this_object());
    return do_alias(do_nickname(tmp));
} 

nomask static void process_request(string request, string xtra){
    switch(request){
        case "ALIAS":
            receive("<ALIAS>[n,go north] [s,go south] [e,go east] [w,go west] "
                    "[nw,go northwest] [ne,go northeast] [sw,go southwest] "
                    "[se,go southeast] [u,go up] [d,go down] [i,inventory] "
                    "[bio,biography] [exa,look at $*] [$',say $*] "
                    "[$:,emote $*] [l,look]\n");
        break;
        case "NICKNAME": receive("<NICKNAME>\n"); break;
        case "USERS":
            receive("<USERS>"+implode(map_array(filter(users(), 
                                "request_vis", this_object()), "user_names", this_object()), 
                        ", ")+"\n");
        break;
        case "ROOM":
            receive("<ROOM>"+
                    environment(this_object())->GetShort()+"\n");
        break;
        case "PRESENT":
            receive("<PRESENT>"+
                    implode(map_array(filter(all_inventory(environment(this_object())),
                                "request_vis", this_object()), "user_names", this_object()),
                        ", ")+"\n");
        break;
        default:
        receive("<error>Request not supported.\n");
        break;
    }
}

static int request_vis(object ob){
    return (userp(ob) && !(ob->GetInvis(this_object())));
}

static string user_names(object ob){
    return ob->GetName();
}

private static int set_cwd(string str){ 
    int x;
    string tmpstr = str;
    if(str == "~-" || str == "-") str = Directories["previous"];
    if(!str || str == "") str = user_path(GetKeyName()); 
    if (str[<1] == '/' && str != "/") str = str[0..<2];
    replace_string(str, "//", "/"); 
    str = absolute_path(query_cwd(), str);
    if(!directory_exists(str) && tmpstr == "here" && environment(this_player())){
        str = path_prefix(base_name(environment(this_player())));
    } 
    if((x=file_size(str)) != -2){ 
        if(x > -1){ 
            message("system", sprintf("%s: Path is a file.", str), this_player()); 
            return 0; 
        } 
        else { 
            message("system", sprintf("%s: No such reference.", str), this_player()); 
            return 0; 
        }  
    } 

    if(str != query_cwd()) Directories["previous"] = query_cwd();
    Directories["current"] = str; 
    message("system", sprintf("%s:", Directories["current"]), this_player()); 
    return 1; 
} 

string GetUserPath(){
    return Directories["home"];
}

string SetUserPath(string str){
    if(this_player() != this_object()) return 0;
    return Directories["home"] = str; 
}

private static void pushd(string str){ 
    if(CWDCount++ == DIRECTORY_STACK_SIZE){ 
        CWDCount--; 
        CWDBottom = (++CWDBottom) % DIRECTORY_STACK_SIZE; 
    } 
    Stack[CWDTop] = str; 
    CWDTop = (++CWDTop) % DIRECTORY_STACK_SIZE; 
} 

private static string popd(){ 
    if(!CWDCount) return 0; 
    CWDCount--; 
    return Stack[--CWDTop]; 
} 

nomask private static string do_nickname(string str){ 
    if(!Nicknames) return str; 
    if(str[0..7] == "nickname") return str; 
    return implode(map_array(explode(str, " "), "replace_nickname", this_object()), " "); 
} 

nomask private static string do_alias(string str){ 
    string *words; 
    string tmp, ret; 
    int x; 

    if(!sizeof(words = explode(str, " "))) return "";
    if((x = strlen(words[0])) && (tmp = Xverbs[words[0][0..0]])){
        words[0] = words[0][1..x-1];
        return replace_string(tmp, "$*", implode(words, " "));
    }
    if(!(tmp = Aliases[words[0]])) return implode(words, " "); 
    else str = implode(words[1..sizeof(words)-1], " "); 
    ret = replace_string(tmp, "$*", str);
    return ret;
} 

string GetAlias(string alias){
    string ret = Aliases[alias];
    if(!this_player() || this_player() != this_object() ||
            this_player()->GetForced()) return 0;
    else return ret;
}

string GetXverb(string xverb){
    string ret = Xverbs[xverb];
    if(!this_player() || this_player() != this_object() ||
            this_player()->GetForced()) return 0;
    else return ret; 
} 

nomask static string replace_nickname(string str){ 
    if(str == "") return str; 
    if(str[0] == '\\') return str[1..(strlen(str)-1)]; 
    else if(Nicknames[str]) return Nicknames[str]; 
    else return str; 
} 

void reset_prompt(){ 
    if(!stringp(Prompt)) Prompt = "> ";
} 

string query_cwd(){ return Directories["current"]; } 

string query_prev_wd(){ return Directories["previous"]; } 

string SetPrompt(string str){ return Prompt = str; }

int query_mp(){ return 1; } 

int query_max_mp(){ return 10; } 

int query_hp(){ return 1; } 

int query_max_hp(){ return 10; } 

int query_sp(){ return 1; } 

int query_max_sp(){ return 10; } 

string get_path(){ return query_cwd(); }

varargs int GetInvis(){ return 0; }

string GetKeyName(){ return 0; }

#if FAIL
string fail(){
    string fale;
    fale += "";
    write("lol fail");
    return fale;
}
#endif

static void EchoCommand(string str){
    mixed clr, ret;
    if(clr = (this_object()->GetProperty("commandecho"))){
        clr = lower_case(clr);
        if(atoi(clr) || clr == "on") clr = "reset";
        ret = "%^"+upper_case(clr)+"%^"+str+"%^RESET%^";
        write(ret);
    }
} 

mixed RecalculateHist(int x){
    mapping command_hist = this_object()->GetHistoryList();
    string charbuffer = this_object()->GetCharbuffer();
    int histsize = sizeof(command_hist);
    if(!histsize){
        command_hist = ([ 0 : "" ]);
        histsize = 1;
    }
    if(histmatch > 0 && sizeof(charbuffer) && !sizeof(recalled_command_sub)){
        mixed tmpvals;
        tmpvals = filter(values(command_hist), 
                (: $1 && !strsrch($1, this_object()->GetCharbuffer()) :));
        if(sizeof(tmpvals)){
            recalled_command_sub = charbuffer;
        }
    }
    if(x) recalled_command_num++;
    else recalled_command_num--;
    if(recalled_command_num < 0) recalled_command_num = (histsize - 1);
    if(recalled_command_num > (histsize - 1)) recalled_command_num = 0;
    recalled_command = command_hist[recalled_command_num];
    if(sizeof(recalled_command_sub) && 
            strsrch(recalled_command, recalled_command_sub)){
        int hit = 0;
        while(!hit){
            if(x) recalled_command_num++;
            else recalled_command_num--;
            if(recalled_command_num < 0) recalled_command_num = (histsize - 1);
            if(recalled_command_num > (histsize - 1)) recalled_command_num = 0;
            if(!strsrch(command_hist[recalled_command_num], 
                        recalled_command_sub)){
                hit = 1;
                recalled_command = command_hist[recalled_command_num];
            }
        }
    } 
    return recalled_command;
}

static int rBackspace(){
    string cb = this_object()->GetCharbuffer();
    if(!sizeof(cb)) return 0;
    recalled_command_sub = "";
    cb = truncate(cb, 1);
    this_object()->SetCharbuffer(cb);
    histmatch = 0;
    erase_prompt();
    write_prompt();
    return 1;
}

static int rDel(){
    histmatch = 0;
    erase_prompt();
    write_prompt();
    return 1;
}

static int rEnter(){
    string charbuffer = this_object()->GetCharbuffer();
    string tmp;
    this_object()->SetNoEcho(0);
    /* re-add any removed pinkfish */
    charbuffer = replace_string(charbuffer,"%%^^","%^");
    EchoCommand(charbuffer);

    /* This next thing is probably a bit obscure, so I'll explain. We're
     * about to save the line that we just received into the 
     * character buffer called "tempbuffer" stored in LIB_CHARIO.
     * The idea is to have "what I just typed" available somewhere before
     * it is expanded from its alias. This allows for up-arrow history
     * recall matching that keys not on the expanded command, but what
     * I really just typed, allowing for selective/matched recall of
     * aliases and add_actions. See the Push() call from LIB_COMMAND 
     * to LIB_HISTORY.
     *
     * And to you it was just an arbitrary line of code.
     *
     * Yeah. I'm that awesome.
     */
    this_object()->SetTempbuffer(charbuffer);

    tmp = process_input(charbuffer);
    this_object()->SetCharbuffer("");
    recalled_command_sub = "";
    histmatch = 0;
    if(sizeof(tmp)){
        if(!command(tmp)){
            this_object()->cmdAll(tmp);
            parse_sentence(tmp);
        }
    }
    else {
        write(""); /* Move up a line */
    }
    recalled_command_num = 0;
    erase_prompt();
    write_prompt();
}

static int rCtrl(string str){
    string charbuffer = this_object()->GetCharbuffer();
    if(str == "d"){ /* Ctrl-D */
        write("Canceling charmode!");
        this_object()->CancelCharmode();
        erase_prompt();
        write_prompt();
    }
    else if(str == "c"){ /* Ctrl-C */
        this_object()->SetCharbuffer("");
        recalled_command_sub = "";
        recalled_command_num = 0;
        erase_prompt();
        write_prompt();
    }
    if(str == "i" ){ /* Taberino */
        mixed cmds;
        if(!sizeof(charbuffer)){
            /* should anything happen here? */
        }
        if(grepp(charbuffer, " ")){
            string arg1, arg2, lastarg, cmd, tmp_str;
            string *commands, *tmp_arr, *ret_arr = ({});
            int i;
            object ob, env, *ob_arr;
            /* An action has been typed and a space after it
             * tells us the user wants us to figure out the 
             * argument. Let's first chew on isolating the action.
             */
            i = sscanf(charbuffer, "%s %s", arg1, arg2); 
            if(i != 2){
                cmd = trim(charbuffer);
            }
            else {
                cmd = arg1;
                arg2 = trim(arg2);
            }
            if(sizeof(arg2)){
                lastarg = trim(last_string_element(charbuffer, " "));
                charbuffer = trim(replace_string(charbuffer, lastarg, ""));
            }
            cmds = match_command(cmd, 1);
            if(sizeof(cmds) == 1){ /* we want an unambiguous match */
                cmd = cmds[0];
                commands = ({});
                foreach(mixed path in this_player()->GetSearchPath()){
                    commands += CMD_D->GetCommands(path);
                }
                if(cmd && member_array(cmd, commands) != -1){
                    string *file_cmds, *dir_cmds, *both_cmds;
                    int abso;
                    /* It's a command! Let's see if it's a file
                     * manipulating kind of command.
                     */
                    file_cmds = ({ "ced", "clone", "goto", "rehash", "reset",
                            "showtree", "bk", "cat", "cp", "diff", "ed", 
                            "grep", "head", "indent", "longcat", "more", "mv",
                            "rm", "sed", "showfuns", "source", "tail", 
                            "update" });
                    dir_cmds = ({ "cd", "ls", "mkdir", "rmdir" });
                    both_cmds = file_cmds + dir_cmds;

                    if(member_array(cmd, both_cmds) != -1 ){
                        string pre, post;
                        string *candidates = ({});
                        string my_path = query_cwd();
                        string clr = "%^GREEN%^";
                        string rst = "%^RESET%^";

                        /* Indeed a filesystem command. If there
                         * is an argument to work with, let's try
                         * to use it to guess which file/dir to
                         * manipulate.
                         */
                        if(lastarg){
                            if(lastarg[0..0] == "/"){ 
                                pre = path_prefix(lastarg);
                                abso = 1;
                            }
#if 0
                            if(lastarg[0..2] == "../"){
                                lastarg = my_path + "/";
                                pre = path_prefix(my_path) + "/";
                                abso = 1;
                            }
#endif
                            else {
                                pre = path_prefix(my_path+"/"+lastarg);
                            }
                            post = last_string_element(lastarg, "/");
                        }
                        else {
                            pre = my_path;
                        }
                        if(!sizeof(post)) post = lastarg;
                        candidates = get_dir(pre + "/");
                        if(sizeof(post) && sizeof(candidates)){
                            candidates = regexp(candidates, "^"+post);
                        }
                        if(sizeof(candidates) == 1){
                            string tmplast;
                            post = last_string_element(lastarg, "/");
                            if(grepp(lastarg,"/")){
                                tmplast = replace_string(candidates[0],
                                        post, "", 1);
                            }
                            else {
                                tmplast = replace_string(candidates[0],
                                        lastarg, "", 1);
                            }
                            lastarg += tmplast;
                        }
                        else if(sizeof(candidates) > 1){
                            mapping Matches = ([]);
                            tmp_str = "";
                            foreach(string thing in candidates){
                                if(sizeof(thing) > sizeof(tmp_str)){
                                    tmp_str = thing;
                                }
                                if(directory_exists(pre+"/"+thing)){
                                    ret_arr += ({ clr+thing+rst+"/" });
                                }
                                else ret_arr += ({ thing });
                            }
                            foreach(string thing in candidates){
                                int fail;
                                if(thing == tmp_str) continue;
                                for(int j=0; j < sizeof(thing)-1; j++){
                                    if(thing[j] != tmp_str[j]) fail = 1;
                                    if(!fail) Matches[j] = thing;
                                }
                            }
                            if(sizeof(Matches)){
                                i = sort_array(keys(Matches), -1)[0];
                                tmp_str = Matches[i][0..i];
                            }
                            if(sizeof(lastarg) && sizeof(tmp_str)) lastarg = tmp_str;
                            if(abso && sizeof(lastarg) &&
                                    lastarg[0..0] != "/"){
                                lastarg = "/"+lastarg;
                            }
                            write(format_page2(ret_arr, 4));
                        }
                        if(!lastarg) lastarg = "";
                        if(grepp(lastarg,"/")){
                            lastarg = last_string_element(lastarg, "/");
                        }
                        tmp_str = pre+"/"+lastarg+"/";
                        tmp_str = replace_string(tmp_str, "//", "/");
                        tmp_str = replace_string(tmp_str, "//", "/");
                        if(directory_exists(tmp_str) && !sizeof(ret_arr)){
                            if(abso) lastarg = tmp_str;
                            else {
                                lastarg = replace_string(tmp_str,
                                        query_cwd()+"/", "");
                            }
                            tmp_arr = get_dir(tmp_str);
                            foreach(string thing in tmp_arr){
                                if(directory_exists(tmp_str+thing)){
                                    ret_arr += ({ clr+thing+rst+"/" });
                                }
                                else ret_arr += ({ thing });
                            }
                            if(sizeof(lastarg) && last(lastarg,1) != "/"){
                                lastarg += "/";
                            }
                            write(format_page2(ret_arr, 4));
                        }
                        else if(sizeof(tmp_str) && last(tmp_str,1) == "/"){
                            lastarg = truncate(tmp_str, 1);
                        }
                        if(abso && sizeof(lastarg) && lastarg[0..0] != "/"){
                            lastarg = "/"+lastarg;
                        }

                        if(sizeof(lastarg)) charbuffer += " "+lastarg;
                        else charbuffer = trim(charbuffer) + " ";
                    } /* end filesystem command */
                } /* end command handler */
                this_object()->SetCharbuffer(charbuffer);
                if(VERBS_D->GetVerb(cmd) &&
                        creatorp(this_object())){
                    /* It's a verb, so the arg otta be an object.
                     * We won't be doing "object name completion"
                     * for players since that's a cheat vector.
                     */
                    if(sizeof(lastarg)){
                        if(!ob && (env = environment(this_object()))){
                            ob_arr = all_inventory(env) +
                                all_inventory(this_object());
                            ob_arr = filter(ob_arr,
                                    (: answers_to( $(lastarg), $1) :) );
                        }
                        else tmp_arr = regexp(ob->GetCanonicalId(),
                                "^"+lastarg);
                        if(!sizeof(tmp_arr) && sizeof(ob_arr) == 1){
                            tmp_arr = regexp(ob_arr[0]->GetCanonicalId(),
                                    "^"+lastarg);
                        }
                        if(sizeof(tmp_arr)) tmp_str = tmp_arr[0];
                        if(!tmp_str && sizeof(ob_arr) > 1){
                            foreach(mixed thing in ob_arr){
                                tmp_arr = regexp(thing->GetCanonicalId(),
                                        "^"+lastarg);
                                if(sizeof(tmp_arr)){
                                    tmp_str = tmp_arr[0];
                                    tmp_str += " " + file_name(thing) + " ";
                                    tmp_str += thing->GetKeyName();
                                    ret_arr += ({ tmp_str });
                                }
                            }                                 
                        }
                        else if(tmp_str){
                            lastarg = tmp_str;
                        }
                        if(sizeof(ret_arr) && tmp_str != lastarg){
                            foreach(string thtring in
                                    sort_array(ret_arr,1)){
                                write(thtring);
                            }
                        }
                        if(lastarg) charbuffer += " " + lastarg;
                    } /* end available argument for the verb */
                } /* end verb handler */
            } /* end unambiguous command */
        } /* end arg detection */
        else { 
            /* A clean fallthrough would be nice here :( */
        }
        cmds = match_command(charbuffer,1);
        if(!sizeof(cmds)){
            this_object()->SetCharbuffer(charbuffer);
        }
        else if(sizeof(cmds) == 1){
            charbuffer = cmds[0];
            this_object()->SetCharbuffer(charbuffer);
            erase_prompt();
            write_prompt();
        }
        else {
            write(identify(sort_array(cmds,1)));
        }
        erase_prompt();
        write_prompt();
    }
    return 1;
}

static int rArrow(string str){
    string charbuffer;
    if(!str) return 0;
    switch(str){
        case "up" :
            charbuffer = RecalculateHist(0);
        histmatch = -1;
        this_object()->SetCharbuffer(charbuffer);
        erase_prompt();
        write_prompt();
        break;
        case "down" :
            charbuffer = RecalculateHist(1);
        histmatch = -1;
        this_object()->SetCharbuffer(charbuffer);
        erase_prompt();
        write_prompt();
        break;
        case "left" :
            recalled_command_sub = "";
#if CMD_EDITING
        this_object()->eventReceive("\e[D", 1, 1);
        this_object()->eventReceive("\e[6n", 1, 1);
#endif
        break;
        case "right" :
            recalled_command_sub = "";
#if CMD_EDITING
        this_object()->eventReceive("\e[C", 1, 1);
        this_object()->eventReceive("\e[6n", 1, 1);
#endif
        break;
    }
    return 0;
}

static int rAscii(string str){
    if(!histmatch) histmatch = 1;
    erase_prompt();
    write_prompt();
    return 1;
}

static int rAnsi(string str){
    return 1;
}
