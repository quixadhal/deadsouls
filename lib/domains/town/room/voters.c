#include <lib.h>
#include <daemons.h>
#include <voting.h>

inherit LIB_ROOM;

mixed ReadList();

static void create() {
    room::create();
    SetTown("town");
    SetClimate("indoors");
    SetAmbientLight(30);
    SetShort("Registrar of Voters");
    SetLong("This small office is where people come to perform their civic duty of voting. Here you may also 'nominate CANDIDATE'. There is a list of candidates here. The main area of the town hall is southwest.");
    SetItems( ([ "list" : "This is the list of candidates." ]) );
    SetExits( ([ 
                "southwest" : "/domains/town/room/thall",
                ]) );
    SetRead( "list", (: ReadList :) );
    SetProperties (([
                "no attack":1, 
                "no bump":1, 
                "no steal":0, 
                "no magic":0, 
                "no paralyze":0, 
                "no teleport":0]));
    SetInventory(([
                ]));
}

mixed ReadList() {
    string msg, admin;

    admin = "\nSpecial commands available to admins:\n";
    admin += "tally\t\t\tReports the vote tally.\n";
    admin += "nextday\t\t\tAdvances voting schedule one day.\n";
    admin += "startvote\t\tGet the ball rolling.\n";
    admin += "endvote\t\t\tPrevent additional voting.\n";
    admin += "votestatus\t\tReports status of the vote.\n";
    admin += "votemode\t\tReports operating mode of voting daemon.\n\n";

    if(archp(this_player())) this_player()->eventPrint( admin );

    if( VOTING_D->GetStatus() == VOTE_NOT_RUNNING ) {
        this_player()->eventPrint(VOTING_D->GetCurrentCouncil());
        return 1;
    }

    msg = VOTING_D->GetCurrentCouncil();
    msg += "\n\tCandidates for Dead Souls Offices\n\n";

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
            this_player()->eventPrint( capitalize(str) + " is not a member of "
                    "the " + pluralize( who->GetClass() ) + ".");
            break;

        case VOTE_ALREADY_RUNNING :
            this_player()->eventPrint( capitalize(str) + " is already running." );
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
            this_player()->eventPrint("The elections are not running now!");
            break;

        case VOTE_MODE_CANDIDATES :
            this_player()->eventPrint("Voting has not yet started. We are "
                    "still nominating candidates. Please nominate a candidate "
                    "instead.");
            break;

        case VOTE_NOT_PRIMARY :
            this_player()->eventPrint("Only your primary character can vote.");
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
            this_player()->eventPrint("The elections are not running now!");
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

mixed tally(){
    write("Voting daemon says: \"Votes tallied.\"");
    return VOTING_D->eventTallyVotes();
}
mixed nextday(){
    write("Voting daemon says: \"Voting schedule advanced one day.\"");
    return VOTING_D->eventNextDay();
}
mixed startvote(){
    return VOTING_D->eventStartVoting();
}
mixed endvote(){
    write("Voting daemon says: \"Polls closed.\"");
    return VOTING_D->eventEndVoting();
}
mixed votestatus(){
    write("Voting daemon says: \"Status bitwise operator.\"");
    return VOTING_D->GetStatus();
}
mixed votemode(){
    write("Voting daemon says: \"Mode bitwise result.\"");
    return VOTING_D->GetMode();
}

void init(){
    ::init();
    add_action("tally","tally");
    add_action("nextday","nextday");
    add_action("startvote","startvote");
    add_action("endvote","endvote");
    add_action("votestatus","votestatus");
    add_action("votemode","votemode");
}
