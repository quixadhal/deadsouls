/*    /lib/chapel.c
 *    from the Dead Souls  Object Library
 *    allows people to marry
 *    created by Descartes of Borg 951210
 *    Version: @(#) chapel.c 1.2@(#)
 *    Last modified: 96/10/09
 */

#include <lib.h>
#include "include/chapel.h"

inherit LIB_ROOM;

private int AllowSacrifice, SacrificeType;
private string array Classes, Religion, Deities, DeityIds;

/*     **********  /lib/chapel.c apply methods  **********     */
static void create(){
    room::create();
    Classes = ({});
    Religion = allocate(2);
    Deities = ({});
    DeityIds = ({});
    AllowSacrifice = 0;
    SacrificeType = 0;
}

/*     **********  /lib/chapel.c modal methods  **********     */
mixed CanMarry(object who, object spouse1, object spouse2){
    mixed tmp;

    if( (tmp = spouse1->CanMarry(who, spouse2)) != 1 ){
        if( tmp ) return tmp;
        else return spouse1->GetName() + " cannot be married.";
    }
    if( (tmp = spouse2->CanMarry(who, spouse1)) != 1 ){
        if( tmp ) return tmp;
        else return spouse2->GetName() + " cannot be married.";
    }
    if( archp(who) ){
        return 1;
    }
    return 1;
}

    mixed CanSacrifice(object who, object what, string deus){
        if( who->GetReligion(1) != Religion[1] )
            return "You must hold the beliefs of " + Religion[1] + " to do that.";
        if( !(what->GetVendorType() & SacrificeType) )
            return "You cannot sacrifice that here.";
        if( member_array(deus, DeityIds) == -1 )
            return "You do not worship anything called \"" + deus + "\".";
        return AllowSacrifice;
    }

mixed eventMarry(object who, object spouse1, object spouse2){
    mixed tmp;

    if( (tmp = spouse1->eventMarry(who, spouse2)) != 1 ) return tmp;
    if( (tmp = spouse2->eventMarry(who, spouse1)) != 1 ){
        spouse1->SetMarried(0);
        return tmp;
    }
    spouse1->eventPrint(who->GetName() + " weds you to " +
            spouse2->GetName() + ".");
    spouse2->eventPrint(who->GetName() + " weds you to " +
            spouse1->GetName() + ".");
    who->eventPrint("You join " + spouse1->GetName() + " to " +
            spouse2->GetName() + " in marriage.");
    this_object()->eventPrint(who->GetName() + " joins " +
            spouse1->GetName() + " and " +
            spouse2->GetName() + ".",
            ({ spouse1, spouse2, who }));
    who->AddSkillPoints("faith", random(100));
    return 1;
}

mixed eventSacrifice(object who, object what, string deus){
    who->eventPrint("You attempt to make a sacrifice, but nothing happens.");
    return 1;
}

/*     **********  /lib/chapel.c event methods  **********     */
int SetAllowSacrifice(int x){ return (AllowSacrifice = x); }

int GetAllowSacrifice(){ return AllowSacrifice; }

string *SetClasses(string *rc){ return (Classes = rc); }

string *GetClasses(){ return Classes; }

string *SetDeities(string *deities){
    string *ids = ({});

    Deities = deities;
    foreach(string deus in deities)
        ids += explode(lower_case(deus), " ");
    DeityIds = ids;
    return Deities;
}

string *GetDeities(){ return Deities; }

string *SetReligion(string adj, string noun){
    Religion[0] = adj;
    Religion[1] = noun;
    return Religion;
}

varargs string GetReligion(int flag){ return Religion[flag]; }

int SetSacrificeType(int x){ return (SacrificeType = x); }

int AddSacrificeType(int x){ return (SacrificeType |= x); }

int GetSacrificeType(){ return SacrificeType; }
