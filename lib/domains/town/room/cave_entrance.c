#include <lib.h>
#include <rooms.h>

inherit LIB_ROOM;


void create() {
    room::create();
    SetAmbientLight(30);
    SetShort("a blank room");
    SetLong("A featureless area.");
    SetExits( ([
	"south" : "/domains/town/virtual/forest/24,25" ,
	"out" : "/domains/town/virtual/forest/24,25" ,
      ]) );
}
void init(){
    ::init();
}
