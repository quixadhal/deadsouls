/*
// The snoop command.
// Daemonized by Buddha (2-19-92)
// Part of the TMI mudlib.
*/

#include <lib.h>

inherit LIB_DAEMON;

int cmd(string str) {
  object ob;
 
    if( !str || str == "" ) {
    if(snoop(this_player()))
	write("Ok.\n"); 
    else write("Couldn't stop snoop\n");
  }
  else if(!(ob=find_player(str=lower_case(str))))
    write(str+": no such player.\n");
  else
    write(snoop(this_player(), ob)?"Now snooping.\n":str+": snoop failed.\n");
 
  return 1;
}

int help()
{
  write( @EndText
Syntax: snoop <user>
Effect: Echos to your screen everything <user> sees or types.
        To stop snooping, type just "snoop"
See also: snoopable
EndText
  );
  return 1;
}
