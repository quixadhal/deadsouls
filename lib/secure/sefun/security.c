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

mixed unguarded(function f) { return (mixed)master()->apply_unguarded(f); }
