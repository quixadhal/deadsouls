/*    /lib/interactive.c
 *    from the Dead Soulsr1 Object Library
 *    an interactive object
 *    created by Descartes of Borg 941001 for the Idea Exchange
 *    Version: @(#) interactive.c 1.17@(#)
 *    Last modified: 97/01/03
 */

#include <lib.h>
#include <clock.h>
#include <privs.h>
#include <daemons.h>
#include <objects.h>
#include <rooms.h>
#include <marriage.h>
#include <medium.h>
#include <position.h>
#include <message_class.h>
#include <vision.h>
#include <voting.h>
#include "include/interactive.h"

inherit LIB_CONTAINER;
inherit LIB_OBJECT;
inherit LIB_AUTOSAVE;
inherit LIB_FILES;
inherit LIB_MOVE;
inherit LIB_PAGER;
inherit LIB_MESSAGES;
inherit LIB_INTERFACE;

private int Age, WhereBlock, Brief, LoginTime, BirthTime;
private string Password, Email, RealName, Rank, LoginSite, HostSite, WebPage;
private mapping News;
private class marriage *Marriages;
private static int LastAge, Setup;
private static object NetDiedHere;
private static mapping LastError;
private static string *UserId;

static void create() {
    object::create();
    messages::create();
    interface::create();
    SetOpacity(0);
    Marriages = ({});
    HostSite = "";
    Brief = 0;
    Password = 0;
    Email = 0;
    RealName = 0;
    Rank = "citizen";
    LoginSite = ROOM_START;
    BirthTime = time();
    LastAge = time();
    News = ([]);
    SetShort("$N the unaccomplished");
    SetLong("$N is nondescript.");
}

/* ***************  /lib/interactive.c modal functions  *************** */

mixed CanDivorce(object who) {
    class marriage m;
    
    if( !Marriages || !sizeof(Marriages) )
      return GetName() + " is not married!";
    m = Marriages[0];
    if( m->DivorceDate ) return GetName() + " is not married!";
    return 1;
}
    
mixed CanGet(object ob) {
    return GetName() + " is a living being!";
}

mixed CanMarry(object who, object to_whom) {
    if( !Marriages ) Marriages = ({});
    if( GetSpouse() ) return GetName() + " is currently married!";
    return 1;
}

/* ***************  /lib/interactive.c lfuns  *************** */
int Setup() {
    mapping mp;
    string tmp;

    if(Setup) return 0;
    else Setup = 1;
    set_living_name(GetKeyName());
    interface::Setup();
    add_action((: cmdQuit :), "quit");
    HostSite = query_ip_name(this_object());
    LoginTime = time();
    SetId(({}));
    autosave::Setup();
    call_out("save_player", 2, GetKeyName());
    log_file("enter", GetCapName()+" (enter): "+ctime(time())+
      " : "+query_ip_name(this_object())+"\n");
    if( VOTING_D->GetStatus() == VOTE_RUNNING ) {
        if( VOTING_D->GetMode() == VOTE_MODE_CANDIDATES )
            eventPrint("%^YELLOW%^Class Elections are in progress!  "
              "Go nominate candidates!%^RESET%^");
        if( VOTING_D->GetMode() == VOTE_MODE_VOTING )
            eventPrint("%^YELLOW%^Class Elections are in progress!  "
              "Go vote for the candidates!%^RESET%^");
    } 

    if( VOTING_D->GetVoteStatus( this_object() ) != VOTE_ALREADY_VOTED ) {
        eventPrint("%^YELLOW%^You have not yet voted!  "
          "Please vote now.%^RESET%^");
        eventMove( VOTING_D->GetVoteRoom() );
    } else {
	object room;
	
	catch(room = load_object(LoginSite));
	if( room && room->GetMedium() == MEDIUM_AIR ) {
	}
        if( !eventMove(LoginSite) ) {
	    eventMove(LoginSite = ROOM_START);
	}
    }
    environment()->eventPrint(tmp, MSG_ENV, this_object());
    if( !(tmp = GetMessage("login")) )
      tmp = GetName() + " enters " + mud_name() + ".";
    filter(users(), (: archp :))->eventPrint("[" + GetCapName() + " logs in]",
                                             MSG_SYSTEM);
    if(!catch(mp = (mapping)FOLDERS_D->mail_status(GetKeyName()))) {
        if(mp["unread"]) {
            eventPrint("\n>>> " + mp["unread"] + " of your " +
                       (mp["total"] == 1 ? mp["total"] + " letter is" :
                        mp["total"] + " letters remain") + " unread. <<<\n",
                       MSG_SYSTEM);
        }
    }
    NEWS_D->GeneralNews();
    return 1;
}

static void net_dead() {
    interface::net_dead();
    Age += time() - LastAge;
    LastAge = time();
    NetDiedHere = environment(this_object());
    save_player(GetKeyName());
    log_file("enter", GetCapName() + " (net-dead): " + ctime(time()) + "\n");
    environment()->eventPrint(GetName() + " suddenly disappears into "
                              "a sea of irreality.", MSG_ENV, this_object());
    filter(users(), (: archp :))->eventPrint("[" + GetCapName() +
                                             " goes net-dead]", MSG_SYSTEM);
    eventMove(ROOM_FREEZER);
    if(query_snoop(this_object()))
      query_snoop(this_object())->eventPrint(GetCapName() + " has gone "
                                             "net-dead.", MSG_SYSTEM);
}

void eventReconnect() {
    string dead_ed;

    interface::eventReconnect();
    LastAge = time();
    HostSite = query_ip_name(this_object());
    eventPrint("Reconnected.", MSG_SYSTEM);
    filter(users(), (: archp :))->eventPrint("[" + GetCapName() + " has "
                                             "rejoined " + mud_name() + "]",
                                             MSG_SYSTEM);
    if( NetDiedHere ) eventMove(NetDiedHere);
    else eventMove(ROOM_START);
    environment()->eventPrint(GetCapName() + " has rejoined this reality.",
                              MSG_ENV, this_object());
    NetDiedHere = 0;
}

void eventDescribeEnvironment(int brief) {
    object env;
    mixed tmp;
    string *shorts;
    string desc, smell, sound, touch;
    int i, maxi;

    if(!(env = environment(this_object()))) {
        eventPrint("You are nowhere.", MSG_ROOMDESC);
        return;
    }
    switch( i = GetEffectiveVision() ) {
    case VISION_BLIND:
	eventPrint("You are blind and can see nothing.");
	break;
    case VISION_TOO_DARK:
        eventPrint("It is much too dark to see.");
	break;
    case VISION_DARK:
	eventPrint("It is too dark to see.");
        break;
    case VISION_TOO_BRIGHT:
        eventPrint("It is much too %^YELLOW%^bright%^RESET%^ to see.");
	break;
    case VISION_BRIGHT:
        eventPrint("It is too %^YELLOW%^bright%^RESET%^ to see.");
        break;
    }
    if( !brief ) {
	if( i == VISION_CLEAR ) {
	    desc = (string)env->GetObviousExits() || "";
	    desc = capitalize((string)env->GetShort() || "")
		+ " [" + desc + "]\n";
	}
	else desc = "";
	if( i == VISION_CLEAR || i == VISION_LIGHT || i == VISION_DIM )
	  desc += (string)env->GetLong();
        if(functionp(tmp = (mixed)env->GetSmell("default")))
	    tmp = (string)(*tmp)("default");
        smell = tmp;
        if(functionp(tmp = (mixed)env->GetListen("default")))
          tmp = (string)(*tmp)("default");
        sound = tmp;
        if( functionp(tmp = (mixed)env->GetTouch("default")) )
          tmp = evaluate(tmp, "default");
        touch = tmp;
    }
    else {
	if( i == VISION_CLEAR || i == VISION_LIGHT || i == VISION_DIM ) {
	    desc = (string)env->GetShort();
	    if( (tmp = (string)env->GetObviousExits()) && tmp != "" )
		desc += " [" + tmp + "]";
	}
	else desc = "";
    }
    if( desc ) eventPrint(desc, MSG_ROOMDESC);
    if( smell ) eventPrint("%^GREEN%^" + smell, MSG_ROOMDESC);
    if( sound ) eventPrint("%^CYAN%^" + sound, MSG_ROOMDESC);
    if( touch ) eventPrint("%^YELLOW%^" + touch, MSG_ROOMDESC);
    desc = "";
    if( i == VISION_CLEAR ) {
	mapping lying = ([]);
	shorts = map(filter(all_inventory(env),
			    function(object ob) {
	                        if( living(ob) ) return 0;
				if( (int)ob->GetInvis(this_object()) )
				  return 0;
				if( (int)ob->isFreshCorpse() ) return 0;
				return 1;
	                    }), (: (string)$1->GetShort() :));
	foreach(string s in shorts) {
	    if( s ) {
		lying[s]++;
	    }
	}
	for(i=0, desc = 0, maxi = sizeof(shorts = keys(lying)); i<maxi; i++) {
	    string key = shorts[i];
	    int val = lying[shorts[i]];

	    if( val < 2 ) {
		if( !desc ) desc = "%^MAGENTA%^" +
				capitalize(key) + "%^RESET%^MAGENTA%^";
		else desc += key + "%^RESET%^MAGENTA%^";
	    }
	    else {
		if( !desc ) desc = "%^MAGENTA%^" +
				capitalize(consolidate(val, key)) +
		    "%^RESET%^MAGENTA%^";
		else desc += consolidate(val, key) +
		    "%^RESET%^MAGENTA%^";
	    }
	    if( i == maxi - 1 ) {
		if( maxi > 1 || val >1 )
		    desc += " are here.%^RESET%^\n";
		else desc += " is here.%^RESET%^\n";
	    }
	    else if( i == maxi - 2 ) {
		if( maxi == 2 ) {
		    desc += " and ";
		}
		else {
		    desc += ", and ";
		}
	    }
	    else desc += ", ";
	}
    }
    i = GetEffectiveVision();
    if( i == VISION_CLEAR || i == VISION_LIGHT || i == VISION_DIM ) {
	mapping lying = ([]), sitting = ([]), standing = ([]);
	object *obs;
	string key;
	int val;
	
	obs = filter(all_inventory(env), function(object ob) {
	                                   if( (int)ob->GetInvis(this_object()) ) return 0;
					   if( living(ob) ) return 1;
					   if( (int)ob->isFreshCorpse() )
					     return 1;
	                                 }) - ({ this_object() });
	maxi = sizeof(shorts = map(obs, (: (string)$1->GetHealthShort() :)));
	foreach(object liv in obs) {
	    string s = (string)liv->GetHealthShort();
	    int pos = (int)liv->GetPosition();
	    
	    if( !s ) continue;
	    if( creatorp(liv) || pos == POSITION_STANDING) standing[s]++;
	    else if( pos == POSITION_LYING || (int)liv->isFreshCorpse() )
	      lying[s]++;
	    else if( pos == POSITION_SITTING ) sitting[s]++;
	    else lying[s]++;
	}
        if( !desc ) {
            tmp = "";
        }
        else {
            tmp = desc;
        }
	desc = "";
	foreach(key, val in lying) {
	    if( val<2 ) desc += capitalize(key) + "%^RESET%^ is lying down.";
	    else desc += capitalize(consolidate(val, key)) +
		"%^RESET%^ are lying down.";
	    desc += "\n";
	}
	foreach(key, val in sitting) {
	    if( val<2 )
	      desc += capitalize(key) + "%^RESET%^ is sitting down.";
	    else desc += capitalize(consolidate(val, key)) +
		"%^RESET%^ are sitting down.";
	    desc += "\n";
	}
	foreach(key, val in standing) {
	    if( val<2 )
	      desc += capitalize(key) + "%^RESET%^ is standing here.";
	    else desc += capitalize(consolidate(val, key)) +
		"%^RESET%^ are standing here.";
	    desc += "\n";
	}
    }
    if( tmp ) {
	desc = tmp + desc;
    }
    if( sizeof(desc) ) {
	eventPrint(desc + "\n", MSG_ROOMDESC);
    }
}

int eventDestruct() {
    object ob;
    int i;

    interface::eventDestruct();
    foreach(ob in deep_inventory(this_object())) {
	if( ob ) catch(ob->eventDestruct());
    }
    return object::eventDestruct();
}

mixed eventDivorce(object who) {
    class marriage m;

    m = Marriages[0];
    m->DivorceDate = time();
    return 1;
}
    
mixed eventMarry(object who, object to_whom) {
    class marriage m;
    object env;

    if( (env = previous_object()) != environment() ) return 0;
    m = new(class marriage);
    m->Spouse = (string)to_whom->GetCapName();
    m->WeddingDate = time();
    m->DivorceDate = 0;
    m->Location = file_name(env);
    Marriages = ({ m }) + Marriages;
    return 1;
}

int eventMove(mixed dest) {
    string str;
    int x;

    x = move::eventMove(dest);
    if( x ) {
        if( !(str = (string)environment()->GetProperty("login")) )
          LoginSite = base_name(environment());
        else LoginSite = str;
    }
    return x;
}

int cmdQuit(string str) {
    string tmp;

    if( previous_object() && !
       ((int)master()->valid_apply( ({ GetKeyName() }) )) ) return 0;
    if( (int)environment(this_object())->GetProperty("no quit") &&
      ! sizeof(previous_object(-1)) ) {
        message("system", "You are unable to escape this reality!",
          this_object());
        return 1;
    }
    message("system", "Please come back another time!", this_object());
    save_player(GetKeyName());
    tmp = GetMessage("logout") || (GetName() + " is gone from this reality!");
    message("environment", tmp, environment(this_object()), ({this_object()}));
    log_file("enter", GetCapName()+" (quit): "+ctime(time())+"\n");
    message("announce", "["+GetCapName()+" quits]", filter(users(), (: archp :)));
    eventDestruct();
    return 1;
}

int GetAge() {
    int x;

    if(!interactive(this_object())) return Age;
    x = time() - LastAge;
    Age += x;
    LastAge = time();
    return Age;
}

int GetBirth() { return BirthTime - (YEAR * 18); }

string query_cwd() { return interface::query_cwd(); }

void SetEmail(string str) {
    if(!((int)master()->valid_apply(({ GetKeyName() })))) return;
    Email = str;
}

string GetEmail() {
    if(!((int)master()->valid_apply(({ GetKeyName() })))) return 0;
    else return Email;
}

void SetId(string *bogus) {
    int i;

    if(UserId) return;
    if(!GetCapName()) {
        UserId = ({ GetKeyName() });
        return;
    }
    UserId = ({ GetKeyName(), lower_case(GetCapName()) });
    if((i=sizeof(bogus = explode(lower_case(GetCapName()), " "))) == 1)
        return;
    while(i--)
        if(!user_exists(bogus[i])) UserId += ({ bogus[i] });
}

string *GetId() { return UserId; }

int id(string str) {
    if(!str || !UserId) return 0;
    else return member_array(lower_case(str), UserId) != -1;
}

int GetLoginTime() { return LoginTime; }

string SetKeyName(string str) {
    if( previous_object() != master() ) return GetKeyName();
    return object::SetKeyName(str);
}

string GetKeyName() { return object::GetKeyName(); }

void SetNews(string type, int sz) { News[type] = sz; }

int GetNews(string type) { return News[type]; }

void SetPassword(string str) {
    if(!((int)master()->valid_apply(({ GetKeyName() })))) return;
    Password = str;
    save_player(GetKeyName());
}

string GetPassword() {
    if(!((int)master()->valid_apply(({ GetKeyName() })))) return 0;
    else return Password;
}

void SetRank(string str) { Rank = str; }

string GetRank() { return Rank; }

void SetRealName(string str) {
    if(!((int)master()->valid_apply(({ GetKeyName() })))) return;
    RealName = str;
}

string GetRealName() { return RealName; }

string GetShort() {
    string str;

    str = object::GetShort(str);
    if( !str ) str = "$N the unaccomplished";
    if( strsrch(str, "$N") == -1 ) str = "$N";
    str = replace_string(str, "$N", GetName());
    if( interactive(this_object()) ) return str;
    else return str + " (net-dead)";
}

varargs string GetLong() {
    string str;

    str = object::GetLong(str);
    if(!str || strsrch(str, "$N") == -1) str = "$N is nondescript.";
    str = replace_string(str, "$N", GetName());
    return str;
}

int SetBriefMode(int x) { return (Brief = x); }

int GetBriefMode() { return Brief; }

int SetWhereBlock() { return (WhereBlock = !WhereBlock); }

int GetWhereBlock() { return WhereBlock; }

string get_path() {
    log_file("get_path", identify(previous_object())+"\n");
    return query_cwd();
}

void SetLastError(mapping m) {
    if (previous_object() != master()) return;
    LastError = m;
}

mapping GetLastError() {
    string caller = file_name(previous_object());
    if( caller[0..24] == "/secure/cmds/creators/dbx" ||
      caller[0..23] == "/secure/cmds/players/bug" ) return LastError;
    error("Privilege Violation: " + caller);
}

void SetCapName(string str) {
    if( base_name(previous_object(0)) != LIB_CONNECT ) return;
    object::SetCapName(str);
}

void move_or_destruct() {
    (eventMove(ROOM_START) || eventMove(ROOM_VOID));
}

string SetShort(string str) {
    if( strsrch(str, "$N") == -1 ) return object::GetShort();
    else return object::SetShort(str);
}

string SetLong(string str) {
    if(strsrch(str, "$N") == -1) return object::GetLong();
    else return object::SetLong(str);
}

string GetName() { return object::GetName(); }

varargs int GetInvis(object ob) { return object::GetInvis(ob); }

mixed *GetCommands() {
    if( !((int)master()->valid_apply( ({ GetKeyName() }) )) ) return ({});
    else return commands();
}

string GetSpouse() {
    if( !Marriages || !sizeof(Marriages) ) return 0;
    if( ((class marriage)Marriages[0])->DivorceDate ) return 0;
    return ((class marriage)Marriages[0])->Spouse;
}

class marriage *GetMarriages() { return Marriages; }

string GetHostSite() {
    if( WhereBlock ) {
	if( !((int)master()->valid_access(({ PRIV_ASSIST }))) )
	  return "Confidential";
	else return HostSite;
    }
    return HostSite;
}

void eventLoadObject(mixed *value, int recurse) { }

int GetRadiantLight(int ambient) {
    return (object::GetRadiantLight(ambient) +
	    container::GetRadiantLight(ambient));
}

string GetWebPage() {
    return WebPage;
}

string SetWebPage(string page) {
    if( !master()->valid_apply(({ GetKeyName() })) ) {
	return WebPage;
    }
    return (WebPage = page);
}
