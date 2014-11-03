/*
// File     :  /domains/Praxis/court_room.c
// Comment  :  The Court where High Justice is served.
// 94-07-02 :  Pallando @ Nightmare wrote it
 */
#include <lib.h>
#include ROOMS_H
#include <council.h>

#define COUNCIL_CHAMBER "/domains/Praxis/council"

inherit LIB_ROOM;

object defendant;

void create()
{
    ::create();
    SetProperties( ([
                "indoors" : 1,
                "light" : 2,
                "no castle" : 1,
                "no teleport" : 1,
                "no bump" : 1,
                "no attack" : 1,
                "no steal" : 1,
                "no magic" : 1,
                ]) );
    SetShort( "the court of high justice" );
    // Thank you, whoever added this.  However, if you have time, I have
    // a few suggestions:
    //   We want the justice system to be intimidating.  Imagine being a
    //   short dwarf.  You have been lead up from a dank stone dungeon.
    //   you peer up at the dark oak furnishings (the dock has omnious
    //   scratches left on by the last victim dragged away for execution)
    //   the flickering torches leave frightning looming dark heights to
    //   the room, while leaving the dock harshly lit.  The judges bench
    //   it tall, imposing, richly ornamented in legal symbols (an iron
    //   glove enclosing an iron fist).  UNderneath it is a supply of gin.
    //   The place smells slightly of chalk dust used for lawyer's wigs,
    //   and the perspiration of bored and jeering juiries.  The stone
    //   fllor flagstones boom ominously every time the usher bangs his
    //   staff to summon the next witness.  A hanging on the wall, 
    //   embroidered by the housewifes union depicts the possible sentances
    //   in gory detail.  Occasional screams can be heard from the prison
    //   or execution square.
    //
    //   You could also use interactive SetItems to show if the dock and
    //   bench are currently occupied.
    SetLong(
            "The sheer size of this room dazzles you.  The ceiling cannot be viewed\n"+
            "from where you stand.  At one end of the room is a huge chair, in front\n"+
            "of which is a desk.  Both the chair and the desk are made from the finest\n"+
            "wood in the land.  Next to the desk lies a second chair, where witnesses\n"+
            "sit.  On the other side of the room are rows of chairs, and an aisle\n"+
            "through the center.  Two doors lead the way out of this room, north\n"+
            "is the Town Hall, east is the sheriff's office.\n"
           );
    SetExits( ([
                "north" : "/domains/Praxis/town_hall",
                "east"  : "/domains/Praxis/sheriff",
                ]) );
    SetItems( ([
                "view" : "A magnificent panorama of all Praxis.",
                "gavel" : "A wooden hammer-like object, used for pounding.",
                "chair" : "Made out of the finest wood.",
                "desk" : "A large desk made out of the finest wood.",
                ({ "door", "doors" }) :
                "Large wooden doors leading out of the room.",
                "ceiling" : "The ceiling extends out of view.",
                "room" : "A very large, spacious room with a "
                "rather formal aura.",
                "office" : "The office of the Sheriff of Praxis.",
                "wood" : "The highest quality wood, made exclusivly "
                "from the trees of the elven forest.",
                ]) );
}

varargs int justicep( mixed arg )
{
    if( !arg ) arg = this_player();
    if( objectp( arg ) ) arg = geteuid( arg );
    return -1 != member_array( arg, JUSTICES) || member_array( arg, POLICECOM );
}

int release_objects( object ob )
{
    return ob != defendant;
}

void init()
{
    add_action( "cmd_law", "law" );
    ::init();
    if( !creatorp( this_player() ) && !justicep( this_player() ) )
        return;
    add_action( "cmd_summon", "summon" );
    add_action( "cmd_verdict", "verdict" );
    add_action( "cmd_sentence", "sentence" );
}

int cmd_law( string title )
{
    return COUNCIL_CHAMBER-> cmd_law( title );
}


int cmd_summon( string arg )
{
    object victim;

    if( !arg )
        return notify_fail( "Syntax: summon <player name>\n" );
    //call_other( ROOM_SHERIFF, "sighs_for_deleted_simul_efun_f.o.o.l." );
    if( victim = present( arg, this_object() ) )
    {
        if( victim == defendant )
            return notify_fail( arg + " is already in the dock.\n" );
    } 
    //else if( victim = present( arg, find_object( ROOM_SHERIFF ) ) ) {
    //	ROOM_SHERIFF-> remove_prisoner( victim );
    //	victim-> eventMove( this_object() );
    //   } 
    else notify_fail( "The sheriffs do not have " + arg + ".\n" );
    if( defendant )
        tell_object( defendant, "You trial is suspended.  You may leave.\n" );
    defendant = victim;
    say( defendant-> query_cap_name() + " is summoned to the dock.\n" );
    return 1;
}

int cmd_verdict( string arg )
{
    if( !arg )
        return notify_fail( "Syntax: verdict guilty|innocent|none\n" );
    if( !defendant )
        return notify_fail( "You must summon a defendant first.\n" );
    switch( arg )
    {
        case "innocent":
        case "free":
        case "insufficient evidence":
            say( "VERDICT: the defendant is found to be Not Guilty.\n" );
            defendant-> set_outlaw( 0 );
            defendant = 0;
            break;
        case "guilty":
            say( "VERDICT: the defendant is Guilty.\n" );
            defendant-> set_outlaw( 1 );
            break;
        case "none":
        default:
            say( "VERDICT: no change in the defendant's status\n" );
            defendant = 0;
            break;
    }
    return 1;
}

int cmd_sentence( string arg )
{
    mapping executions;
    object criminal;

    //executions = ROOM_SHERIFF-> valid_execution();
    executions = ([]);
    if( !arg || undefinedp( executions[arg] ) )
        return notify_fail(
                "Syntax: sentence <punishment>\nCurrent options:\n  "+
                implode( keys( executions ), "\n  " ) + "\n" );
    if( !defendant )
        return notify_fail( "You must summon a defendant first.\n" );
    say( sprintf( 
                "%s says: %s, you have been found guilty by due process\n"
                "  and now justice must be seen to be done.  It is the will of law that\n"+
                "  you shall now be taken from here unto the place of execution,\n"+
                "  where you shall suffer the torments of %s until dead.\n",
                this_player()-> query_cap_name(),
                defendant-> query_cap_name(),
                arg ) );

    criminal = defendant;
    defendant = 0;
    //ROOM_SHERIFF-> add_prisoner( criminal );
    //ROOM_SHERIFF-> execute( criminal, arg );

    return 1;
}


/* EOF */
