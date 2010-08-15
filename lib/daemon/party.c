/*    /daemon/party.c
 *    from the Dead Souls  Object Library
 *    manages parties
 *    created by Descartes of Borg 951120
 */

#include <lib.h>
#include <save.h>
#include <daemons.h>
#include <message_class.h>
#include "include/party.h"

inherit LIB_DAEMON;

mapping Parties;
static string SaveFile;
string *old_parties;

static void create() {
    daemon::create();
    SaveFile = save_file(SAVE_PARTIES);
    Parties = ([]);
    old_parties = ({});
    if( unguarded((: file_exists(SaveFile) :)) ){
        RestoreObject(SaveFile);
    }
    SetNoClean(1);
    this_object()->eventCleanParties();
    set_heart_beat(60);
    SaveObject(SaveFile);
}

static void eventDestroyParty(string party){
    if(!party || !Parties[party]) return;
    foreach(mixed member in this_object()->GetPartyMembers(party)){
        if(member){
            member->SetParty(0); 
        }
    }
    if(member_array(party, CHAT_D->GetLocalChannels()) != -1){
        CHAT_D->eventSendChannel("System", party, "The party " + party + 
          " has been disbanded.");
        CHAT_D->RemoveLocalChannel(party);
    }
    map_delete(Parties, party);
    old_parties = distinct_array( (old_parties || ({})) + ({ party }) );
    SaveObject(SaveFile);
}

string *GetOldParties(){
    return sort_array((copy(old_parties || ({})) - keys(Parties || ([]))),1);
}

static void eventCleanParties(){
    foreach(mixed key, mixed val in Parties){
        mixed array members = this_object()->GetPartyMembers(key);
        members -= ({ 0 });
        if(!sizeof(members)) eventDestroyParty(key);
    }
}

void heart_beat(){
    eventCleanParties();
}

mixed array GetParties(){
    return keys(Parties);
}

mapping GetPartiesMap(){
    return copy(Parties);
}

mixed CanChangeLeader(object who, object targ) {
    class party p;
    string pname;

    if( !(p = Parties[pname]) ) return "No such party exists.";
    if( p->Leader != who )
        return "You must be the party leader in order to change leaders.";
    if( member_array(targ, p->Members) == -1 )
        return targ->GetName() + " is not in the party.";
    SaveObject(SaveFile);
    return 1;
}

mixed CanCreateParty(object who, string name) {
    string *tmpnames = ( CHAT_D->GetChannels() || ({}) );
    tmpnames += ({ "party", "lemon" });
    if( Parties[name] ) return "A party by that name already exists.";
    if( member_array(name, tmpnames) != -1 )
        return "You cannot use the name " + name + " for your party.";
    if( who->GetParty() ) return "You are already in a party!";
    eventSave();
    return 1;
}

mixed CanInviteMember(object who, object member) {
    string pname;

    pname = who->GetParty();
    if( !Parties[pname] ) return "There is no such party!";
    if( ((class party)Parties[pname])->Leader != who )
        return "Only the party leader may add members!";
    if( member->GetParty() )
        return member->GetName() + " is already in a party.";
    if( environment(member) != environment(who) )
        return member->GetName() + " must be somewhere near you.";
    return 1;
}

mixed CanJoinParty(object who, string pname) {
    if( !Parties[pname] ) return "There is no such party.";
    if( member_array(who, ((class party)Parties[pname])->Invited) == -1 )
        return "You have not been invited to join that party.";
    if( who->GetParty() ) return "You are already in a party.";
    if( environment(who) != environment(((class party)Parties[pname])->Leader) )
        return "You are nowhere the leader of the party.";
    return 1;
}

mixed CanLeaveParty(object who) {
    class party p;
    string pname;

    pname = who->GetParty();
    if( !pname || !(p = Parties[pname]) ) return "There is no such party.";
    if( member_array(who, p->Members) == -1 )
        return "You are not in that party.";
    return 1;
}

mixed CanRemoveMember(object who, object targ) {
    class party p;
    string pname;

    pname = who->GetParty();
    if( !(p = Parties[pname]) ) return "There is no such party.";
    if( p->Leader != who ) return "Only the party leader may remove people.";
    return 1;
}	

mixed CanRemoveParty(object who) {
    class party p;
    string pname;

    pname = who->GetParty();
    if( !(p = Parties[pname]) ) return "There is no such party!";
    if( p->Leader != who )
        return "Only the party leader may disband the party.";
    return 1;
}

mixed eventChangeLeader(object who, object targ) {
    class party p;
    string pname;

    pname = who->GetParty();
    p = Parties[pname];
    p->Leader = targ;
    CHAT_D->eventSendChannel("System", pname, targ->GetName() + " is now "
            "the leader.");
    SaveObject(SaveFile);
    return 1;
}

mixed eventCreateParty(object who, string name) {
    class party this_party;

    if( who->SetParty(name) != name )
        return "There was some bizarre problem sticking you in a party.";
    this_party = new(class party);
    this_party->Leader = who;
    this_party->Members = ({ who });
    this_party->Invited = ({});
    Parties[name] = this_party;
    who->eventPrint("Party " + name + " successfully created.", MSG_SYSTEM);
    SaveObject(SaveFile);
    return 1;
}

mixed eventInviteMember(object who, object targ) {
    class party this_party;
    string name;

    name = who->GetParty();
    this_party = Parties[name];
    this_party->Invited += ({ targ });
    CHAT_D->eventSendChannel("System", name, targ->GetName() +
            " has been invited to join the party.");    
    call_out((: RemoveInvitiation :), 60, name, targ);
    targ->eventPrint("You have been invited to join the party \"" + name +
            "\".\nType \"party join " + name + "\" in 60 "
            "seconds to join.", MSG_SYSTEM);
    SaveObject(SaveFile);
    return 1;
}

mixed eventJoinParty(object who, string name) {
    class party this_party;
    mixed tmp;

    if( (tmp = CanJoinParty(who, name)) != 1 ) return tmp;
    this_party = Parties[name];
    if( who->SetParty(name) != name )
        return "Bogus error in joining party.";
    this_party->Invited -= ({ who });
    this_party->Members += ({ who });
    CHAT_D->eventSendChannel("System", name, who->GetName() +
            " has joined the party.");    
    SaveObject(SaveFile);
    return 1;
}

mixed eventLeaveParty(object who) {
    mixed ret = eventRemoveMember(who, who);
    SaveObject(SaveFile);
    return ret;
}

mixed eventRemoveMember(object who, object targ) {
    class party p;
    object ob = 0;
    string name;

    p = Parties[name = who->GetParty()];
    if( targ == p->Leader ) {
        if( sizeof(p->Members) == 1 ) return eventRemoveParty(who);
        else {
            foreach(ob in p->Members) if( ob != targ ) break;
            if( !ob ) return eventRemoveParty(who);
            else {
                p->Leader = ob;
                ob->eventPrint("You are now the leader of the party " + name +
                        ".", MSG_SYSTEM);
            }
        }
    }
    targ->SetParty(0);
    if( Parties[name] ) {
        p->Members -= ({ targ });
        CHAT_D->eventSendChannel("System", name, targ->GetName() +
                " is no longer in the party.");
    }
    targ->eventPrint("You are no longer a member of the party " + name +
            ".", MSG_SYSTEM);
    SaveObject(SaveFile);
    return 1;
}

mixed eventRemoveParty(object who) {
    string name = who->GetParty();
    eventDestroyParty(name);
    return 1;
}

object GetPartyLeader(string name) {
    class party p;
    string nom;

    foreach(nom, p in Parties) if( nom == name ) return p->Leader;
    return 0;
}

object *GetPartyMembers(string name) {
    class party p;
    string nom;

    foreach(nom, p in Parties) if( nom == name ) return p->Members;
    return 0;
}

static void RemoveInvitiation(string name, object who) {
    if( !Parties[name] ) return;
    if( member_array(who, ((class party)Parties[name])->Invited) == -1 )
        return;
    ((class party)Parties[name])->Invited -= ({ who });
}
