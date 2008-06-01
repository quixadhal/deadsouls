#include <lib.h>
#include <daemons.h>
#include "/lib/include/stargate.h"

inherit LIB_STARGATE;

int ReadScreen();

void create() 
{
    ::create();
    SetOrigin("outer space", "/domains/town/virtual/space/1,1,1");
    SetRead(([ ({ "screen" }) : (: ReadScreen :) ]) );
    SetItems(([ ({ "screen" }) : "a computer screen which shows the status of the gate network" ]) );
}

void init()
{
    ::init();
}

int ReadScreen()
{
    write("stargate network status\n");
    write("-----------------------\n");
    write("\n");

}
