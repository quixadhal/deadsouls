#include <objects.h>
#include <privs.h>
#include <rooms.h>
#include <cgi.h>
#include <commands.h>
#include <save.h>
#include <lib.h>
#include <cfg.h>
#include <daemons.h>
#include <dirs.h>
#ifndef AUTO_ADVANCE
#define AUTO_ADVANCE 0
#endif
#ifndef REQUIRE_QUESTING
#define REQUIRE_QUESTING 1
#endif
#ifndef ENABLE_INSTANCES
#define ENABLE_INSTANCES 0
#endif

inherit LIB_DAEMON;

mapping PlayerDataMap = ([]);
mapping UserData = ([]);
string *PendingEncres = ({});
string *PendingDecres = ({});
string *PendingPauses = ({});
string *PendingUnpauses = ({});
string *players = ({});
string *creators = ({});
string *user_list = ({});
static object ob;
static string gplayer, SaveFile;
static int maxlevel, override;
static string home_dir, LevelList = "";

string player_save_file;
static string namestr = "";

//Player vars
string InternalDesc, Short, CapName, ExternalDesc, CurrentWorkingDirectory, PreviousWorkingDirectory, Terminal;
string Password, Email, RealName, Rank, LoginSite, HostSite, Torso, DefaultLanguage, Town, Race;
string Gender, Class, WimpyCommand, InternalDesc, WebPage, Clan;
int ProperNoun, Opacity, Modify, Invisible, NoChanColors, Paused, MaxCommandHistSize, HistorySize;
int Age, WhereBlock, Brief, LoginTime, BirthTime, RescueBit, CarriedMass, MaxCarry, Position;
int  Undead, UndeadType, Mount, BodyMass, SaveRecurse, Retain, HealthPoints, MagicPoints, ExperiencePoints;
int QuestPoints, melee, Alcohol, Caffeine, Food, Drink, Poison, Sleeping, DeathEvents, Biter;
int Blind, CustomStats, Wimpy, Polyglot, Level, Morality, Dead, isPK, Deaths, TrainingPoints;
int TitleLength, CreatorAge, CreatorBirth;
object Agent;
float StaminaPoints;
mixed array Inventory, CommandHist, Screen,  Marriages, Saved, ExtraChannels, LightSensitivity;
mixed array Religion, Titles, Muffed, Quests; 
mapping Items, Properties, Messages, RestrictedChannels, Nicknames, Aliases, Xverbs, Blocked;
mapping News, Fingers, Limbs, MissingLimbs, Resistance, Stats, Languages, Skills, SkillModifiers;
mapping Currency, Bank, SpellBook; 
//end player vars

mapping Levels = ([]);
static mapping LevelTitles = ([
        1:"the utter novice",
        2:"the simple novice",
        3:"the beginner",
        4:({"the adventurer","the adventuress"}),
        5:({"the experienced adventurer","the experienced adventuress"}),
        6:({"the expert adventurer","the expert adventuress"}),
        7:({"the great adventurer","the great adventuress"}),
        8:({"the master adventurer","the master adventuress"}),
        9:({"the Freeman","the Freewoman"}),
        10:"the Citizen",
        11:"the Knight",
        12:({"the Baron","the Baroness"}),
        13:({"the Count","the Viscountess"}),
        14:({"the Earl","the Countess"}),
        15:({"the Marquis","the Marquise"}),
        16:({"the Duke","the Duchess"}),
        17:({"the Archduke","the Archduchess"}),
        18:({"the Praetor","the Praetrix"}),
        19:({"the Quaestor","the Quaestrix"}),
        20:({"the Caesar","the Caesara"}),
        ]);

static mapping QuestLevels = ([
        10:5,
        12:12,
        14:21,
        16:32,
        18:45,
        20:60,
        22:77,
        24:96,
        26:117,
        28:140,
        30:165,
        32:192,
        34:221,
        36:252,
        38:285,
        40:450,
        ]);


void validate(){
    if(!master()->valid_apply(({ "SECURE", "ASSIST", "LIB_CONNECT" })) &&
            base_name(previous_object()) != CGI_LOGIN && !override &&
            base_name(previous_object()) != CMD_RID &&
            base_name(previous_object()) != CMD_SUICIDE){
        string offender = identify(previous_object(-1));
        debug("PLAYERS_D SECURITY VIOLATION: "+offender+" ",get_stack(),"red");
        log_file("security", "\n"+timestamp()+" PLAYERS_D breach: "+offender+" "+get_stack());
        error("PLAYERS_D SECURITY VIOLATION: "+offender+" "+get_stack());
    }
    override = 0;
}

// This function generates a table of required xp per level.
// The xp required for lower levels is high, approximately
// half-again the xp required for the previous level. As the
// player progresses, this ratio decreases.
mapping CompileLevelList(){
    int i=1;
    mixed title;
    int seed=300;
    float mod;
    LevelList = "";
    Levels = ([ 0 : ([ "xp" : 0, "qp" : 0 ]) ]);
    Levels[1] = ([ "title" : "the utter novice ", "xp" : 0, "qp" : 0 ]);
    while(seed > 0){
        i++;
        mod = 50/i;
        if(mod > 9) mod = 9.0;
        if(mod < 1) mod = 0.5;
        if(i > 100) mod = 0.1;
        mod *= 0.1;
        seed = seed * (1+mod); 
        seed = ((seed/100) * 100);
        LevelList += "level: "+i+", ";
        if(seed > 0){
            Levels[i] = (["xp" : seed ]);
            if(REQUIRE_QUESTING){
                if(QuestLevels[i]){
                    Levels[i]["qp"] = QuestLevels[i];
                }
            }
            else Levels[i]["qp"] = 0;
            if(title = LevelTitles[i]){
                string line;
                if(stringp(title)) line = title;
                else line = implode(title, "/");
                Levels[i]["title"] = line;
            }
            maxlevel = i;
            LevelList += ( Levels[i]["title"] || "Untitled" );
            LevelList += ", exp: "+seed;
            LevelList += ( Levels[i]["qp"] ? ", qp: "+Levels[i]["qp"] : "");
            LevelList += "\n";
        }
        if(seed >= 2146000000) break;
    }
    return copy(Levels);
}

static void ScheduledPlayerAdd(string *plays){
    foreach(string play in plays){
        this_object()->AddPlayerInfo(play);
    }
}

string *CompileCreList(){
    string *cre_dirs = unguarded( (: get_dir(DIR_CRES+"/") :) );
    string *cres = ({});
    string *cache = ({});
    int i;
    creators = ({});
    if(!user_list) user_list = ({});
    foreach(string subdir in cre_dirs){
        cres += unguarded( (: get_dir(DIR_CRES+"/"+$(subdir)+"/") :) );
    }
#if ENABLE_INSTANCES
    cres = filter(cres, (: grepp($1, ""+__PORT__) :) );
#else
    cres = filter(cres, (: sscanf($1, "%*s.%*s.o") == 1 :) );
#endif
    foreach(string cre in cres){
        string sub;
#if ENABLE_INSTANCES
        sub = replace_string(cre, "."+__PORT__+".o", "");
        sub = replace_string(sub, ".o","");
#else
        sub = replace_string(cre, ".o","");
#endif
        if(sub && !grepp(sub, ".") && 
                (member_array(sub, user_list) == -1 ||
                 member_array(sub, creators) == -1)){
            cache += ({ sub });
        }

        if(sizeof(cache) > 20){
            call_out("ScheduledPlayerAdd", i, cache);
            cache = ({});
        }
    }
    if(sizeof(cache)){
        call_out("ScheduledPlayerAdd", i, cache);
    }
    return cres;
}

string *CompilePlayerList(){
    string *play_dirs = unguarded( (: get_dir(DIR_PLAYERS+"/") :) );
    string *plays = ({});
    string *cache = ({});
    int i;
    players = ({});
    if(!user_list) user_list = ({});
    foreach(string subdir in play_dirs){
        mixed tmp_plays = unguarded((: get_dir,DIR_PLAYERS+"/"+subdir+"/" :));
        if(tmp_plays) plays += tmp_plays;
    }
#if ENABLE_INSTANCES
    plays = filter(plays, (: grepp($1, ""+__PORT__) :) );
#else
    plays = filter(plays, (: sscanf($1, "%*s.%*s.o") == 1 :) );
#endif
    foreach(string play in plays){
        string sub;
#if ENABLE_INSTANCES
        sub = replace_string(play, "."+__PORT__+".o", "");
        sub = replace_string(sub, ".o","");
#else
        sub = replace_string(play, ".o","");
#endif
        if(sub && !grepp(sub, ".") && 
                (member_array(sub, user_list) == -1 ||
                 member_array(sub, players) == -1)){
            cache += ({ sub });
        }

        if(sizeof(cache) > 20){
            call_out("ScheduledPlayerAdd", i, cache);
            cache = ({});
        }
    }
    if(sizeof(cache)){
        call_out("ScheduledPlayerAdd", i, cache);
    }
    return plays;
}

void create() {
    int ret;
    ::create();
    SaveFile = save_file(SAVE_PLAYER_LIST);
    ret = RestoreObject(SaveFile);
    if(PendingEncres) PendingEncres = distinct_array(PendingEncres);
    if(PendingDecres) PendingDecres = distinct_array(PendingDecres);
    if(PendingPauses) PendingPauses = distinct_array(PendingPauses);
    if(PendingUnpauses) PendingUnpauses = distinct_array(PendingUnpauses);
    if(players) players = distinct_array(players);
    else players = ({});
    if(creators) creators = distinct_array(creators);
    else creators = ({});
    if(user_list) user_list = distinct_array(user_list);
    else user_list = ({});
    SaveObject(SaveFile);
    if(!Levels) Levels = ([]);
    if(!UserData) UserData = ([]);
    call_out( (: CompileLevelList :), 3);
    call_out( (: CompileCreList :), 1);
    call_out( (: CompilePlayerList :), 5);
    call_out( "SelektUsers", 604800, 1 );
}

string *eventCre(string str){
    str = lower_case(cleaned_name(str));
    if(member_array(str,creators) == -1) creators += ({ str });
    if(member_array(str,players) != -1) players -= ({ str });
    SaveObject(SaveFile);
    return copy(creators);
}

string *eventDecre(string str){
    str = lower_case(str);
    if(member_array(str,creators) != -1) creators -= ({ str });
    if(member_array(str,players) == -1) players += ({ str });
    SaveObject(SaveFile);
    return players + ({});
}

static int AutoAdvance(object ob, int level){
    int ret;
    ob->ChangeLevel(level);
    ret = ob->GetLevel();
    if(ret >= level){
        ob->eventPrint("%^RED%^%^B_BLACK%^You automatically advance to "+
                "level "+level+". Congratulations!%^RESET%^");
        ob->AddTrainingPoints(level);
        if(LevelTitles[level]){
            ob->AddTitle(LevelTitles[level]);
            ob->RemoveTitle(LevelTitles[level-1]);
        }
    }
    return ret;
}

int CheckAdvance(object ob){
    int dlev, xp, qp;
    if(!ob || !playerp(ob)) return 0;
    if(!sizeof(Levels)) CompileLevelList();
    dlev = (ob->GetLevel())+1;
    if(!Levels[dlev]) return 0;
    xp = ob->GetExperiencePoints();
    qp = ob->GetQuestPoints();
    if(xp >= Levels[dlev]["xp"] && qp >= Levels[dlev]["qp"]){
        if(AUTO_ADVANCE) AutoAdvance(ob, dlev);
        return 1;
    }
    return 0;
}

void AddPlayerInfo(mixed arg){
    if(!objectp(arg) && !stringp(arg)){
        return ;
    }
    if(!creators) creators = ({});
    if(!players) players = ({});
    if(!user_list) user_list = ({});
    if(objectp(arg)){
        if(base_name(previous_object())!=LIB_CONNECT &&!interactive(arg)){
            return;
        }
        else player_save_file = base_name(arg)+".o";
    }
    else {
        arg = lower_case(arg);
        if(last(arg,2) != ".o") arg += ".o";
        if(!grepp(arg,"/")){
            string initial = arg[0..0];
            if(!directory_exists(DIR_ESTATES))
                mkdir(DIR_ESTATES);
            if(!directory_exists(DIR_ESTATES+"/"+initial))
                mkdir(DIR_ESTATES + "/"+initial);
            mkdir(DIR_ESTATES + "/"+initial+"/"+truncate(arg,2));
            mkdir(DIR_ESTATES + "/"+initial+"/"+truncate(arg,2)+"/tmp");
            mkdir(DIR_ESTATES + "/"+initial+"/"+truncate(arg,2)+"/log");
            mkdir(DIR_ESTATES + "/"+initial+"/"+truncate(arg,2)+"/bak");
            mkdir(DIR_ESTATES + "/"+initial+"/"+truncate(arg,2)+"/adm");
            player_save_file = DIR_CRES +"/"+initial+"/"+arg;
#if ENABLE_INSTANCES
            player_save_file = new_savename(player_save_file);
#endif
            if(!file_exists(player_save_file))
                player_save_file = DIR_PLAYERS +"/"+initial+"/"+arg;
#if ENABLE_INSTANCES
            player_save_file = new_savename(player_save_file);
#endif
            if(!file_exists(player_save_file)){
                return;
            }
        }
        else player_save_file = arg;
    }
#if ENABLE_INSTANCES
    player_save_file = old_savename(player_save_file);
    player_save_file = new_savename(player_save_file);
    namestr = replace_string(last_string_element(player_save_file,"/"),
            "."+__PORT__+".o","");
#else
    namestr = replace_string(last_string_element(player_save_file,"/"),".o","");
#endif
    namestr = cleaned_name(namestr);
    if(grepp(player_save_file, DIR_CRES) && 
            member_array(namestr,creators) == -1){ 
        creators += ({ namestr }) ; 
    }
    else if(member_array(namestr,players) == -1){ 
        players += ({ namestr }) ; 
    }
    if(!unguarded( (: file_exists, player_save_file :) )){
        return;
    }
    if(member_array(namestr,user_list) == -1){
        user_list += ({ namestr }) ; 
    }
    SaveObject(SaveFile);
}

string *GetPlayerList(){
    return players + ({});
}

string *GetCreatorList(){    
    return copy(creators);
}

string *GetUserList(){
    return copy(user_list);
}

mapping GetLevelList(){
    if(!sizeof(Levels)) CompileLevelList();
    return copy(Levels);
}

string GetLevels(){
    if(!sizeof(LevelList)) CompileLevelList();
    return LevelList;
}

int RemoveUser(string str){
    string sfile, targetdir;
    mixed purge_array;
    object ob;
    validate();
    if(!str || str == "") return 0;
    str = lower_case(str);
    home_dir = homedir(str);
    if( member_group(str, PRIV_SECURE) || member_group(str, PRIV_ASSIST) ){
        return 0;
    }
    debug("REMOVE USER REQUEST: "+str, "red");
    if( ob = find_player(str) ) {
        message("system", "You are being ridded from " + mud_name() + ".",
                ob);
        if( !(ob->eventDestruct()) ) destruct(ob);
    }
    purge_array = filter(objects(),(: !strsrch(base_name($1), home_dir) :));
    foreach(object tainted in purge_array){
        if(clonep(tainted)){
            catch(tainted->eventMove(ROOM_FURNACE));
            purge_array -= ({ tainted });
        }
    }
    foreach(object tainted in purge_array){
        catch(tainted->eventDestruct());
        if(tainted) catch(destruct(tainted));
    }
    if(member_array(str, players) != -1) players -= ({ str });
    if(member_array(str, creators) != -1) creators -= ({ str });
    if(member_array(str, user_list) != -1) user_list -= ({ str });
    targetdir = DIR_RID + "/" + str[0..0];
    if(directory_exists(targetdir+"/"+str)){
        rename(targetdir+"/"+str, targetdir+"/"+str+"."+time());
    }
    mkdir_recurse(targetdir+"/"+str);
    sfile = player_save_file(str);
    if(file_exists(sfile)){
        rename(sfile, targetdir+"/"+str+"/"+str+".bak");
    }
    sfile = REALMS_DIRS + "/" + str;
    if(directory_exists(sfile)) rename(sfile, targetdir+"/"+str+"/realm");
    sfile = ESTATES_DIRS + "/" + str[0..0] + "/" + str;
    if(directory_exists(sfile)) rename(sfile, targetdir+"/"+str+"/estate");
    log_file("rid", "\n" + str + 
            " by PLAYERS_D as a trivial/unused account.\n");
    return 1;
}

string *GetPendingPauses(){
    validate();
    if(!PendingPauses) PendingPauses = ({});
    return copy(PendingPauses);
}

string *GetPendingUnpauses(){
    validate();
    if(!PendingUnpauses) PendingUnpauses = ({});
    return copy(PendingUnpauses);
}

string *AddPendingPause(string str){
    validate();
    if(!PendingPauses) PendingPauses = ({});
    if(str && str != "") PendingPauses += ({ lower_case(str) });
    SaveObject(SaveFile);
    return copy(PendingPauses);
}

string *RemovePendingPause(string str){
    validate();
    if(!PendingPauses) PendingPauses = ({});
    if(!str || str == "") return copy(PendingPauses);
    str = lower_case(str);
    if(member_array(str, PendingPauses) != -1){
        PendingPauses -= ({ lower_case(str) });
    }
    SaveObject(SaveFile);
    return copy(PendingPauses);
}

string *AddPendingUnpause(string str){
    validate();
    if(!PendingUnpauses) PendingUnpauses = ({});
    if(str && str != "") PendingUnpauses += ({ lower_case(str) });
    SaveObject(SaveFile);
    return copy(PendingUnpauses);
}

string *RemovePendingUnpause(string str){
    validate();
    if(!PendingUnpauses) PendingUnpauses = ({});
    if(!str || str == "") return copy(PendingUnpauses);
    str = lower_case(str);
    if(member_array(str, PendingUnpauses) != -1){
        PendingUnpauses -= ({ lower_case(str) });
    }
    SaveObject(SaveFile);
    return copy(PendingUnpauses);
}

string *AddPendingEncre(string str){
    validate();
    if(!PendingEncres) PendingEncres = ({});
    if(str && str != "") PendingEncres += ({ lower_case(str) });
    SaveObject(SaveFile);
    return copy(PendingEncres);
}

string *RemovePendingEncre(string str){
    validate();
    if(!PendingEncres) PendingEncres = ({});
    if(!str || str == "") return copy(PendingEncres);
    str = lower_case(str);
    if(member_array(str, PendingEncres) != -1) PendingEncres -= ({ lower_case(str) });
    SaveObject(SaveFile);
    return copy(PendingEncres);
}

string *GetPendingEncres(){
    validate();
    if(!PendingEncres) PendingEncres = ({});
    return copy(PendingEncres);
}

string *AddPendingDecre(string str){
    validate();
    if(!PendingDecres) PendingDecres = ({});
    if(str && str != "") PendingDecres += ({ lower_case(str) });
    SaveObject(SaveFile);
    return copy(PendingDecres);
}

string *RemovePendingDecre(string str){
    validate();
    if(!PendingDecres) PendingDecres = ({});
    if(!str || str == "") return copy(PendingDecres);
    str = lower_case(str);
    if(member_array(str, PendingDecres) != -1) PendingDecres -= ({ lower_case(str) });
    SaveObject(SaveFile);
    return copy(PendingDecres);
}

string *GetPendingDecres(){
    validate();
    if(!PendingDecres) PendingDecres = ({});
    return copy(PendingDecres);
}

static int LoadPlayer(string str){
    string arg = "", initial = "";
    arg = last_string_element(lower_case(replace_string("/"+str,".o","")),"/");
#if ENABLE_INSTANCES
    arg = replace_string(arg, "."+__PORT__, "");
#endif
    if(!user_exists(arg)){
        return 0;
    }
    arg += ".o";
    initial = arg[0..0];
    player_save_file = DIR_CRES +"/"+initial+"/"+arg;
#if ENABLE_INSTANCES
    player_save_file = new_savename(player_save_file);
#endif
    if(!file_exists(player_save_file)){
        player_save_file = DIR_PLAYERS +"/"+initial+"/"+arg;
#if ENABLE_INSTANCES
        player_save_file = new_savename(player_save_file);
        if(!file_exists(player_save_file)){
            player_save_file = old_savename(player_save_file);
        }
#endif
    }
    if(!file_exists(player_save_file)){
        return -1;
    }
    RestoreObject(player_save_file,1);
    return 1;
}

int eventDestruct(){
    validate();
    SaveObject(SaveFile);
    return ::eventDestruct();
}

static mixed GetVariable(string val){
    if(!val){
        return variables(this_object());
    }
    if(member_array(val,variables(this_object())) == -1){
        write("No such PLAYER_D variable exists.");
        return 0;
    }
    return fetch_variable(val);
}

mixed GetPlayerVariables(){
    string *vars = variables(this_object());
    vars -= ({"PlayerDataMap", "PendingEncres", "PendingDecres"});
    vars -= ({"PendingPauses", "PendingUnpauses"});
    vars -= ({"players", "creators", "user_list", "override" });
    vars -= ({"player_save_file", "namestr", "ob", "gplayer"});
    return vars;
}

static mixed GetPlayerVariable(string val){
    string vars = GetPlayerVariables();
    mixed ret;
    if(!val) return vars;
    if(member_array(val,variables(this_object())) == -1){
        //write("No such player variable exists.");
        return ret;
    }
    ret = fetch_variable(val);
    return ret;
}

mixed GetPlayerData(string player, string val){
    mixed ret;
    validate();
    if(!player) return 0;
    if(!user_exists(player)){
        write("No such user exists.");
        return 0;
    }
    gplayer = player;
    if(ob = find_player(player)){
        unguarded( (: ob->save_player(gplayer) :));
    }
    SaveObject(SaveFile);
    unguarded( (: LoadPlayer(gplayer) :) );
    ret = GetPlayerVariable(val);
    RestoreObject(SaveFile);
    return ret;
}

string array GetAdminIPs(){
    string array ret_array, name_array, line_array;
    string config_file;
    config_file = read_file(CFG_GROUPS);
    line_array = explode(config_file, "\n");
    ret_array = ({});
    name_array = ({});
    foreach(string line in line_array){
        string g,m;
        if(sizeof(line)) line = trim(line);
        if(sscanf(line,"(%s)%s",g,m) != 2) continue;
        if(g == "SECURE") name_array += explode(trim(m),":");
        else if(g == "ASSIST") name_array += explode(trim(m),":");
    }
    if(!sizeof(name_array)) return name_array;
    foreach(string nombre in name_array){
        ret_array += ({ GetPlayerData(nombre,"HostSite") });
    }
    return ret_array;
}

int CheckBuilder(object who){
    if(creatorp(who)) return 1;
    if(builderp(who)){
        if(!directory_exists(homedir(who)+"/area")){
            unguarded( (: make_workroom($(who)) :) );
        }
        return 1;
    }
    return 0; 
}

varargs string GetUserPath(mixed name, int legacy){
    string ret = "/tmp/";
    object player;
    if(!creators) creators = ({});
    if(!players) players = ({});
    if(name && objectp(name)) name = name->GetKeyName();
    if(!name){
        if(!this_player()) return ret;
        else name = this_player()->GetKeyName();
    }
    if(user_exists(name) && !legacy){
        player = unguarded( (: find_player($(name)) :) );
        if(player) ret = player->GetUserPath();
        else {
            mapping tmpmap;
            override = 1;
            tmpmap = GetPlayerData(name, "Directories");
            if(tmpmap) ret = tmpmap["home"];
        }
        if(ret) return ret;
    }
    if(member_array(name, creators) != -1){
        ret = REALMS_DIRS+"/"+name+"/";
    }
    else if(member_array(name, players) != -1){
        ret = DIR_ESTATES + "/"+name[0..0]+"/"+name+"/";
    }
    return ret;
}

string GetHomeRoom(string name){
    string ret;
    object dude;
    mapping tmpmap;

    if(!user_exists(name)) return 0;

    dude = unguarded( (: find_player($(name)) :) );
    if(dude){
        ret = dude->GetParanoia("homeroom");
    }
    else {
        override = 1;
        tmpmap = GetPlayerData(name, "Paranoia");
        ret = tmpmap["homeroom"];
    }
    return ret;
}

static void UserUpdate(string name, int status){
    object ob = find_player(name);
    if(member_array(name, local_users()) != -1) status = 1;
    else if(ob && !interactive(ob)) status = -1;
    else status = 0;
    INSTANCES_D->SendWhoUpdate(name, status);
}

void PlayerUpdate(string name, int status){
    call_out("UserUpdate", 1, name, status);
}

static mapping GatherUserData(){
    mapping cands = ([]);
    foreach(string user in user_list){         
        reset_eval_cost();
        gplayer = user;         
        if(ob = find_player(gplayer)){
            unguarded( (: ob->save_player(gplayer) :));
        }
        SaveObject(SaveFile);
        unguarded( (: LoadPlayer(gplayer) :) );
        cands[gplayer] = ([ "LoginTime" : LoginTime, "Age" : Age,
                "BirthTime" : BirthTime, "Email" : Email, "HostSite" : HostSite,
                "CreatorAge" : CreatorAge, "CreatorBirth" : CreatorBirth,
                "RealName" : RealName ]);
        RestoreObject(SaveFile);
    }
    UserData = cands;
    return cands;
}

int SelektUsers(int gather){
    mapping cands = ([]);
    int last, count, interval;
    validate();
    if(gather){
        CompilePlayerList();
        CompileCreList();
        cands = GatherUserData();
    }
    else cands = UserData;

    foreach(string user in sort_array(keys(cands),1)){
        int purge = 0;
        if(!cands[user]) continue;

        //Admins, elders, and testchars are exempt
        if(member_group(user, "SECURE")) continue;
        if(member_group(user, "ASSIST")) continue;
        if(member_group(user, "ELDER")) continue;
        if(member_group(user, "TEST")) continue;
        if(user == "guest") continue;

        count++;
        if(!(count % 5)) interval++;

        //bot always get purged, and snoop logs tagged
        if((cands[user]["RealName"] == "John Smith" && 
                    cands[user]["Email"] == "me@here") ||
                cands[user]["Email"] == "bot@delete.me"){
            reset_eval_cost();
            SNOOP_D->NotifyBot(user);
            reset_eval_cost();
            purge = 1;
        }

        //They last logged in over 1 month ago, and their
        //in-game time is less than 30 seconds.
        else if(cands[user]["Age"] < 30 &&
                (last = (time() - cands[user]["LoginTime"])) > 2592000){
            debug("Purging: "+user+", age: "+time_elapsed(cands[user]["Age"])+
                    " last logged in "+ctime(cands[user]["LoginTime"])+", "+
                    time_elapsed(last)+" ago.", "green");
            purge = 1;
        }

        //They last logged in over 6 months ago, and their in-game
        //time is less than two minutes.
        else if(cands[user]["Age"] < 121 && 
                (last = (time() - cands[user]["LoginTime"])) > 15552000){ 
            debug("Purging: "+user+", age: "+time_elapsed(cands[user]["Age"])+
                    " last logged in "+ctime(cands[user]["LoginTime"])+", "+
                    time_elapsed(last)+" ago.", "red"); 
            purge = 1;
        }

        //They last logged in over a year ago, only once, and
        //for less than one hour.
        else if(cands[user]["Age"] < 3600 &&
                (cands[user]["LoginTime"] - cands[user]["BirthTime"] < 100) &&
                (last = (time() - cands[user]["LoginTime"])) > 31104000){
            debug("Purging: "+user+", age: "+time_elapsed(cands[user]["Age"])+ 
                    " last logged in "+ctime(cands[user]["LoginTime"])+", "+
                    time_elapsed(last)+" ago.", "black");
            purge = 1;
        }

        if(purge){
            call_out("RemoveUser", interval, user);
            purge = 0;
        }
    }
    call_out( "SelektUsers", 604800, 1 );
    return 1;
}

int GetPaused(string player){
    int ret = 0;
    override = 1;
    if(user_exists(player)){
        gplayer = lower_case(player);
        ret = unguarded( (: GetPlayerData(lower_case(gplayer), "Paused") :) );
    }
    override = 0;
    return ret;
}
