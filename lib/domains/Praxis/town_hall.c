/*
// File     :  /domains/Praxis/town_hall.c
// Comment  :  Entrance for Court room and Council chambers
// 94-07-02 :  Pallando @ Nightmare wrote it
 */
#include <lib.h>
#include <objects.h>

#define COUNCIL_CHAMBER "/domains/Praxis/council"

inherit LIB_ROOM;

object *listeners = ({ });


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
    SetShort( "Praxis Town Hall" );
    SetLong(
            "This is a large stately hall; the walls are covered with rich hangings, "
            "the vaulted roof is supported by thick marble columns.  A wide curving "
            "staircase leads up to the council chambers above.  The exit to the "
            "street is north."
            // "  South lies the Court of Justice."
           );
    SetItems( ([
                "hall" : "It is five times the height of a tall man.",
                "roof" : "If you \"listen to council\", perhaps you will hear them.",
                "hangings" : "They depict the noble self-sacrifice of being a councilor.",
                "columns" : "They have gargoyles at their tops.",
                "staircase" : "It goes between two of the columns.",
                "chambers" : "If you listen, perhaps you can hear the council.",
                "court" : "This is where trials are held.",
                "street" : "It is Boc La Road south of the monastary.",
                "gargoyles" : "They look innocently inanimate.",
                "gargoyle" : "It is impenetrateable as a stone wall.",
                ]) );
    SetExits( ([
                "south" : "/domains/Praxis/court_room",
                "north" : "/domains/Praxis/e_boc_la1",
                ]) );
    SetListen( "council", (: "listen_council" :) );
}
int pre_exit_up()
{
    if( leaderp( this_player() ) || creatorp( this_player() ) )
        return 1;
    write( "A gargoyle blocks your way.\n" );
    return 0;
}
string listen_council( string arg )
{
    // First, check if there are any councilors to listen to.
    //call_other( COUNCIL_CHAMBER, "???" );
    //if( !sizeof( filter_array( all_inventory( find_object( COUNCIL_CHAMBER
    // ) ), "leaderp", OB_SIMUL_EFUN ) ) )
    //	return "Council is not in session.\n";
    //   if( -1 != member_array( this_player(), listeners ) )
    //	return "You are already listening.\n";
    //   listeners += ({ this_player() });
    return "You start listening in to the council's debate.\n";
}
int release_objects( object ob )
{
    if( -1 != member_array( ob, listeners ) )
    {
        tell_object( ob, "You stop listening to the council.\n" );
        listeners -= ({ ob });
    }
    return 1;
}
void receive_message( string msg_class, string msg )
{
    message( msg_class, msg, listeners );
}

/* EOF */
void init(){
    ::init();
}
