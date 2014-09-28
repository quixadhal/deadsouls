/*    /adm/daemon/banish.c
 *    from Dead Souls
 *    maintains information on legitimate character creation
 *    created by Descartes of Borg 940115
 */

#include <lib.h>
#include <privs.h>
#include <objects.h>
#include <daemons.h> 
#include <save.h> 
#include <dirs.h> 

#ifndef GUEST_ALLOWED
#define GUEST_ALLOWED 1
#endif

#ifndef CFG_IP_BLACKLIST
#define CFG_IP_BLACKLIST DIR_SECURE_CFG "/ip_blacklist.cfg"
#endif

inherit LIB_DAEMON;

string *__Names, *__Sites, *__WatchNames, *__WatchSites; 
string *__Allowed, *__Guests, *__IllegalSubStrings; 
mapping __TmpBanish;
static string SaveFile;

static private int valid_access(object ob);
void register_site(string str);
void temporary_register(string str, int time);
string query_temp_sites();
void manual_temp_unregister(string str);
void clean_temp_sites();
void unregister_site(string str);
string *query_register();
void banish_name(string str);
void unbanish_name(string str);
string *query_banished();
void watch_site(string str);
void unwatch_site(string str);
string *query_watched_sites();
void watch_name(string str);
void unwatch_name(string str);
string *query_watched_names();
void allow_name(string str);
void unallow_name(string str);
string *query_allowed();
void set_illegal_substring(string str);
void remove_illegal_substring(string str);
string *query_illegal_substrings();
void add_guest(string str);
void remove_guest(string str);
string *query_guests();
static private void save_banish();
int GetGuest(string str);
int valid_name(string str);
int eventConnect(string nom, string ip);
static private int match_ip(string ip, string *sites);
void setup_blacklist();

void create() { 
    daemon::create();
    SetNoClean(1);
    SaveFile = save_file(SAVE_BANISH);
    __Names = ({"template"}); 
    __Sites = ({}); 
    __WatchNames = ({}); 
    __WatchSites = ({}); 
    __Allowed = ({}); 
    __Guests = ({}); 
    __IllegalSubStrings = ({}); 
    __TmpBanish=([]);
    if(unguarded( (: file_exists(SaveFile) :) )){
        RestoreObject(SaveFile);
    }
    if(!__TmpBanish) __TmpBanish = ([]);
    clean_temp_sites();
    setup_blacklist();
} 

void setup_blacklist(){
    if(file_exists(CFG_IP_BLACKLIST)){
        __Sites += read_big_file(CFG_IP_BLACKLIST);
        __Sites = distinct_array(__Sites);
    }
    //tc("Number of banished ip's: "+sizeof(__Sites));
    save_banish();
}

static private int valid_access(object ob) { 
    return master()->valid_apply( ({ "ASSIST" }) );
} 

int valid_cap_name(string cap, string nom) {
    int i;

    if(convert_name(cap) != nom) return 0;
    if((i = strlen(cap)) > MAX_USER_CAP_NAME_LENGTH) return 0;
    if(strsrch(cap, "'-") != -1) return 0;
    if(strsrch(cap, "' ") != -1) return 0;
    if(strsrch(cap, "''") != -1) return 0;
    if(strsrch(cap, "-'") != -1) return 0;
    if(strsrch(cap, "- ") != -1) return 0;
    if(strsrch(cap, "--") != -1) return 0;
    if(strsrch(cap, " '") != -1) return 0;
    if(strsrch(cap, " -") != -1) return 0;
    if(strsrch(cap, "  ") != -1) return 0;
    if(lower_case(cap[i-1..i-1]) != nom[strlen(nom)-1..strlen(nom)-1])
        return 0;
    return 1;
}

void register_site(string str) { 
    if(!valid_access(previous_object())) return; 
    if(member_array(str, keys(__TmpBanish))!=-1)
        map_delete(__TmpBanish,str);   
    __Sites = distinct_array(__Sites + ({ str }) ); 
    save_banish(); 
} 

void temporary_register(string str, int foo) {
    if(!valid_access(previous_object())) return;
    foo+=time();
    __TmpBanish+=([ str : foo ]);
    __Sites = distinct_array(__Sites + ({str}));
    save_banish();
}

string query_temp_site_info() {
    string info, *site;
    int i;

    if(!valid_access(previous_object())) return "";
    for(i=0,info="";i<sizeof(site=keys(__TmpBanish));i++)
        info+= site[i]+" will expire at "+ctime(__TmpBanish[site[i]])+".\n";
    if(!info) info = "No sites are on temporary registration.";
    return info;
}

void manual_temp_unregister(string str) {

    if(!valid_access(previous_object())) return;
    if(member_array(str, keys(__TmpBanish))==-1) return;
    map_delete(__TmpBanish,str);
    __Sites -= ({str});
    save_banish();
}

void clean_temp_sites() {
    int *times,i;
    string *sites;


    times=values(__TmpBanish);
    sites=keys(__TmpBanish);

    for(i=0;i<sizeof(times);i++) {
        if(times[i]<time()) {
            map_delete(__TmpBanish,sites[i]);
            __Sites -= ({sites[i]});
        }
    }
    save_banish();
    call_out("clean_temp_sites", 900); 
}

void unregister_site(string str) { 
    if(!valid_access(previous_object())) return; 
    __Sites -= ({ str }); 
    save_banish(); 
} 

string *query_registered() { 
    if(!valid_access(previous_object())) return ({});
    return __Sites;
} 

void banish_name(string str) { 
    if(!valid_access(previous_object())) return;
    unguarded( (: log_file, "banish", 
                this_player()->GetName() + " banished " + str + "." :) );
    __Names = distinct_array(__Names + ({ lower_case(str) })); 
    save_banish(); 
} 

void unbanish_name(string str) { 
    if(!valid_access(previous_object())) return; 
    __Names -= ({ lower_case(str) }); 
    save_banish(); 
} 

string *query_banished() { 
    if(!valid_access(previous_object())) return ({}); 
    return __Names; 
} 

void watch_site(string str) { 
    if(!valid_access(previous_object())) return; 
    __WatchSites = distinct_array(__WatchSites + ({ str })); 
    save_banish(); 
} 

void unwatch_site(string str) { 
    if(!valid_access(previous_object())) return; 
    __WatchSites -= ({ str }); 
    save_banish(); 
} 

string *query_watched_sites() { 
    if(!valid_access(previous_object())) return ({}); 
    return __WatchSites; 
} 

void watch_name(string str) { 
    if(!valid_access(previous_object())) return; 
    __WatchNames = distinct_array(__WatchNames + ({ lower_case(str) })); 
    save_banish(); 
} 

void unwatch_name(string str) { 
    if(!valid_access(previous_object())) return; 
    __WatchNames -= ({ lower_case(str) }); 
    save_banish(); 
} 

string *query_watched_names() { 
    if(!valid_access(previous_object())) return ({}); 
    return __WatchNames; 
} 

void allow_name(string str) { 
    if(!valid_access(previous_object())) return; 
    __Allowed = distinct_array(__Allowed + ({ lower_case(str) })); 
    save_banish(); 
} 

void unallow_name(string str) { 
    if(!valid_access(previous_object())) return; 
    __Allowed -= ({ lower_case(str) }); 
    save_banish(); 
} 

string *query_allowed() { 
    if(!valid_access(previous_object())) return ({}); 
    return __Allowed; 
} 

void set_illegal_substring(string str) { 
    if(!valid_access(previous_object())) return; 
    __IllegalSubStrings = distinct_array(__IllegalSubStrings + 
            ({ lower_case(str) })); 
    save_banish(); 
} 

void remove_illegal_substring(string str) { 
    if(!valid_access(previous_object())) return; 
    __IllegalSubStrings -= ({ lower_case(str) }); 
    save_banish(); 
} 

string *query_illegal_substrings() {  
    if(!valid_access(previous_object())) return ({}); 
    else return __IllegalSubStrings; 
} 

void add_guest(string str) { 
    if(!valid_access(previous_object())) return; 
    __Guests = distinct_array(__Guests + ({ lower_case(str) })); 
    save_banish(); 
} 

void remove_guest(string str) { 
    if(!valid_access(previous_object())) return; 
    __Guests -= ({ lower_case(str) }); 
    save_banish(); 
} 

string *query_guests() { 
    if(!valid_access(previous_object())) return ({}); 
    else return __Guests; 
} 

static private void save_banish() { 
    SaveObject(SaveFile);
} 

int GetGuest(string str) { 
    if(base_name(previous_object()) != LIB_CONNECT ) return 0;
    else return (member_array(lower_case(str), __Guests) != -1); 
} 

int valid_name(string str) { 
    int i, x; 

    if(base_name(previous_object()) != LIB_CONNECT ) return 0;
    if(member_array(str, __Names) != -1) return 0; 
    if(!GUEST_ALLOWED && lower_case(str) == "guest") return 0;
    i = sizeof(__IllegalSubStrings);
    while(i--) if(strsrch(str, __IllegalSubStrings[i]) != -1) return 0;
    if((x = strlen(str)) > MAX_USER_NAME_LENGTH) return 0; 
    if(x < MIN_USER_NAME_LENGTH) return 0; 
    for(i=0; i<x; i++)  
        if(str[i] < 'a' || str[i] > 'z') return 0; 
    return 1; 
} 

int eventConnect(string nom, string ip) {
    if(base_name(previous_object()) != LIB_CONNECT ) return 0;
    if(member_array(nom, __WatchNames) != -1) { 
        log_file("watch/names", sprintf("%s from %s at %s\n", nom, ip,
                    ctime(time()))); 
    } 
    if(match_ip(ip, __WatchSites)) { 
        log_file("watch/"+ip, sprintf("%s at %s\n", nom, ctime(time()))); 
    } 
    if(member_array(nom, __Allowed) != -1) { 
        log_file("watch/allowed", sprintf("%s from %s at %s\n", nom, ip,
                    ctime(time()))); 
        __Allowed -= ({ nom }); 
        save_banish(); 
        return 1; 
    }
    if(match_ip(ip, __Sites)) { 
        if(user_exists(nom)) { 
            log_file("watch/"+ip, sprintf("%s allowed in from %s at %s\n",
                        nom, ip, 
                        ctime(time()))); 
            return 1; 
        } 
        else { 
            log_file("watch/"+ip, sprintf("%s failed from %s at %s\n"
                        , nom,
                        ip, 
                        ctime(time()))); 
            return 0; 
        } 
    } 
    return 1; 
} 

static private int match_ip(string ip, string *sites) { 
    foreach(string site in sites) {
        int i;

        if( site == ip ) return 1;
        i = strsrch(site, "*");
        if( i > 0 ) {
            if( ip[0..(i-1)] == site[0..(i-1)] ) return 1;
        }
    }
    return 0;
}
