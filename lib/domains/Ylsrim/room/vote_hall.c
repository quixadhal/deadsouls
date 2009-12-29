/*
 *  /domains/Praxis/room/vote_hall.c
 *  from the Dead Souls mudlib
 *  So that players can vote
 *  by Kalinash on 961026
 */

#include <lib.h>
#include <daemons.h>
#include <voting.h>

inherit LIB_ROOM;

mixed ReadList();

static void create() {
    ::create();
    SetTown("Ylsrim");
    SetClimate("indoors");
    SetProperties( ([ "no kill" : 1, "no attack" : 1, "no steal" : 1,
                "no magic" : 1, "light" : 3, "no bump" : 1, "no teleport" : 1 ]) );
    SetShort("voting hall");
    SetLong("You are in the voting hall of Ylsrim. This is where people come to nominate candidates for class leader and to cast their vote.  There is a list posted on the wall here.");
    SetItems( ([ "list" : "This is the list of candidates." ]) );
    SetRead( "list", (: ReadList :) );
    SetObviousExits("down");
    SetExits( ([ "down" : "/domains/Ylsrim/room/bazaar" ]) );
}

mixed ReadList() {
    string msg;

    if( VOTING_D->GetStatus() == VOTE_NOT_RUNNING ) {
        this_player()->eventPrint("Since the elections are not "
                "currently running, the list is blank.");
        return 1;
    }

    msg = "\tCandidates for Dead Souls Offices\n\n";

    foreach( string sClass in CLASSES_D->GetClasses() ) {
        msg += capitalize( sClass ) + " : ";
        foreach( string sName in VOTING_D->GetCandidates(sClass) )
            msg += capitalize(sName) + ", ";
        if( msg[strlen(msg)-2] == ',' )        
            msg = msg[0..strlen(msg)-3];
        msg += "\n";
    }

    this_player()->eventPrint( msg );
    return 1;
}

mixed CanNominate( object who ) { return 1; }
mixed CanVote( object who ) { return 1; }
mixed CanWithdraw( object who ) { return 1; }

mixed eventNominate( object who, string str ) {
    int iErr;

    if( creatorp( who ) ) {
        who->eventPrint("Creators cannot vote!");
        return 1;
    }

    iErr = VOTING_D->eventAddCandidate( who->GetClass(), str );

    switch( iErr ) {
        case VOTE_NOT_RUNNING :
            this_player()->eventPrint("The elections are not running now!");
            break;

        case VOTE_MODE_VOTING :
            this_player()->eventPrint("The time for nominating "
                    "candidates is past, cast your vote instead.");
            break;

        case VOTE_ERROR :
            this_player()->eventPrint("There was an error, you cannot "
                    "nominate someone at this time.");
            break;

        case VOTE_NOT_CLASS_MEMBER :
            this_player()->eventPrint( capitalize(str) + " is not a member
                    of "
                    "the " + pluralize( who->GetClass() ) + ".");
            break;

        case VOTE_ALREADY_RUNNING :
            this_player()->eventPrint( capitalize(str) + " is already
                    running." );
            break;
    }
    return 1;
}

mixed eventVote( object who, string str ) {
    int    iErr;
    string sClass;

    sClass = who->GetClass();
    iErr = VOTING_D->eventCastVote( sClass, who->GetName(), str );

    switch( iErr ) {
        case VOTE_NOT_RUNNING :
            this_player()->eventPrint("The elections are not running
                    now!");
            break;

        case VOTE_MODE_CANDIDATES :
            this_player()->eventPrint("Voting has not yet started, "
                    "nominate a candidate instead.");
            break;

        case VOTE_NOT_PRIMARY :
            this_player()->eventPrint("Only your primary character can
                    vote.");
            break;

        case VOTE_NOT_CLASS_MEMBER :
            this_player()->eventPrint( str + " is not a candidate for "
                    "the " + pluralize(who->GetClass()) + ".");
            break;

        case VOTE_ALREADY_VOTED :
            this_player()->eventPrint("You have already cast your vote!");
            break;

        case VOTE_SUCCESS :
            this_player()->eventPrint("You cast your vote!");
            break;        
    }

    return 1;
}

mixed eventWithdraw( object who ) {
    int iErr;

    iErr = VOTING_D->eventRemoveCandidate( who->GetClass(), who->GetName()
            );

    switch( iErr ) {
        case VOTE_NOT_RUNNING :
            this_player()->eventPrint("The elections are not running
                    now!");
            break;

        case VOTE_MODE_VOTING :
            this_player()->eventPrint("The elections have begun, it is "
                    "too late to withdraw.");
            break;

        case VOTE_NOT_CANDIDATE :
            this_player()->eventPrint("You are not a candidate.");
            break;
    }

    return 1;
}
void init(){
    ::init();
}
