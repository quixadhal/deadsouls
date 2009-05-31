#include <lib.h>
#include ROOMS_H
#include "ex.h"

inherit LIB_ROOM;


void create() {
    room::create();
    SetAmbientLight(30);
    SetShort( "Entrance to Hall Of Rooms" );
    SetLong(
            "|-----------------------------------------------------------------------------|\n\n"+
            "  You are in a large, empty room.  There are few things to examine -- only the\n"
            "floor, ceiling and wall at this time.  An opening to your north leads to the \n"
            "incredible Hall of Examples.  The line at the top of this description was put\n"
            "in as a VERY useful tool for keeping a standard width to all descriptions\n"
            " and messages.\n\n"
            "  The Hall of Examples has many rooms to look through.  Each room is limited\n"
            "to one or two different types of things.  For example, showing how to do a\n"
            "pre-exit function.  Naturally, you will probably combine many of these\n"
            "functions into one single room.  Room files can get long and involved, but\n"
            "when you understand the individual parts, you have no trouble understanding\n"
            "the whole.  Don't forget to %^BOLD%^GREEN%^'more here'%^RESET%^ in each room."
            "  Most of the rooms have explanations written out in comments.\n\n"
            "  As a final aside, most of the functions are simplistic.  These are \n"
            "very basic examples."
           );

    SetExits( ([  
                "north"   : EXPATH + "exroom1",
                "south" : START
                ]));

    //  What follows is a short tutorial on item descriptions.

    // A mapping is a special type of variable -- a list. 
    //   It has a 'key' and a 'value' which are tied together.
    //   When you query a key it returns the value.
    //   Each key must be unique.

    // The SetItems in rooms is a mapping where the key & value are
    //   both strings.

    // This: ([ ]) means what is inside is a mapping.

    // SetItems( ([ ])  ); means you're putting an entire
    //   mapping into the SetItems.  This is how we put items into
    //   various rooms.  We're manipulating the entire mapping
    //   as a whole.  You'll notice for the walls, there is an
    //   array to allow a player to do 'look wall' and 'look walls'
    //   and get the same description.  This is a very useful method.

    SetItems( ([
                "floor" : "The floor is dirty.",
                "ceiling" : "The ceiling is high",
                ({ "wall", "walls" }) : "The walls are currtly not very exciting.",
                ]) );

    // The room code also allows us to manipulate individual 'keys' of the
    //   SetItems mapping:
    //   AddItem( "bird" , "A small robin sits  passageway you can enter.");
    //   would be a way to put the description for bird into the SetItems mapping.

    // With any mapping, when you set a value for a key that already exists 
    //   the mapping replaces the old one with the new; if that key does 
    //   not yet exist it will add it in.

    // So, when you see the ([ ]) it means you are plunking in a group of
    //   key/value pairs into the mapping.

    // Finally, if you do SetItems( ([ ]) );  it is going to replace
    //   the entire SetItems mapping with what is in the ([ ]).
    //   If you do AddItem(); it will ADD the new mapping to  
    //   the existing SetItems mapping.  

    // To illustrate you need to call the test1() function in this room.
    //   To do that, do the following:
    //   > call here->test1()

}

void test1()
{
    write("Now take a look at the room");
    SetItems( ([
                "north wall" : "The north wall has an exit.",
                "east wall" : "The east wall is painted blue.",
                "south wall" : "The south wall is painted yellow.",
                "west wall" : "The west wall is painted green.",
                "wall" : "Which wall?  North, South, East or West?",
                "walls" : "Which wall?  North, South, East or West?"
                ]) );

    // In this example, when overwriting the SetItems mapping, you are
    //   not able to use arrays at this time.  That is the reason that
    //   'wall' and 'walls' is not listed as you first saw.  Keep this
    //   in mind when overwriting existing mappings.  As you'll also
    //   notice, as mentioned above, you can no longer look at the floor
    //   or the ceiling since the whole SetItems mapping has been replaced.

    // Now let's add one description to the list.
    AddItem( "statue", "There is a statue of a wizard here." );

    return;
}
