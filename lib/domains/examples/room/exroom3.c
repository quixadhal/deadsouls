#include <lib.h>
#include "ex.h"

inherit LIB_ROOM;

void create()
{
    ::create();
    SetAmbientLight(30);
    SetShort( "move the player" );
    SetLong(@EndText
This is an example room for moving a player.  There's nothing
to look at here. But, surprise, there's a lever to pull.  

We are going to get a bit fancier and have something special
happen when a player pulls the lever.
EndText
    );

    SetExits( ([  
        "south" : EXPATH + "exroom2",
        "north" : EXPATH + "exroom4",
      ]));

}

void init()
{
    ::init();
    add_action("aa_pull","pull");
}

int aa_pull(string str)
{
    if (str!="lever")
    {
        write( "Pull what?\n" );
        return 1;
    }   
    write(@EndText
You pull the lever and a big hand grabs you!
It whisks you away to parts unknown.  Ok, they are known.
Look around when you get there, which is basically instantaneous.
EndText);
    say(this_player()->GetName() + " pulls the lever!");
    this_player()->eventMoveLiving(EXPATH + "exroom1", "forcibly when a big hand takes " + objective(this_player()) + " away",
      this_player()->GetName() + " is dropped from the sky by a big hand." 
    );
    return 1;
}

/*  The eventMoveLiving() function transports the player who pulls
    the lever to another room.  It also writes a message in the room
    that the player leaves and in the room they are sent to.
 */
