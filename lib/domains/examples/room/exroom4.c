#include <lib.h>
#include "ex.h"

inherit LIB_ROOM;
int pre_north();
int post_south();
int post_north();

void create()
{
    ::create();
    SetAmbientLight(30);
    SetShort( "pre/post exits and add/remove exits" );
    SetLong(@EndText
There's a door to the north with a doorbell, and a blank
wall to the south.
-------------------------------------------------------------
This is an example room for using pre-exits and post-exits.
It also shows how to add & delete an exit from the room, and
use a temporary variable on a player.

There's a lever to pull and push, and a bell to ring.
Pull the lever out: create an exit to the south.
Push the lever in:  delete an exit to the south.
Ring the bell: allows you to go north.
When you go south, the exit to the south disappears.
-------------------------------------------------------------
EndText
    );

    SetExits( ([  
      ]));

    AddExit( "north", EXPATH + "exroom5", (: pre_north :));    

}

void init()
{
    ::init();
    add_action("aa_pull","pull");
    add_action("aa_push","push");
    add_action("aa_ring","ring");
}


int aa_pull(string str)
{
    if (str!="lever" && str!="lever out")
    {
        write( "Pull what?\n" );
        return 1;
    }
    /*  Now, we can find out if the lever has been already pulled
        in one of two ways. 
        Method one: we test to see if the exit to the south is open.
        Since pulling the lever does this, and the only way to
        create that exit is pulling the lever -- this is a valid method.

        Method two: we use a variable and change it's value when the lever
        is pushed or pulled.  Since we have a method that works without
        using an additional variable, that is the method I'll pick here.
        It will also show you how to query for an exit.

Note:  GetExits() returns a mapping for all the exits in a room
GetExit("dir") returns the value for that specific direction.
if there is NOT an exit in that direction it will return UNDEFINED
and can be detected using the ! operator. (! means 'not') 
conversely, if GetExit("dir") is true, then there IS an exit
to that direction.  Got it?  Good.
     */

    if ( GetExit("south") )
    {
        write("The lever is already pulled out!\n");
        return 1;
    }

    /*  If you don't know by now, this_player() indicates the player doing the actions
        So when I do this_player()->GetName() it gives me the name of the player
        who is doing the actions and allows me to tell the room who it is.
     */

    write( "You pull the lever and an invisible crack appears in the south wall. "+
      "A door slides open and there is a previously undetected exit." );
    say(this_player()->GetName() + " pulls the lever and an exit appears to the south.\n");

    //  AddExit() functions add an exit to the room.  Here, we added an exit and
    //  set up a (: functional :) to do close up the exit when they leave.

    AddExit( "south", EXPATH + "exroom3", (: post_south :) );
    SetObviousExits("north, south");
    return 1;
}


int aa_push(string str)
{
    if (str!="lever" && str!="lever in")
    {
        write( "Push what?\n" );
        return 1;
    }

    /*  Since there's more then one way for the exit to dissappear (see the post-exit
        function for south) and there may be more then one player wandering the halls
        we don't want to assume that 'this' player pushed the lever.  So we wouldn't
        say "You've already pushed the lever."
     */

    if ( ! GetExit("south") )
    {
        write( "The lever is pushed in as far as it will go.\n");
        return 1;
    }
    write( "You push the lever in and an unseen door slides shut in the south wall."+
      "The wall now appears solid and unpenatrable." );
    say(this_player()->GetName() + " pushes the lever and the south closes tight.\n");

    //  Now we remove the exit that was added earlier.

    RemoveExit("south");
    SetObviousExits("n");
    return 1;
}


int aa_ring(string str)
{
    if (str!="bell" && str!="doorbell" && str!="door bell")
    {
        write( "Ring what?\n" );
        return 1;
    }
    //if we only want to let them ring it once, we can check for the
    //temp var here.  This version will let them ring it many times.
    write("DONG!\nYou ring a doorbell! You are now admitted north.\n");
    say("DONG!  A doorbell rings.\n");
    this_player()->SetProperty("rung_bell", 1);
    return 1;
}


/* CODING STYLE 101
   Personally, I always put my post/pre exit functions right after create and 
   before the reset() or init() functions.  This makes them easy to find.
   Since they're MENTIONED in the create() I like to keep 'em close at hand.
   For the same reason I usually follow my init() with any add_actions that
   I've created.  I also preface my functions with an indicator saying what
   their purpose is.  I've chosen aa_ for add_actions an pe_ for pre or post
   exit functions.  OR i use pre_ or post_ when i have both going on.  This
   may not seem like a big deal in a little file, but when the file gets 
   huge, it helps to glance at a function and go: ah ha! this is a post_exit
   function.  or ah ha! this is an add_action or similarly: ah ha! this is
   not any specialized function so I must be calling it for some other reason.
   The order you write them in is a personal decision.  Just keep your
   create() function first on the list.
 */

//A return 0 in a pre-exit will prevent you from going that dir.
//A return 1 will allow you to go that dir.


int pre_north()
{
    if( !this_player()->GetProperty("rung_bell") )
    {
        write("Ring the doorbell first!\n");
        return 0;  
    }
    //  Now that the pre_north() function has done what it needs to do, we 
    //   will call the post_north() function here
    post_north();
    return 1;
}


int post_north()
{
    //might as well delete it, it's served its purpose.
    this_player()->RemoveProperty("rung_bell");
    return 1;
}


//We're going to delete the exit AFTER the player walks through
//  Because doing it in a pre_exit would be tatamount to slamming
//  a door in their face.  You wouldn't want to do that... would you?  :P


int post_south()
{
    write("You hear a noise and realize the wall has mysteriously closed.\n");
    say("You hear a noise and realize the wall has mysteriously closed.\n");
    this_player()->eventMoveLiving("/domains/examples/room/exroom3", "south", this_player()->GetName()+" enters.");
    RemoveExit("south");
    SetObviousExits("n");
}
