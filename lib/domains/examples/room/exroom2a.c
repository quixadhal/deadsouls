#include <lib.h>
#include "ex.h"

inherit PAPAROOM;

void create()
{
    ::create();
    SetAmbientLight(30);
    SetShort( "a 'child' file" );
    SetLong("This is an example room whose properties are inherited "
      "from another room.\n<look more> for more information."
    );

    //Note:  When we set the "exits" in this room we
    //       over-write the "exits" we inherited from PAPAROOM
    // There's a way to just add an exit to the list, but exits
    // are primarily unique to each room, so this method is fine.
    SetExits( ([
        "west" : EXPATH + "exroom2",
        "east" : EXPATH + "exroom2b"
      ]));

    SetItems( ([ 
        "more" : @EndText
-------------------------------------------------------------------
Take a look at the CanReceive function.
Do you see the line ::CanReceive(ob)?
The :: refers to 'the file I've inherited from'.
So, we are calling (using) the function CanReceive() as it is
written in exroom2.c 
THEN we are doing our own thing.

When an object enters this room (player or bowl or sword etc)
it becomes the variable 'ob'.

We pass that variable 'ob' along to the function CanReceive()
that's in exroom2.c 

Then we check to see if ob is a player or creator. (type: man userp)
If so, we give that object a message.
-------------------------------------------------------------------
EndText,
      ]));

    /*   See the way I did the SetItems again for 'more'.  This is one
         way to do it.  Another way to do it is to use a (: functional :)
         like I did for the lever.  Take a look, however, at this comment
         that you are reading.  If you'll notice, I've done it a little
         differently to show you a way to comment out several lines all
         at once.  The original method that I used was to put // in front
         of the line that I wanted to comment out.  If you have several
         lines like we have here, then you'll want to begin your comment
         like you see here and end it like below.  This is how I will
         be doing large comments in future examples.
     */


}

int CanReceive(object ob)
{
    ::CanReceive();
    if ( userp(ob) ) 
        write("%^MAGENTA%^Room tells you: %^GREEN%^You're any player, creator or not! %^MAGENTA%^(This is from this room)%^RESET%^\n");
    return 1;
}

void init()
{
    ::init();
}
