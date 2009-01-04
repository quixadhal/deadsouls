#include <lib.h>
#include "ex.h"

//LOOKIE! A constant (#define) at the top of the file instead
// of in a header (.h) file.  Yes, this is possible and sometimes
// a better method.  This is the only room in the entire directory
// which uses this define.  It doesn't really need to go into 
// the ex.h file.  However, it must be at the top of the file.

#define ORC "/domains/town/npc/orc"

inherit LIB_ROOM;

void create()
{
    ::create();
    SetAmbientLight(30);
    SetShort( "temp vars" );
    SetLong(@EndText
You can't really see north and there's a bell
on the south door.  Don't ring it too many times!
-------------------------------------------------------------
This is an example room for querying the specific value of a
stored variable.  Remember though, variables are persistent.
If you want to prevent a future bug report from your players
or creators, then make sure you remove the variable when they
leave.  Otherwise, when the come back in a week and ring the
bell only once, it would actually be added to their previous
rings.
-------------------------------------------------------------
EndText
    );

    SetProperty( "no peer", 1);

    SetExits( ([  
        "north" : EXPATH + "exroom6",
        "south" : EXPATH + "exroom4",
      ]));
}

void init()
{
    ::init();
    add_action("aa_ring","ring");
}

/*  Since we don't need the temp var now, we will remove it.
    In the CanRelease() function, I am telling the room that
    when an object tries to leave, to remove the temp var
    and return 1; to say that it is okay to leave.  Were
    I to put return 0; then the player would not be able
    to leave.
 */

int CanRelease()
{
    this_player()->RemoveProperty("rung_bell");
    return 1;
}

mixed aa_ring(string str)
{
    int numrings;
    if (str!="bell" && str!="doorbell" && str!="door bell")
        return notify_fail("Ring what?\n");
    /* if "rung_bell" has not been set in the player, a query() for it
       will return UNDEFINED.  you CAN add 1 to an undefined to get 1.

       they just rang it one more time, so we'll take what was there and
       add one.  Then we'll put the new value back into the temp var
     **see note at bottom of file.
     */
    numrings=this_player()->GetProperty("rung_bell")+1;  
    this_player()->SetProperty("rung_bell",numrings);
    write("DONG!\nYou ring a doorbell! Don't do it too many times!\n");
    say("DONG!  A doorbell rings.\n");

    if ( numrings>5  )
    {
        if (!present("orc",this_object())) //nested if. see NOTE 2.
        {
            write("Now you've done it!  An orc walks in a bonks you on the head.\n");
            say("Now " + this_player()->GetName() + " has done it!  An orc walks in a bonks " +
              objective(this_player()) +  " on the head.\n");
            new(ORC)->eventMove(this_object());  //this_object IS -this- room!
        }
    }
    return 1;
}

/* NOTE!
   There are many ways to do the same thing with code.  Some ways
   are more readable.  Some are less.  Some are more efficient.
   Some ways are very readable and only slightly less efficient.
   I urge you to start out writing code that is readable AND correct,
   rather then try to squeeze things down to one line of code.  The
   goal is to learn -- to understand what you're doing.  If you
   attempt to copy code that is all squeezed down into one line, and
   you really don't understand it, then you'll be forever just copying
   code and not really "getting it".

   That said.
   in the add_action i did the following:

   int numrings;
   numrings=this_player()->GetProperty("rung_bell")+1;  
   this_player()->SetProperty("rung_bell",numrings);
   if ( numrings>5 ) 
   etc...

   I COULD have eliminated the variable numrings :

   this_player()->SetProperty("rung_bell",
   this_player()->GetProperty("rung_bell") + 1);
   if (this_player()->GetProperty("rung_bell") > 5)
   etc...

   But, read both sets of code and determine for yourself which is easier
   to read; easier to figure out on reading.  You see, other people will
   be reading your code.  Other people will be debugging your code at 
   a later date.  You may decide to never log on again and your project
   is in a domain.  The people in that domain will be fixing bugs & typos
   & runtimes and you gotta bet that they will appreciate READABLE code...
   as well as good comments.  Never be afraid of comments!


   NOTE 2! The nested if.  Could also have been written:
   if ( numrings>5 && ! present("orc",this_object()))
   {
   write("Now you've done it!  An orc walks in a bonks you on the head.\n");
   say("Now " + this_player->GetName() + " has done it!  An orc walks in a bonks " +
   objective(this_player()) " +  " on the head.\n");
   new(ORC)->eventMove(this_object());  //this_object IS -this- room!
   }
   But, if you are having problems with the booleans && || and the code
   is pretty clean & simple already, you might want to go the route
   of nesting your if statements until you get the hang of && and ||.
 */
