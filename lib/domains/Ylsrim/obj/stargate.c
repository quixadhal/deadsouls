#include <lib.h>
#include <daemons.h>
#include "/lib/include/stargate.h"

inherit LIB_STARGATE;

int ReadScreen();

void create() 
{
    ::create();
    SetOrigin("tower", "/domains/Ylsrim/room/tower");
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
