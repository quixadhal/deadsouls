#include <lib.h>
#include "ex.h"

inherit LIB_ROOM;

int myvar;

void create()
{
    ::create();
    SetAmbientLight(30);
    SetShort( "a master file" );
    SetLong(@EndText
|---------------------------------------------------------------|-------------|
This is an example room for inheriting files.  This 
file is inherited by two others.  It's also a fully
functional room.  There's a lever to pull.
The rooms which inherit me are to the east.
EndText
    );

    //  You'll notice that for SetLong(), I've put @EndText at the top and EndText at the bottom
    //    and have not put in quotes.  When using this style of code, it is important to make sure
    //    you manually enter to the next line to wrap the lines on the player's screen.
    //    This can be a tedious if you do not know how many lines to enter before you need to
    //    continue on the next line.  If you look back to the Entrance to the Hall of Examples,
    //    You will see a handy line I've used to know when to stop a line and continue on
    //    the next.  I've included it in the room description for your convenience.  the
    //    normal screen default is 79 characters.  Whenever you would like to recreate the line
    //    at the top, type 'margins'

    SetExits( ([
        "south" : EXPATH + "exroom1",
        "north" : EXPATH + "exroom3",
        "east"  : EXPATH + "exroom2a"
      ]) );
    SetItems( ([
        "lever" : "A lever for pulling.",
      ]) );

    myvar = 0;
}

//   The CanReceive() function is very helpful to do checks on an interactive object
//     that enters the room.  In this case, we are checking to see if the object that
//     enters is a creator player object.  If it is, then we will give a message to
//     that object with the write() funciton.  I've put some color tags to set the color
//     off from the rest of the text.  For colors available, type 'colors'.

int CanReceive(object ob)
{
    if ( creatorp(ob) )
        write( "%^BOLD%^BLUE%^Room tells you: %^BOLD%^GREEN%^You're a creator! %^BOLD%^BLUE%^(This is from the master file.)%^RESET%^\n");
    return 1;
}

void init()
{
    ::init();
    add_action("aa_pull","pull");
}

//  We're going to use the variable 'myvar' in the middle of a write() funciton.  As
//   you'll notice below, the text you are writing has to be in the " ".  Then we
//   separate the text by putting in: " + myvar + ".  This adds the myvar variable
//   to the text output whenever a player pulls the lever.  Try it and see.


mixed aa_pull(string str)
{
    if (str!="lever")
        return notify_fail("Pull what?\n");
    myvar ++;
    write("You pull the lever and the value of 'myvar' is now " + myvar + "\n");
    say(this_player()->GetName() + " pulls the lever!\n");
    return 1;
}
