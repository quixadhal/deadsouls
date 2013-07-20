#include <lib.h>
#include <daemons.h>
#include "/lib/include/stargate.h"

inherit LIB_STARGATE;

int ReadScreen();

void create() 
{
    ::create();
    SetOrigin("cave", "/domains/town/room/cave_entrance");
    SetRead(([ ({ "panel","writing" }) : (: ReadScreen :) ]) );
    SetItems(([ ({ "panel","writing" }) : "a panel you can read" ]) );
}

void init()
{
    ::init();
}

int ReadScreen()
{
    write("-----------------------\n");
    write("dial tower\n");
    write("-----------------------\n");
    write("\n");

}
