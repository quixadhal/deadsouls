/*
// File     :  /domains/Praxis/council.c
// Comment  :  Town Council Chambers
// 94-07-02 :  Pallando @ Nightmare wrote it
// 94-07-06 :  Manny @ Nightmare added descriptions
// 94-08-18 :  Forlock @ Nightmare added descriptions of plaque & law book
 */
#include <lib.h>
#include <dirs.h>
#include <objects.h>
#include <daemons.h>

#define TOWN_HALL "/domains/Praxis/town_hall"
#define HELP_FILE "/domains/Praxis/commands"
#define TMP_FILE DIR_TMP + "/" + (this_player()->query_name()) + ".proposal"
#define QUORUM 5
#define PASSES 5

inherit LIB_ROOM;

mapping policies = ([]);

void create()
{
    ::create();
    enable_commands(); // Used to echo tells to the Town Hall
    SetProperties( ([
                "mountable" : 1,
                "indoors" : 1,
                "light" : 2,
                "no castle" : 1,
                "no teleport" : 1,
                "no bump" : 1,
                "no attack" : 1,
                "no steal" : 1,
                "no magic" : 1,
                ]) );
    SetShort( "The Chambers of the High Council" );
    SetLong(
            "The lights are darkened in this room, a set of 12 candles set along a "
            "long, narrow table are the only light.  The table is constructed of the "
            "purest black obsidian, and is surrounded at all sides by wooden chairs, "
            "each covered in a bright pattern of rubies.  On the walls of the room hang "
            "the flags of each of the classes of Nightmare, each created by the best "
            "artists in the land.  The ceiling is covered in 4 ornate patterns, each "
            "a picture of one of the Head Archwizards of the land.  The pictures give "
            "the appearance of watching all that occurs within this chamber.  "
            "Sitting on a pedestal made completely of platinum is the Book of Law.  "
            "A plaque is attached to the west wall. "
           );
    SetExits( ([
                "down"  : "/domains/Praxis/town_hall",
                ]) );
    SetItems( ([
                "view" : "A magnificent panorama of all Praxis.",
                ({ "candles", "candle" }) : "Tall, thin white candles on a "+
                "long, narrow table.",
                "table" : "A brightly polished table of deep black obsidian",
                "obsidian" : "The obsidian is of the purest in the land.",
                ({ "chair", "chairs" }) : "Each of the chairs have a unique pattern of "+
                "rubies engrained within the body of the chair.",
                "rubies" : "Large rubies of the rarest kind.",
                "flags" : "6 flags, each representing the individual classes "
                "of the land.",
                "ceiling" : "A vast mural of the Four greatest in "
                "all the land, Forlock, Descartes, Lassondra, "
                "and Shadowwolf.",
                "plaque" : "A discreet piece of granite with finly etched words.",
                "book"   : "The Book of Law.  Its cover is made of pure solid gold "
                "and the pages of the finest leather.  Inside, one can find "
                "details of the Laws of the Land.",
                "forlock" : "The Great Enforcer, donning a great black robe "
                "lined with a deep red sheath.  The deep stare of "
                "Forlock fills you with all that is good and just.",
                                           "descartes" : "Descartes of Borg, in his most noble position "
                                               "upon a great white horse, is the embodiment of "
                                               "great wisdom and knowlege.",
                                           "lassondra" : "Long, black wavy hair drape down upon a "
                                               "great green cloak upon her broad sholders.  "
                                               "The warm smile and loving eyes depict equality "
                                               "and fairness for all.",
                                           "shadowwolf" : "Donning a long hooded robe, Shadowwolf is "
                                               "perhaps the most mysterious of the four.  "
                                               "Shadowwolf's eyes faintly pierce the darkness "
                                               "that surrounds him, seemingly staring "
                                               "directly at you."
                                               ]) );
}

void init()
{
    ::init();
    add_action( "cmd_propose", "propose" );
    add_action( "cmd_repeal", "repeal" );
    add_action( "cmd_cast", "cast" );
    add_action( "cmd_read", "read" );
    add_action( "cmd_law", "law" );
}

int quorum()
{
    object *voters;

    voters = all_inventory( this_object() );
    voters = filter_array( voters, "leaderp", find_object( OB_SIMUL_EFUN ) );
    return sizeof( voters );
}

int votes_needed( string title )
{
    return PASSES + sizeof( policies[title]["Nay"] ) -
        sizeof( policies[title]["Aye"] );
}

int vetos_needed( string title )
{
    return PASSES + sizeof( policies[title]["Aye"] ) -
        sizeof( policies[title]["Nay"] );
}

varargs void display_policy( mapping policy, int as_law )
{
    int then;

    if( as_law )
    {
        then = policy["Time"];
        printf( "As of %s, %d %s, %d NM;  at the urging of %s\n\n\t\t\%s\n\n%s",
                day( then ), date( then ), month( then ), year( then ),
                capitalize( policy["Proposer"] ), policy["Title"], policy["Text"] );
        return;
    }
    printf(
            "Title: %s\nProposer: %s\n\n%s",
            policy["Title"],
            capitalize( policy["Proposer"] ),
            policy["Text"],
          );
    if( sizeof( policy["Aye"] ) )
        write( wrap( "In favour: " + implode( policy["Aye"], ", " ) ) );
    if( sizeof( policy["Nay"] ) )
        write( wrap( "Against: " + implode( policy["Nay"], ", " ) ) );
    if( sizeof( policy["Abs"] ) )
        write( wrap( "Abstentions: " + implode( policy["Abs"], ", " ) ) );
}

int cmd_law( string arg )
{
    mapping policy;

    if( arg )
    {
        if( undefinedp( policy = POLITICS_D-> query_policy( arg ) ) )
            write( "There is no law: " + arg + "\n" );
        else
            display_policy( policy, 1 );
    } else {
        printf( "The book of law contains the following:\n  %s\n",
                wrap( implode( sort_array( keys(
                                POLITICS_D-> query_policies() ), 1 ), "\n  " ) ) );
    }
    return 1;
}

int cmd_read( string arg )
{
    mapping policy;

    if( arg )
    {
        if( arg == "book" || arg == "law" )
            write( "Use the \"law\" command.\n" );
        else if( arg == "plaque" )
            write( read_file( HELP_FILE ) );
        else if( undefinedp( policy = policies[arg] ) )
            write( "There is no proposal: " + arg + "\n" );
        else
            display_policy( policy );
    } else {
        if( !sizeof( policies ) )
            write( "There are no proposals waiting to be passed.\n" );
        else
            write( "The following proposals are waiting to be passed:\n" +
                    wrap( implode( keys( policies ), ", " ) ) );
    }
    return 1;
}

int cmd_propose( string title )
{
    if( !title ) return notify_fail( "Syntax: propose <title>\n" );
    title = lower_case( title );
    if( policies[title] || POLITICS_D-> query_policy( title ) )
        return notify_fail( "That already exists.\n" );
    if( ( quorum() < QUORUM ) && !archp( this_player() ) )
        return notify_fail( "There are not " + QUORUM + " voters present.\n" );
    write( "Enter your proposal:\n" );
    this_player()-> edit( TMP_FILE,
            (: "callback_propose" :),
            (: "abort_propose" :),
            title,
            );
    return 1;
}

void abort_propose( string title ) { write( "Aborting proposal.\n" ); }

void callback_propose( string title )
{
    string proposal;

    proposal = read_file( TMP_FILE );
    rm( TMP_FILE );
    if( !proposal )
    {
        write( "Aborting proposal.\n" );
        return;
    }

    policies[title] = ([
            "Title" : title,
            "Proposer" : this_player()-> query_name(),
            "Time" : time(),
            "Text" : proposal,
            "Voted" : ([]),
            "Aye" : ({}),
            "Nay" : ({}),
            "Abs" : ({}),
            ]);
    display_policy( policies[title] );
    write( "You have 1/2 hr to get " + votes_needed( title ) + "\n" );
    call_out( "last_warning", 1500, title );
}

void last_warning( string title )
{
    object proposer;

    if( !policies[title] ) return;
    call_out( "delete_proposal", 300, title );
    if( !proposer = find_player( policies[title]["Proposer"] ) ) return;
    tell_object( proposer,
            "***** LAST WARNING *****\n"+
            "Proposal " + title + " will be deleted in 5 minutes\nunless "+
            "you can find " + votes_needed( title ) + " votes before then.\n" );
}

void delete_proposal( string title )
{
    map_delete( policies, title );
}

int cmd_repeal( string title )
{
    mapping policy;

    if( !title ) return notify_fail( "Syntax: repeal <title>\n" );
    title = lower_case( title );
    if( !policy = POLITICS_D-> query_policy( title ) )
        return notify_fail( "There is no law: " + title + ".\n" );
    if( ( quorum() < QUORUM ) && !archp( this_player() ) )
        return notify_fail( "There are not " + QUORUM + " voters present.\n" );
    policy["Proposer"] = this_player()-> query_name();
    policy["Repeal"] = 1;
    policies[title] = policy;
    call_out( "delete_proposal", 1800, title );
    write( "You have 1/2 hour to get the votes needed to repeal " +
            title + ".\n" );
    return 1;
}

int cmd_cast( string arg )
{
    string vote, title, name;

    if( !arg ||
            ( ( 2 != sscanf( arg, "%s on %s", vote, title ) ) &&
              ( 2 != sscanf( arg, "%s %s", vote, title ) )      ) )
        return notify_fail( "Syntax: cast <vote> on <title>\n" );
    if( !policies[title] )
        return notify_fail( "There is no proposal: " + title + ".\n" );
    name = this_player()-> query_name();
    if( policies[title]["Voted"][name] )
        policies[title][policies[title]["Voted"][name]] -= ({ name });
    vote = lower_case( vote[0..2] );
    switch( vote )
    {
        case "aye":
            policies[title]["Aye"] += ({ name }); break;
        case "nay":
            policies[title]["Nay"] += ({ name }); break;
        case "abs":
            policies[title]["Abs"] += ({ name }); break;
        default:
        return notify_fail( "You vote must be 'aye', 'nay' or 'abs'.\n" );
    }
    policies[title]["Voted"][name] = capitalize( vote );
    if( policies[title]["Repeal"] )
    {
        if( vetos_needed( title ) < 1 )
        {
            say( title + " is no longer law.\n" );
            POLITICS_D-> repeal_policy( title );
            delete_proposal( title );
            return 1;
        }
    }
    else if( votes_needed( title ) < 1 )
    {
        say( "Proposal: " + title + " is writ large in the book of law.\n" );
        POLITICS_D-> set_policy( title, policies[title] );
        delete_proposal( title );
        return 1;
    }
    write( "Ok.\n" );
    return 1;
}

void receive_message( string msg_class, string msg )
{
    /*
       write( "class = " + msg_class + "\n" );
     */
    message( msg_class, msg, all_inventory( this_object() ), ({ this_player() }) );
    TOWN_HALL-> receive_message( msg_class, msg );
}

// Wizard test functions

mixed query_policies() { return policies; }

void alter_proposal( string title, string field, mixed value )
{
    policies[title][field] = value;
}

void make_law( string title )
{
    POLITICS_D-> set_policy( title, policies[title] );
    delete_proposal( title );
}

void repeal_law( string title )
{
    POLITICS_D-> repeal_policy( title );
    delete_proposal( title );
}

/* EOF */
