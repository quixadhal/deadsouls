/*
 *  /secure/daemon/voting.c
 *  from the Dead Souls mudlib
 *  Handle player elections
 *  by Kalinash on 961026
 *  Version : @(#) voting.c 1.3@(#)
 *  Last Modified : 96/10/26
 */

#include <lib.h>
#include <daemons.h>
#include <voting.h>
#include <clock.h>

inherit LIB_DAEMON;

private mapping mapVoting;
private mapping mapCouncil;
static string SaveFile;

static void create() {
    daemon::create();
    SaveFile = save_file(SAVE_VOTES);
    SetNoClean(1);
    if( file_exists( SaveFile ) ) {
        RestoreObject(SaveFile);
        if( mapVoting["status"] == VOTE_RUNNING )
            call_out( (: eventNextDay :), DAY );
    }

    if( ! mapVoting ) {
        mapVoting = ([]);
        mapVoting["candidates"] = ([]);
        mapVoting["votes"] = ([]);
        mapVoting["voted"] = ({});
        foreach( string sClass in CLASSES_D->GetClasses() ) {
            mapVoting["candidates"][sClass] = ({});
            mapVoting["votes"][sClass] = ([]);
        }
        mapVoting["status"] = VOTE_NOT_RUNNING;
    }
    if( ! mapCouncil ) 
        mapCouncil = ([]);

    eventSave();
}

mixed eventSave() {
    SaveObject(SaveFile);
    return VOTE_SUCCESS;
}

mixed eventStartVoting() {
    if( GetStatus() == VOTE_RUNNING )
        return VOTE_ALREADY_RUNNING;

    mapVoting["candidates"] = ([]);
    mapVoting["votes"] = ([]);
    mapVoting["voted"] = ({});
    foreach( string sClass in CLASSES_D->GetClasses() ) {
        mapVoting["candidates"][sClass] = ({});
        mapVoting["votes"][sClass] = ([]);
    }

    mapVoting["status"] = VOTE_RUNNING;
    mapVoting["mode"] = VOTE_MODE_CANDIDATES;
    mapVoting["daycount"] = VOTE_DAY_COUNT;
    call_out( (: eventNextDay :), DAY );
    message("shout", "%^YELLOW%^Election announcement:%^RESET%^ " +
            "Class elections have now begun! Go to the voting booth " +
            "and nominate candidates.", users() );
    eventSave();
    return VOTE_SUCCESS;
}

mixed eventEndVoting() {
    mapVoting["status"] = VOTE_NOT_RUNNING;
    eventTallyVotes();
    return eventSave();

}

mixed eventAddCandidate( string sClass, string sWho ) {
    object ob;

    sWho = convert_name( sWho );
    ob = find_player( sWho );

    if( GetStatus() == VOTE_NOT_RUNNING )
        return VOTE_NOT_RUNNING;
    if( mapVoting["mode"] != VOTE_MODE_CANDIDATES )
        return VOTE_MODE_VOTING;
    if( ! ob ) return VOTE_ERROR;
    if( ! ob->ClassMember(sClass) )
        return VOTE_NOT_CLASS_MEMBER;

    foreach( string cls in CLASSES_D->GetClasses() )            
        if( member_array( sWho, mapVoting["candidates"][cls] ) != -1 )
            return VOTE_ALREADY_RUNNING;

    mapVoting["candidates"][sClass] += ({ sWho });
    mapVoting["votes"][sClass][sWho] = 0;
    message("shout", "%^YELLOW%^Election announcement:%^RESET%^ " + 
            ob->GetName() + " has been nominated for the leader of the " +
            pluralize(capitalize(sClass)) + ".", users() );
    eventSave();
    return VOTE_SUCCESS;
}

mixed eventRemoveCandidate( string sClass, string sWho ) {
    sWho = convert_name( sWho );

    if( GetStatus() == VOTE_NOT_RUNNING )
        return VOTE_NOT_RUNNING;
    if( mapVoting["mode"] != VOTE_MODE_CANDIDATES )
        return VOTE_MODE_VOTING;
    if( member_array( sWho, mapVoting["candidates"][sClass] ) == -1 )
        return VOTE_NOT_CANDIDATE;

    mapVoting["candidates"][sClass] -= ({ sWho });
    map_delete( mapVoting["votes"][sClass], sWho );
    message("shout", "%^YELLOW%^Election announcement:%^RESET%^ " + 
            find_player(sWho)->GetName() + " has withdrawn from the " +
            "elections!", users() );
    eventSave();
    return VOTE_SUCCESS;
}

mixed eventCastVote( string sClass, string sVoter, string sVotee ) {
    mapping mapChar;

    sVoter = convert_name( sVoter );
    sVotee = convert_name( sVotee );

    if( GetStatus() == VOTE_NOT_RUNNING )
        return VOTE_NOT_RUNNING;

    if( mapVoting["mode"] != VOTE_MODE_VOTING )
        return VOTE_MODE_CANDIDATES;

    mapChar = CHARACTER_D->GetLink( sVoter );

    if( mapChar )
        if( mapChar["primary"] != sVoter )
            return VOTE_NOT_PRIMARY;

    if( member_array( sVotee, mapVoting["candidates"][sClass] ) == -1 )
        return VOTE_NOT_CLASS_MEMBER;

    if( member_array( sVoter, mapVoting["voted"] ) != -1 )
        return VOTE_ALREADY_VOTED;

    mapVoting["voted"] += ({ sVoter });
    mapVoting["votes"][sClass][sVotee]++;
    eventSave();
    return VOTE_SUCCESS;
}

mixed eventNextDay() {
    mapVoting["daycount"]--;

    if( !mapVoting["daycount"] ) {
        if( mapVoting["mode"] == VOTE_MODE_CANDIDATES ) {
            mapVoting["mode"] = VOTE_MODE_VOTING;
            mapVoting["daycount"] = VOTE_DAY_COUNT;
            call_out( (: eventNextDay :), DAY );
            message("shout", "%^YELLOW%^Election announcement:%^RESET%^ " +
                    "Nominations are closed, go vote for the candidates!!", users() );
            eventSave();
            return VOTE_SUCCESS;
        }
        eventEndVoting();
        return VOTE_SUCCESS;
    }

    call_out( (: eventNextDay :), DAY );
    eventSave();
    return VOTE_SUCCESS;
}

mixed eventTallyVotes() {
    mapping mapWho;
    string  *asWho;

    mapCouncil = ([]);

    foreach( string sClass in CLASSES_D->GetClasses() ) {
        mapWho = mapVoting["votes"][lower_case(sClass)];
        if( ! ( sizeof( asWho = keys( mapWho ) ) ) ){
        }

        while( sizeof( asWho ) >= 2 ) {
            string player1, player2;

            player1 = asWho[0];
            player2 = asWho[1];

            if( mapWho[player1] > mapWho[player2] )
                asWho -= ({ player2 });
            else
                asWho -= ({ player1 });
        }

        if(asWho && sizeof(asWho)) mapCouncil[lower_case(sClass)] = asWho[0];
    }

    eventSave();
    return VOTE_SUCCESS;
}

int GetStatus() { return mapVoting["status"]; }
int GetMode() { return mapVoting["mode"]; }

string *GetCandidates( string sClass ) {
    return mapVoting["candidates"][lower_case(sClass)];
}

string *GetCouncil() {
    string *asWho;

    asWho = ({});

    foreach( string sClass in CLASSES_D->GetClasses() )
        asWho += ({ mapCouncil[sClass] });

    return asWho;
}

string GetCouncilMember( string sClass ) { 
    if(mapCouncil[sClass]) return mapCouncil[sClass];
    else return "";
}

string GetCurrentCouncil(){
    string ret = "Current council:\n";
    foreach( string sClass in CLASSES_D->GetClasses() ){
        string councillor;
        if(mapCouncil[sClass]) councillor = mapCouncil[sClass];
        else councillor = "NONE";
        ret += capitalize(sClass)+": "+capitalize(councillor)+"\n";
    }
    return ret;
}

int GetVoteStatus( object ob ) {
    string sVoter;
    mapping mapChar;

    sVoter = convert_name( ob->GetName() );

    if( mapVoting["status"] == VOTE_NOT_RUNNING )
        return VOTE_ALREADY_VOTED;
    if( mapVoting["mode"] == VOTE_MODE_CANDIDATES )
        return VOTE_ALREADY_VOTED;
    if( member_array( sVoter, mapVoting["voted"] ) != -1 )
        return VOTE_ALREADY_VOTED;
    if( ! ob->GetClass() )
        return VOTE_ALREADY_VOTED;
    mapChar = CHARACTER_D->GetLink( sVoter );

    if( mapChar )
        if( mapChar["primary"] != sVoter )
            return VOTE_ALREADY_VOTED;

    return VOTE_SUCCESS;
}

string GetVoteRoom() { return VOTE_ROOM; }

int IsCouncilMember( object ob ) {
    foreach( string sClass in CLASSES_D->GetClasses() )
        if( mapCouncil[sClass] == convert_name(ob->GetName()) )
            return 1;
    return 0;
}

mixed GetTest() {
    foreach( string sClass in CLASSES_D->GetClasses() )
        this_player()->eventPrint( sClass + " : " + mapCouncil[sClass] );
    return 1;
}

