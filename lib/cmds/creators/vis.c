//      /bin/dev/_vis.c
//      from the Dead Souls mudlib
//      created by Descartes of Borg october 1992

#include <lib.h>

inherit LIB_DAEMON;

varargs int cmd(string str)
{
   if(!this_player()->GetInvis()) {
	notify_fail("You are already visible.\n");
	return 0;
    }
    this_player()->SetInvis(0);
    message("my_action", "You step out of the shadows.", this_player());
   return 1;
}


int help()
{
  write( @EndText
Syntax: vis
Effect: Makes you visible if you wern't already.
EndText
  );
  return 1;
}
