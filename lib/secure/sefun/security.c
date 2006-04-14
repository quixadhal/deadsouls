/*    /secure/sefun/security.c
 *    from Dead Souls 3.3
 *    returns the file privs for any file or directory
 *    created by Descartes of Borg 940903
 */

#include <privs.h>

string file_privs(string file) {
    string *path;


    if(!sizeof(path = explode(file, "/"))) return 0;
    switch(path[0]) {
    case "adm": return PRIV_SECURE;
    case "cmds": return PRIV_CMDS;
    case "daemon": return PRIV_MUDLIB;
    case "estates": return PRIV_GENERAL;
    case "lib": return PRIV_GENERAL;
    case "obj": return PRIV_GENERAL;
    case "secure": return PRIV_SECURE;
    case "shadows": return PRIV_GENERAL;
    case "verbs": return PRIV_CMDS;
    case "www": return PRIV_GENERAL;
    case "realms":
	if(sizeof(path) > 1) return lower_case(path[1]);
	else return 0;
    case "domains":
	if(sizeof(path) > 1) return capitalize(lower_case(path[1]));
	else return 0;
    default: return 0;
    }
}

int member_group(mixed who, string grp) {
    string *ppl;

    if(objectp(who)) who = (string)who->GetKeyName();
    if(!(ppl = (string *)master()->query_group(grp))) return 0;
    return (member_array(who, ppl) != -1);
}

int domain_admin(mixed pretender, string domain){
    mapping DomainsMap = ([]);
    string write_perms = read_file("/secure/cfg/write.cfg");
    string *tmp_array = explode(write_perms, "\n");
    string *admin_array = ({});
    if(objectp(pretender)) pretender = pretender->GetKeyName();
    foreach(string line in tmp_array){
	string where, admins;
	if(sscanf(line,"(/domains/%s/) %s", where, admins)){
	    DomainsMap[where] = admins;
	}
    }
    //tc("I think pretender is: "+identify(pretender)+", type: "+typeof(pretender),"yellow");
    if(!sizeof(DomainsMap[domain])) return 0;
    admin_array = explode(DomainsMap[domain],":");
    //tc("will I return 0?");
    if(member_array(lower_case(pretender), admin_array) == -1){
	//tc("about to return 0");
	return 0;
    }
    else {
	//tc("about to return 1");
	return 1;
    }
}

int check_privs(mixed pretender, mixed target){
    int x;
    string domain, junk;
    if(stringp(pretender)) pretender = load_object(pretender);
    if(objectp(target)) target = base_name(target)+".c";
    if(!stringp(target)) x= 1;
    if(!pretender) x= 2;
    if(pretender->GetForced() ) x= 3;
    foreach(object ob in previous_object(-1)){
	if(ob && ob->GetForced() ) x= 4;
    }
    if(first_string_element(target,"/",1) == "tmp" ) x= 15;
    else if(first_string_element(target,"/",1) == "open") x= 16;
    else if(first_string_element(target,"/",1) == "realms" &&
      grepp(target,homedir(pretender)) ) x= 17;
    else if(archp(pretender)) x= 18;
    else x= 9;

    if(stringp(target) && first_string_element(target,"/",1) == "domains"){
	//tc("entered new test sub","blue");
	if(sscanf(target,"/domains/%s/%s", domain, junk) == 2){
	    //tc("entered subsub","blue");
	    //tc("pretender: "+identify(pretender),"white");
	    //tc("domain: "+identify(domain),"white");
	    if(domain_admin(pretender, domain)) x = 19;
	}
    }
    //tc("x: "+x);
    if(x < 10) return 0;
    if(x > 10) return 1;
}

mixed unguarded(function f) { return (mixed)master()->apply_unguarded(f); }
