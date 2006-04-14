#include <lib.h>
#include <daemons.h>
#include "/lib/include/stargate.h"

inherit LIB_STARGATE;

int readScreen();

void create() 
{
    ::create();
    setOrigin("tower", "/domains/Ylsrim/room/tower");
    SetRead(([ ({ "screen" }) : (: readScreen :) ]) );
    SetItems(([ ({ "screen" }) : "a computer screen which shows the status of the gate network" ]) );
}

void init()
{
    ::init();
}

int readScreen()
{
    write("stargate network status\n");
    write("-----------------------\n");
    write("\n");

}
