#include <lib.h>
inherit LIB_ROOM;


int readSign() {
    this_player()->more("/domains/town/txt/warning_sign.txt");
    return 1;
}
static void create() {
    room::create();
    SetClimate("outdoors");
    SetAmbientLight(30);
    SetShort("South Village road");
    SetLong("This is a cobblestone road, leading "
      "north into town and south away from it." 
      "\n%^RED%^There is a sign here you can read.%^RESET%^");
    SetItems( ([
	({"road","cobblestone road"}) : "A "
	"cobblestone road, running north and south.",
	"sign":"A large sign on the road. To read it, 'read sign'.",
      ]) );
    SetRead("sign", (: readSign :) );
    SetExits( ([
	"north" : "/domains/town/room/south_road1",
	"south" : "/domains/Ylsrim/room/kaliid4"
      ]) );

}
void init(){
    ::init();
}
