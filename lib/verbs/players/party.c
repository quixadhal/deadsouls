/*    /verbs/players/party.c
 *    from the Dead Souls  Object Library
 *    party
 *    party WRD
 *    party WRD WRD
 *    party WRD LIV
 *    created by Descartes of Borg 951121
 */

#pragma save_binary

#include <lib.h>
#include <daemons.h>
#include "include/party.h"

inherit LIB_VERB;

static void create() {
    verb::create();
    SetVerb("party");
    SetRules("WRD", "WRD WRD", "WRD LIV");
    SetErrorMessage("You must specify an action, see \"help party\".");
}

int livings_are_remote() { return 1; }

mixed can_party_wrd(string cmd) {
    if( !cmd ) return 0;
    switch( cmd ) {
	case "disband":
	return (mixed)PARTY_D->CanRemoveParty(this_player());

	case "leave":
	return (mixed)PARTY_D->CanLeaveParty(this_player());

	default:
	return "That is an unknown party action.";
    }
}

mixed can_party_wrd_wrd(string cmd, string party) {
    if( !cmd || !party ) return 0;
    switch(cmd) {
	case "create":
	if( creatorp(this_player()) ) return "Not gonna happen.";
	return (mixed)PARTY_D->CanCreateParty(this_player(), party);

	case "join":
	return (mixed)PARTY_D->CanJoinParty(this_player(), party);

	default:
	return "That is an unknown party action.";
    }
}

mixed can_party_wrd_liv(string cmd) {
    if( !cmd ) return 0;    
    switch(cmd) {
	case "invite":
	case "leader":
	case "remove":
	return 1;

	default:
	return "That is an unknown party action.";
    }
}

mixed do_party_wrd(string cmd) {
    switch(cmd) {
	case "disband":
	return (mixed)PARTY_D->eventRemoveParty(this_player());

	case "leave":
	return (mixed)PARTY_D->eventLeaveParty(this_player());
    }
}

mixed do_party_wrd_wrd(string cmd, string party) {
    switch(cmd) {
	case "create":
	return (mixed)PARTY_D->eventCreateParty(this_player(), party);

	case "join":
	return (mixed)PARTY_D->eventJoinParty(this_player(), party);
    }
}

mixed do_party_wrd_liv(string cmd, object targ) {
    mixed foo;
    switch(cmd) {
        case "invite":
        foo = (mixed)PARTY_D->CanInviteMember(this_player(), targ);
        break; 
        case "leader":
        foo = (mixed)PARTY_D->CanChangeLeader(this_player(), targ);
        break;    
        case "remove":
        foo = (mixed)PARTY_D->CanRemoveMember(this_player(), targ);
        break;
        default:
        return "That is an unknown party action.";
    }
    if( stringp(foo) ) 
       return this_player()->eventPrint(foo), 1;    
    if( !foo || !intp(foo) ) return foo;
    switch(cmd) {
	case "invite":
	return (mixed)PARTY_D->eventInviteMember(this_player(), targ);

	case "leader":
	return (mixed)PARTY_D->eventChangeLeader(this_player(), targ);
	
	case "remove":
	return (mixed)PARTY_D->eventRemoveMember(this_player(), targ);
    }
}

string GetHelp(string str) {
    return ("Syntax: <party create PARTY>\n"
	    "        <party disband>\n"
	    "        <party invite PLAYER>\n"
	    "        <party join PARTY>\n"
	    "        <party leader PLAYER>\n"
	    "        <party leave>\n"
	    "        <party remove PLAYER>\n\n"
	    "This command is the interface to the Dead Souls party system.  "
	    "A party is simply a group of players who are adventuring "
	    "together.  The advantage of a party is that it allows people "
	    "to share the bounties of a quest or other such venture "
	    "together.\n\nIt is important that this party system is extremely "
	    "different from other party systems.  The first thing is that "
	    "it does not support the sharing of experience, since players "
	    "have no experience.  It allows such things as group "
	    "responses to danger and group completions of quests.  For "
	    "example, if the party completes a quest, each member of the "
	    "party is credited with completing the quest.  Note that in "
	    "order to get such credit, the party must be together upon "
	    "completion of the quest.\n\n");
}

	    
