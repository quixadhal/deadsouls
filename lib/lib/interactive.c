/*    /lib/interactive.c
 *    from the Dead Souls Object Library
 *    an interactive object
 *    created by Descartes of Borg 941001 for the Idea Exchange
 *    Version: @(#) interactive.c 1.17@(#)
 *    Last modified: 97/01/03
 */

#ifndef NM_STYLE_EXITS
#define NM_STYLE_EXITS 1
#endif

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

inherit LIB_DESCRIBE;
inherit LIB_CONTAINER;
inherit LIB_OBJECT;
inherit LIB_AUTOSAVE;
inherit LIB_FILES;
inherit LIB_MOVE;
inherit LIB_PAGER;
inherit LIB_MESSAGES;
inherit LIB_INTERFACE;

private int Age, WhereBlock, Brief, LoginTime, BirthTime, RescueBit;
private string Password, Email, RealName, Rank, LoginSite, HostSite, WebPage;
private static string globaltmp;
private mapping News;
private class marriage *Marriages;
private static int LastAge, Setup;
private static object NetDiedHere;
private static mapping LastError;
private static string *UserId;

static void create(){
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
    RescueBit = 0;
    SetShort("$N the unaccomplished");
    SetLong("$N is nondescript.");
}

int SetRescueBit(int i){
    RescueBit = i;
    return i;
}

/* ***************  /lib/interactive.c modal functions  *************** */

mixed CanDivorce(){
    class marriage m;

    if( !Marriages || !sizeof(Marriages) )
        return GetName() + " is not married!";
    m = Marriages[0];
    if( m->DivorceDate ) return GetName() + " is not married!";
    return 1;
}

mixed CanGet(){
    return GetName() + " is a living being!";
}

mixed CanMarry(){
    if( !Marriages ) Marriages = ({});
    if( GetSpouse() ) return GetName() + " is currently married!";
    return 1;
}

/* ***************  /lib/interactive.c lfuns  *************** */
int Setup(){
    mapping mp;
    string tmp;

    if(Setup) return 0;
    else Setup = 1;
    set_living_name(GetKeyName());
    interface::Setup();
    add_action((: cmdQuit :), "quit");
    add_action((: cmdParseRefresh :), "parserefresh");
    HostSite = query_ip_name(this_object());
    LoginTime = time();
    SetId(({}));
    autosave::Setup();
    call_out("save_player", 2, GetKeyName());
    if( VOTING_D->GetStatus() == VOTE_RUNNING ){
        if( VOTING_D->GetMode() == VOTE_MODE_CANDIDATES )
            eventPrint("%^YELLOW%^Class Elections are in progress!  "
              "Go nominate candidates!%^RESET%^");
        if( VOTING_D->GetMode() == VOTE_MODE_VOTING )
            eventPrint("%^YELLOW%^Class Elections are in progress!  "
              "Go vote for the candidates!%^RESET%^");
    } 

    if( VOTING_D->GetVoteStatus( this_object() ) != VOTE_ALREADY_VOTED ){
        eventPrint("%^YELLOW%^You have not yet voted!  "
          "Please vote now.%^RESET%^");
        eventMove( VOTING_D->GetVoteRoom() );
    } else {
        object room;
        if(grepp(LoginSite,"#")){
            room = find_object(LoginSite);
        }
        if(!room) catch(room = load_object(LoginSite));
        if( room && room->GetMedium() == MEDIUM_AIR ){
        }
        if(!sizeof(LoginSite) || 
          (!room && !file_exists(LoginSite) && !file_exists(LoginSite+".c")) || 
          !eventMove(LoginSite) || RescueBit ){ 
            LoginSite = ROOM_START;
            eventMove(ROOM_START);
            SetRescueBit(0);
        }
    }
    environment()->eventPrint(tmp, MSG_ENV, this_object());
    if( !(tmp = GetMessage("login")) )
        tmp = GetName() + " enters " + mud_name() + ".";
    if(!(archp(this_object()) && this_object()->GetInvis())){
        log_file("enter", GetCapName()+" (enter): "+ctime(time())+
          " : "+query_ip_name(this_object())+"\n");
        CHAT_D->eventSendChannel("SYSTEM","connections","[" + GetCapName() + " logs in]",0);
    }

    if(!catch(mp = (mapping)FOLDERS_D->mail_status(GetKeyName()))){
        if(mp["unread"]){
            eventPrint("\n%^RED%^%^BOLD%^>>> " + mp["unread"] + " of your " +
              (mp["total"] == 1 ? mp["total"] + " letter is" :
                mp["total"] + " letters remain") + " unread. <<<%^RESET%^\n",
              MSG_SYSTEM);
        }
    }
    NEWS_D->GeneralNews();
    return 1;
}

static void net_dead(){
    interface::net_dead();
    Age += time() - LastAge;
    LastAge = time();
    NetDiedHere = environment(this_object());
    save_player(GetKeyName());
    if(!(archp(this_object()) && this_object()->GetInvis())){
        log_file("enter", GetCapName() + " (net-dead): " + ctime(time()) + "\n");
        environment()->eventPrint(GetName() + " suddenly disappears into "
          "a sea of irreality.", MSG_ENV, this_object());
        CHAT_D->eventSendChannel("SYSTEM","connections","[" + GetCapName() + " goes net-dead]",0);
    }
    SNOOP_D->ReportLinkDeath(this_object()->GetKeyName());
    eventMove(ROOM_FREEZER);
    if(query_snoop(this_object()))
        query_snoop(this_object())->eventPrint(GetCapName() + " has gone "
          "net-dead.", MSG_SYSTEM);
}

void eventReconnect(){
    interface::eventReconnect();
    LastAge = time();
    HostSite = query_ip_name(this_object());
    eventPrint("Reconnected.", MSG_SYSTEM);
    if(!(archp(this_object()) && this_object()->GetInvis())){
        CHAT_D->eventSendChannel("SYSTEM","connections","[" + GetCapName() + " has rejoined " + mud_name() + "]",0);
        environment()->eventPrint(GetCapName() + " has rejoined this reality.",
          MSG_ENV, this_object());
    }
    if( NetDiedHere ) eventMove(NetDiedHere);
    else eventMove(ROOM_START);
    NetDiedHere = 0;
}

int eventDestruct(){
    object ob;

    interface::eventDestruct();
    foreach(ob in deep_inventory(this_object())){
        if( ob ) catch(ob->eventDestruct());
    }
    return object::Destruct();
}

mixed eventDivorce(){
    class marriage m;

    m = Marriages[0];
    m->DivorceDate = time();
    return 1;
}

mixed eventMarry(object who, object to_whom){
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

int eventMove(mixed dest){
    string str;
    int x;

    x = move::eventMove(dest);
    if( x ){
        if( !(str = (string)environment()->GetProperty("login")) ){
            if(clonep(environment())) LoginSite = file_name(environment());
            else LoginSite = base_name(environment());
        }
        else LoginSite = str;
    }
    return x;
}

void cmdParseRefresh(){
    parse_refresh();
}

int cmdQuit(){
    string tmp;
    object env = environment(this_object());
    int retain = RETAIN_ON_QUIT;
    if(!env) env = load_object(ROOM_FURNACE);

    if( previous_object() && !
      ((int)master()->valid_apply( ({ GetKeyName() }) )) ) return 0;
    if( env->GetProperty("no quit") &&
      ! sizeof(previous_object(-1)) ){
        message("system", "You are unable to escape this reality!",
          this_object());
        return 0;
    }
    tell_object(this_object(),"Please come back another time!");
    if(!creatorp(this_object())){
        foreach(object ob in all_inventory(this_object())){
            if((!retain && !ob->GetRetain()) || !ob->GetRetain()) ob->eventMove(env);
        }
        foreach(object ob in deep_inventory(this_object())){
            if((!retain && !ob->GetRetain()) || !ob->GetRetain()) ob->eventMove(env);
        }
    }
    this_object()->AddCarriedMass(-(this_object()->GetCarriedMass()));
    tmp = GetMessage("logout") || (this_object()->GetName() + 
      " is gone from this reality!");
    save_player(GetKeyName());
    if(!(archp(this_object()) && this_object()->GetInvis())){
        log_file("enter", GetCapName()+" (quit): "+timestamp()+"\n");
        if(env) message("environment", tmp, env, ({this_object()}));
        CHAT_D->eventSendChannel("SYSTEM","connections","[" + GetCapName() + " quits]",0);
    }
    if(in_edit()){
        ed_cmd(".");
        ed_cmd("x");
        ed_cmd("Q");
    }
    flush_messages();
    this_object()->eventDestruct();
    return 1;
}

int GetAge(){
    int x;

    if(!interactive(this_object())) return Age;
    x = time() - LastAge;
    Age += x;
    LastAge = time();
    return Age;
}

int GetBirth(){ return BirthTime - (YEAR * 18); }

string query_cwd(){ return interface::query_cwd(); }

void SetEmail(string str){
    if(!((int)master()->valid_apply(({ GetKeyName() })))) return;
    Email = str;
}

string GetEmail(){
    if(!((int)master()->valid_apply(({ GetKeyName() })))) return 0;
    else return Email;
}

varargs string array SetId(string *bogus){
    int i;
    string tmp;

    UserId = ({ GetKeyName() });
    if(GetCapName()) UserId += ({ lower_case(GetCapName()) });

    if(sizeof(bogus)){
        if(this_player() && this_player() == this_object()){
            if(!arrayp(bogus)) bogus = ({ bogus });
            if(!UserId) UserId = bogus;
            else UserId += bogus;
        }
    }

    if(tmp = this_object()->GetRace()) UserId += ({ tmp });

    if(OBJECT_MATCHING){
        UserId = atomize_array(UserId);
    }

    UserId = distinct_array(UserId);

    foreach(string sub in UserId){
        if(user_exists(sub) && GetKeyName() != sub){
            //if(find_player(sub) && find_player(sub) != this_object()){
            UserId -= ({ sub });
            //}
        }
    }
    parse_init();
    return UserId;
}

string *GetId(){ return UserId; }

int id(string str){
    if(!str || !UserId) return 0;
    else return member_array(lower_case(str), UserId) != -1;
}

int GetLoginTime(){ return LoginTime; }

string SetKeyName(string str){
    if( previous_object() != master() ) return GetKeyName();
    return object::SetKeyName(str);
}

string GetKeyName(){ return object::GetKeyName(); }

void SetNews(string type, int sz){ News[type] = sz; }

int GetNews(string type){ return News[type]; }

void SetPassword(string str){
    if(!((int)master()->valid_apply(({ GetKeyName() })))) return;
    Password = str;
    save_player(GetKeyName());
}

string GetPassword(){
    if(!((int)master()->valid_apply(({ GetKeyName() })))) return 0;
    else return Password;
}

void SetRank(string str){ Rank = str; }

string GetRank(){ return Rank; }

void SetRealName(string str){
    if(!((int)master()->valid_apply(({ GetKeyName() })))) return;
    RealName = str;
}

string GetRealName(){ return RealName; }

string GetShort(){
    string str;

    str = object::GetShort(str);
    if( !str ) str = "$N the unaccomplished";
    if( strsrch(str, "$N") == -1 ) str = "$N";
    str = replace_string(str, "$N", (GetName() || ""));
    if( interactive(this_object()) ) return str;
    else return str + " (net-dead)";
}

varargs string GetLong(){
    string str;
    str = object::GetLong(str);
    if(!str || strsrch(str, "$N") == -1) str = "$N is nondescript.";
    str = replace_string(str, "$N", GetName());
    return str;
}

int SetBriefMode(int x){ return (Brief = x); }

int GetBriefMode(){ return Brief; }

int SetWhereBlock(){ return (WhereBlock = !WhereBlock); }

int GetWhereBlock(){ return WhereBlock; }

string get_path(){
    return query_cwd();
}

void SetLastError(mapping m){
    if (previous_object() != master()) return;
    LastError = m;
}

mapping GetLastError(){
    string *allowed = ({ "/secure/cmds/creators/dbxwhere", "/secure/cmds/creators/dbxframe", "/secure/cmds/players/bug", file_name(master()) });
    string caller = file_name(previous_object());
    if(member_array(caller, allowed) != -1 ){
        if(LastError) return LastError;
    }
    else error("Privilege Violation: " + caller);
}

string SetCapName(string str){
    if( base_name(previous_object(0)) != LIB_CONNECT ) return str;
    return object::SetCapName(str);
}

void move_or_destruct(){
    (eventMove(ROOM_START) || eventMove(ROOM_VOID));
}

string SetShort(string str){
    if( strsrch(str, "$N") == -1 ) return object::GetShort();
    else return object::SetShort(str);
}

string SetLong(string str){
    if(strsrch(str, "$N") == -1) return object::GetLong();
    else return object::SetLong(str);
}

string GetName(){ return object::GetName(); }

varargs int GetInvis(object ob){ return object::GetInvis(ob); }

mixed *GetCommands(){
    if( !((int)master()->valid_apply( ({ GetKeyName() }) )) ) return ({});
    else return commands();
}

string GetSpouse(){
    if( !Marriages || !sizeof(Marriages) ) return 0;
    if( ((class marriage)Marriages[0])->DivorceDate ) return 0;
    return ((class marriage)Marriages[0])->Spouse;
}

class marriage *GetMarriages(){ return Marriages; }

string GetHostSite(){
    if( WhereBlock ){
        if( !((int)master()->valid_access(({ PRIV_ASSIST }))) )
            return "Confidential";
        else return HostSite;
    }
    return HostSite;
}

int GetRadiantLight(int ambient){
    return container::GetRadiantLight(ambient);
}

string GetWebPage(){
    return WebPage;
}

string SetWebPage(string page){
    if( !master()->valid_apply(({ GetKeyName() })) ){
        return WebPage;
    }
    return (WebPage = page);
}
