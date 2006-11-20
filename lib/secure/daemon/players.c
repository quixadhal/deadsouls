#include <objects.h>
#include <save.h>
#include <lib.h>
#include <cfg.h>
#include <daemons.h>
#include <dirs.h>

inherit LIB_DAEMON;

mapping PlayerDataMap = ([]);
string *PendingEncres = ({});
string *PendingDecres = ({});
string *players = ({});
string *creators = ({});
string *user_list = ({});
static object ob;
static string gplayer;

string player_save_file;
static string namestr = "";

//Player vars
string Short, CapName, ExternalDesc, CurrentWorkingDirectory, PreviousWorkingDirectory, Terminal;
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

void validate(){
    if(catch((int)master()->valid_apply()) ||
      !(int)master()->valid_apply(({ "SECURE", "ASSIST", "LIB_CONNECT" })) ){
	string offender = identify(previous_object(-1));
	debug("PLAYERS_D SECURITY VIOLATION: "+offender+" ",get_stack(),"red");
	log_file("security", "\n"+timestamp()+" PLAYERS_D breach: "+offender+" "+get_stack());
    }
}


void create() {
    ::create();
    if(unguarded((: file_exists, SAVE_PLAYER_LIST+__SAVE_EXTENSION__ :)))
	unguarded((: restore_object, SAVE_PLAYER_LIST :));
    if(PendingEncres) PendingEncres = singular_array(PendingEncres);
    if(PendingDecres) PendingDecres = singular_array(PendingDecres);
    if(players) players = singular_array(players);
    if(creators) creators = singular_array(creators);
    if(user_list) user_list = singular_array(user_list);
    unguarded((: save_object, SAVE_PLAYER_LIST :));

}

string *eventCre(string str){
    str = lower_case(str);
    if(member_array(str,creators) == -1) creators += ({ str });
    if(member_array(str,players) != -1) players -= ({ str });
    unguarded((: save_object, SAVE_PLAYER_LIST :));
    return creators;
}

string *eventDecre(string str){
    str = lower_case(str);
    if(member_array(str,creators) != -1) creators -= ({ str });
    if(member_array(str,players) == -1) players += ({ str });
    unguarded((: save_object, SAVE_PLAYER_LIST :));
    return players;
}

void AddPlayerInfo(mixed arg) {

    if(!objectp(arg) && !stringp(arg)) return ;
    //tc("reveiced: "+identify(arg),"cyan");
    if(objectp(arg)){
	if(base_name(previous_object())!=LIB_CONNECT &&!interactive(arg)) return;
	else player_save_file = base_name(arg)+".o";
	//tc("player_save_file: "+identify(player_save_file),"red");
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
	    player_save_file = DIR_CRES +"/"+initial+"/"+arg;
	    if(!file_exists(player_save_file))
		player_save_file = DIR_PLAYERS +"/"+initial+"/"+arg;
	    if(!file_exists(player_save_file)) return;
	}
	else player_save_file = arg;
    }
    //tc("player_save_file: "+player_save_file,"blue");
    namestr = replace_string(last_string_element(player_save_file,"/"),".o","");
    if(grepp(player_save_file, DIR_CRES) && member_array(namestr,creators) == -1) 
	creators += ({ namestr }) ; 
    else if(member_array(namestr,players) == -1) 
	players += ({ namestr }) ; 
    if(!file_exists(player_save_file)) return;
    //tc("player_save_file: "+identify(player_save_file),"green");
    if(member_array(namestr,user_list) == -1)
	user_list += ({ replace_string(last_string_element(player_save_file,"/"),".o","") }) ; 
    unguarded((: save_object, SAVE_PLAYER_LIST :));
}

string *GetPlayerList(){
    return players;
}

string *GetCreatorList(){
    return creators;
}

string *GetUserList(){
    return user_list;
}

int RemoveUser(string str){
    if(!str || str == "") return 0;
    str = lower_case(str);
    if(!user_exists(str)) return 0;
    if(member_array(str, players) != -1) players -= ({ str });
    if(member_array(str, creators) != -1) creators -= ({ str });
    if(member_array(str, user_list) != -1) user_list -= ({ str });
    return 1;
}

string *AddPendingEncre(string str){
    validate();
    if(str && str != "") PendingEncres += ({ lower_case(str) });
    unguarded((: save_object, SAVE_PLAYER_LIST :));
    return PendingEncres;
}

string *RemovePendingEncre(string str){
    validate();
    if(!str || str == "") return PendingEncres;
    str = lower_case(str);
    if(member_array(str, PendingEncres) != -1) PendingEncres -= ({ lower_case(str) });
    unguarded((: save_object, SAVE_PLAYER_LIST :));
    return PendingEncres;
}

string *GetPendingEncres(){
    validate();
    return PendingEncres;
}

string *AddPendingDecre(string str){
    validate();
    if(str && str != "") PendingDecres += ({ lower_case(str) });
    unguarded((: save_object, SAVE_PLAYER_LIST :));
    return PendingDecres;
}

string *RemovePendingDecre(string str){
    validate();
    if(!str || str == "") return PendingDecres;
    str = lower_case(str);
    if(member_array(str, PendingDecres) != -1) PendingDecres -= ({ lower_case(str) });
    unguarded((: save_object, SAVE_PLAYER_LIST :));
    return PendingDecres;
}

string *GetPendingDecres(){
    validate();
    return PendingDecres;
}

static int LoadPlayer(string str){
    string arg = "", initial = "";
    if(!user_exists(arg = last_string_element(lower_case(replace_string("/"+str,".o","")),"/"))){
	return 0;
    }
    arg += ".o";
    initial = arg[0..0];
    player_save_file = DIR_CRES +"/"+initial+"/"+arg;
    if(!file_exists(player_save_file))
	player_save_file = DIR_PLAYERS +"/"+initial+"/"+arg;
    //tc("player_save_file: "+player_save_file);
    if(!file_exists(player_save_file)) return -1;

    restore_object(player_save_file,1);
    //tc("ExternalDesc: "+ExternalDesc);
    //tc("player_save_file: "+player_save_file);
    return 1;
}

int eventDestruct(){
    validate();
    unguarded((: save_object, SAVE_PLAYER_LIST :));
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
    vars -= ({"players", "creators", "user_list" });
    vars -= ({"player_save_file", "namestr", "ob", "gplayer"});
    return vars;
}

static mixed GetPlayerVariable(string val){
    string vars = GetPlayerVariables();
    if(!val) return vars;
    if(member_array(val,variables(this_object())) == -1){
	write("No such player variable exists.");
	return 0;
    }
    return fetch_variable(val);
}

mixed GetPlayerData(string player, string val){
    validate();
    if(!player) return 0;
    if(!user_exists(player)){
	write("No such user exists.");
	return 0;
    }
    gplayer = player;
    if(ob = find_player(player)){
	//tc("wtf");
	unguarded( (: ob->save_player(gplayer) :));
    }
    unguarded( (: LoadPlayer(gplayer) :) );
    return GetPlayerVariable(val);
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
	if(g == "SECURE") name_array = explode(trim(m),":");
    }
    if(!sizeof(name_array)) return name_array;
    foreach(string nombre in name_array){
	ret_array += ({ GetPlayerData(nombre,"HostSite") });
    }
    return ret_array;
}

