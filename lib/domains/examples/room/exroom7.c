#include <lib.h>
#include "ex.h"

inherit LIB_ROOM;

int once;
int is_pulled();

void create()
{
    ::create();
    SetAmbientLight(30);
    SetShort( "two verbs for one action" );
    SetLong(@EndText
This room demonstrates how you can use two verbs to do 
one thing. 
There's a chain here.  You can pull it or yank it.

Note:  See how we change the description of the chain?
It's important to remove 'quest hints' once a 
quest has been done.  And then put them back in
when the quest is ready to do.
EndText
    );

    //  Here is another functional to point to the is_pulled()
    //   function.  This is a very useful tool.

    SetItems( ([
        "chain" : (: is_pulled :),
      ]));

    SetExits( ([
        "south" : EXPATH + "exroom6",
      ]));
}

/*  Here you'll see that we have two add_actions.  We have 'pull' and
    'yank'.  If the player uses either verb when pulling the chain
    the aa_chain() function is called.
 */

void init()
{
    ::init();
    add_action("aa_chain","pull");
    add_action("aa_chain","yank");
}

/* This is the first reset() function that we've seen so I'd like
   to take a moment to explain it.  The reset() function allows
   an object to do self-maintenance.  After every reset interval
   (whose exact length is determined on a mud by mud basis, but
   averages around every 2 hours), reset() is called in every
   object that currently exists.  Here, we use the reset()
   function to return the variable 'once' back to 0 to allow the
   chain to be pulled again after reset occurs.
 */

void reset()
{
    ::reset();
    once=0;
}

/* query_verb() tells you what the player typed */
mixed aa_chain (string str)
{
    string averb=query_verb();
    if (str!="chain")
        return notify_fail(capitalize(averb)+ " what?\n");
    if (once)
        return notify_fail("The chain has already been " +
          averb + "ed.\n");

    //   We're returning the averb variable that was defined and set
    //     above in a message to the player and the room.  Then, we
    //     set the 'once' variable to show that the chain has been pulled.
    write("You " + averb + " the chain!\n");
    say(this_player()->GetName() + " "+ averb + "s the chain!\n");
    once=1;    
    return 1;
}

//  This is the functional that was called to show the current state
//    of the chain.  Pulled or not pulled.
string is_pulled()
{
    if( once )
        return "You see a plain chain.\n";
    else
        return "Here's a chain begging to be yanked or pulled.\n";
}
