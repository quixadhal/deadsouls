/*    /adm/sefun/pointers.c
 *    from Dead Souls
 *    sefuns for testing things
 *    created by Descartes of Borg 940213
 */

#include <dirs.h>
#include <config.h>
#include "sefun.h"
#include <council.h>
#include <daemons.h>

int ambassadorp(object ob) {
    if(!ob) ob = previous_object();
    return (userp(ob) && member_group(ob, "AMBASSADOR"));
}

int archp(object ob) {
    string str;

    if(!ob) ob = previous_object();
    if(!creatorp(ob)) return 0;
    return (member_group(ob, "ASSIST") || member_group(ob, "SECURE"));
}

varargs int creatorp(object ob) {
    if(!ob) ob = previous_object();
    if(!ob || !userp(ob)) return 0;
    return !(strsrch(file_name(ob), DIR_CRES));
}

int hiddenp(object ob) {
    if(!objectp(ob)) error("Bad argument 1 to hiddenp().\n");
    return !find_object(file_name(ob));
}

int high_mortalp(object ob) {
    if(!ob) ob = previous_object();
    return (!creatorp(ob) && ((int)ob->GetLevel() > 24));
}

int councilp(object ob) {
    int val;

    if(!ob) ob = previous_object();
    if( ! catch( val = (int)VOTING_D->IsCouncilMember(ob) ) )
        return (!creatorp(ob) && ( val ));
    return 0;
}

int avatarp(object ob) {
    if( !ob ) ob = previous_object();
    return (!creatorp(ob) && (int)ob->GetLevel() > 49);
}

int leaderp(object ob) {
    if(!ob) ob = previous_object();
    if(!userp(ob) || creatorp(ob)) return 0;
    return ((string)ob->GetPosition() != "citizen");
}

varargs int playerp(object ob) {
    if( !ob ) ob = previous_object();
    return (userp(ob) && !creatorp(ob));
}

varargs int newbiep(object ob) {
    if( !ob ) ob = previous_object();
    return (!creatorp(ob) && ((int)ob->GetLevel() <= MAX_NEWBIE_LEVEL));
}
