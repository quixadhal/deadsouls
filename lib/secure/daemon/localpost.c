/*    /daemon/postal/localpost.c
 *    from Foundation II
 *    filter for mails coming in from outside and inside
 *    created by Descartes of Borg 940503
 */

#include <lib.h>
#include <cfg.h>
#include <objects.h>
#include <daemons.h>
#include "localpost.h"

inherit LIB_DAEMON;

static private mapping __MudGroups;

void create() {
    daemon::create();
    SetNoClean(1);
    load_groups();
}

void load_groups() {
    mapping tmp;
    string *lines, *parts;
    string a, b;
    int i, maxi, x, j;

    maxi = sizeof(lines = filter(explode(read_file(CFG_ALIASES), "\n"),
                (: $1 && $1 != "" && $1[0] != '#' :)));
    for(i=0, tmp = ([]); i<maxi; i++) {
        if(sscanf(lines[i], "%s: %s", a, b) != 2) continue;
        if(b[0..0] == "|") {
            x = sizeof(parts = explode(b[1..strlen(b)-1], ";"));
            if(a == "include") {
                switch(sizeof(parts)) {
                    case 2:
                        tmp += call_other(parts[0],parts[1]);
                        break;
                    case 3:
                        tmp+=call_other(parts[0],parts[1],
                                parts[2]);
                        break;
                    default:
                        tmp += call_other(parts[0], parts[1],
                                parts[2], parts[3]);
                        break;
                }
            }
            else switch(sizeof(parts)) {
                case 2: 
                    tmp[a] = call_other(parts[0],parts[1]);
                    break;
                case 3:
                    tmp[a] = call_other(parts[0],parts[1],
                            parts[2]);
                    break;
                default:
                    tmp[a] = call_other(parts[0],parts[1],
                            parts[2], parts[3]);
            }
        }
        else {
            x = sizeof(parts = explode(b, " "));
            if(a != "exclude") tmp[a] = ({});
            for(j=0; j<x; j++) {
                if(a == "exclude" && tmp[parts[j]])
                    map_delete(tmp, parts[j]);
                else if(a == "exclude") continue;
                else if(tmp[parts[j]]) tmp[a]+=tmp[parts[j]];
                else tmp[a] += ({ parts[j] });
            }
        }
    }
    i = sizeof(parts = keys(tmp));
    __MudGroups = ([]);
    while(i--) __MudGroups[lower_case(parts[i])] = tmp[parts[i]];
}

varargs string *send_post(mapping borg, mixed who) {
    mapping remote_mail, grps;
    string *rejects, *tmp, *tmpwho, *cles;
    string msg, a, b, tmpstr;
    int i, j, x;

    if(!who) who = distinct_array(borg["to"] + borg["cc"]);
    tmpstr = base_name(previous_object(0));
    if(tmpstr != OBJ_POST && tmpstr != REMOTEPOST_D && tmpstr != FOLDERS_D)
        return (pointerp(who) ? who : ({ who }));
    rejects = ({});
    grps = OPTIONS_D->query_groups(borg["from"]);
    borg["id"] = LETTERS_D->create_letter(borg["message"]);
    msg = borg["message"];
    map_delete(borg, "message");
    remote_mail = ([]);
    if(pointerp(who)) {
        i = sizeof(who);
        tmpwho = ({});
        x = OPTIONS_D->query_option(borg["from"], "metoo");
        while(i--) {
            if(pointerp(who[i] = map_groups(who[i], grps, borg["from"]))) 
                tmpwho += who[i];
            else tmpwho += ({ who[i] });
        }
        i = sizeof(who = tmpwho);
        while(i--) {
            if(sscanf(who[i], "%s@%s", a, b) == 2) {
                if(!remote_mail[b]) remote_mail[b] = ({ a });
                else remote_mail[b] += ({ a });
            }
            else if(!user_exists(who[i])) rejects += ({ who[i] });
            else FOLDERS_D->add_post(who[i], "new", borg);
        }
        j = sizeof(cles = keys(remote_mail));
        while(j--) 
            if(!(REMOTEPOST_D->send_post(borg+(["message":msg]), cles[j]))){
                rejects += remote_mail[cles[j]];
            }
        return rejects;
    }
    else if(pointerp(tmp = __MudGroups[who]) && (i = sizeof(tmp))) {
        while(i--) {
            if(sscanf(tmp[i], "%s@%s", a, b) == 2) {
                if(!remote_mail[b]) remote_mail[b] = ({});
                remote_mail[b] += ({ tmp[i] });
            }
            else if(!user_exists(tmp[i])) rejects += ({ tmp[i] });
            else FOLDERS_D->add_post(tmp[i], "new", borg);
        }
        i = sizeof(cles = keys(remote_mail));
        while(i--) 
            if(!(REMOTEPOST_D->send_post(borg+(["message":msg]), cles[i])))
                rejects += remote_mail[cles[i]];
        return rejects;
    }
    else if(pointerp(tmp = OPTIONS_D->query_group(who)) &&
            (i = sizeof(tmp))) {
        while(i--) {
            if(sscanf(tmp[i], "%s@%s", a, b) == 2) {
                if(!remote_mail[b]) remote_mail[b] = ({});
                remote_mail[b] += ({ tmp[i] });
            }
            else if(!user_exists(tmp[i])) rejects += ({ tmp[i] });
            else FOLDERS_D->add_post(tmp[i], "new", borg);
        }
        i = sizeof(cles = keys(remote_mail));
        while(i--)
            if(!(REMOTEPOST_D->send_post(borg+(["message":msg]),cles[i])))
                rejects += remote_mail[cles[i]];
        return rejects;
    }
    else {
        if(sscanf(who, "%s@%s", a, b) == 2) {
            if(!(REMOTEPOST_D->send_post(borg+(["message":msg]),b)))
                return ({ who });
        }
        else if(!user_exists(who)) return ({ who });
        else FOLDERS_D->add_post(who, "new", borg);
        return ({});
    }
}

static private mixed map_groups(string str, mapping grps, string who) {
    if(__MudGroups[str]) return __MudGroups[str] - ({ who });
    else if(grps[str] && sizeof(grps[str]))
        return grps[str] - ({ who });
    else return str;
}

string *query_mud_group(string str) { return copy(__MudGroups[str]); }

mapping query_mud_groups() { return copy(__MudGroups); }
