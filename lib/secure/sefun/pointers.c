/*    /adm/sefun/pointers.c
 *    from Dead Souls
 *    sefuns for testing things
 *    created by Descartes of Borg 940213
 */

#include <dirs.h>
#include "sefun.h"
#include <council.h>
#include <daemons.h>

int ambassadorp(object ob) {
    if(!ob) ob = previous_object();
    return (userp(ob) && member_group(ob, "AMBASSADOR"));
}

int elderp(object ob) {
    if(!ob) ob = previous_object();
    return (userp(ob) && member_group(ob, "ELDER"));
}

int testp(object ob) {
    if(!ob) ob = previous_object();
    return (userp(ob) && member_group(ob, "TEST"));
}

int archp(object ob) {
    if(!ob) ob = previous_object();
    if(!creatorp(ob)) return 0;
    return (member_group(ob, "ASSIST") || member_group(ob, "SECURE"));
}

int adminp(object ob){
    return archp(ob);
}

int imud_privp(mixed guy) {
    if(member_group(guy, "INTERMUD")) return 1;
    else return 0;
}

int telnet_privp(mixed guy) {
    if(member_group(guy, "TELNET")) return 1;
    else return 0;
}

int securep(mixed guy) {
    mixed dude = guy;
    if(!guy) guy = previous_object();
    if(!stringp(guy)) guy = guy->GetKeyName();
    if(!guy || guy == "") guy = base_name(dude);
    if(member_group(guy, "SECURE")) return 1;
    else return 0;
}

int assistp(mixed guy) {
    mixed dude = guy;
    if(!guy) guy = previous_object();
    if(!stringp(guy)) guy = guy->GetKeyName();
    if(!guy || guy == "") guy = base_name(dude);
    if(member_group(guy, "ASSIST")) return 1;
    else return 0;
}

varargs int creatorp(object ob) {
    if(!ob) ob = previous_object();
    if(!ob || !userp(ob)) return 0;
    return !(strsrch(file_name(ob), DIR_CRES));
}

varargs int wizardp(object ob){
    return creatorp(ob);
}

int builderp(object ob) {
    if(!ob) ob = previous_object();
    if(creatorp(ob)) return 1;
    return (userp(ob) && member_group(ob, "BUILDER"));
}

int snooperp(mixed guy) {
    mixed dude = guy;
    if(!guy) guy = previous_object();
    if(!stringp(guy)) guy = guy->GetKeyName();
    if(!guy || guy == "") guy = base_name(dude);
    if(member_group(guy, "SNOOPER")) return 1;
    else return 0;
}

int hiddenp(object ob) {
    if(!objectp(ob)) error("Bad argument 1 to hiddenp().\n");
    return !find_object(file_name(ob));
}

int high_mortalp(object ob) {
    if(!ob) ob = previous_object();
    return (!creatorp(ob) && (ob->GetLevel() > 24));
}

int councilp(object ob) {
    int val;

    if(!ob) ob = previous_object();
    if( ! catch( val = VOTING_D->IsCouncilMember(ob) ) )
        return (!creatorp(ob) && ( val ));
    return 0;
}

int avatarp(object ob) {
    if( !ob ) ob = previous_object();
    return (!creatorp(ob) && ob->GetLevel() > 49);
}

int leaderp(object ob) {
    if(!ob) ob = previous_object();
    if(!userp(ob) || creatorp(ob)) return 0;
    return (ob->GetPosition() != "citizen");
}

varargs int playerp(object ob) {
    if( !ob ) ob = previous_object();
    return (userp(ob) && !creatorp(ob));
}

varargs int newbiep(object ob) {
    if(!MAX_NEWBIE_LEVEL) return 0;
    if( !ob ) ob = previous_object();
    return (!creatorp(ob) && (ob->GetLevel() <= MAX_NEWBIE_LEVEL));
}

varargs int estatep(object ob){
    if( !ob ) ob = previous_object();
    return (!(strsrch(base_name(ob), ESTATES_DIRS)));
}
