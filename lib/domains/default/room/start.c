#include <lib.h>
#include ROOMS_H

inherit LIB_ROOM;

int tutorial = 0;

string DoDesc(){
    string desc = "Welcome! To enter the main area, type: %^%^GREEN%^%^enter town%^%^RESET%^%^\n\n";
    string desc2 = "To enter a tutorial area, type: %^%^RED%^%^enter tutorial\n\n%^%^RESET%^%^";
    return desc + ( tutorial ? desc2 : "");
}

void create() {
    room::create();
    SetClimate("indoors");
    SetAmbientLight(30);
    SetShort("The start room");
    SetLong( (: DoDesc :));
    SetExits( ([ 
                "south" : "/domains/default/room/wiz_hall",
                ]) );
    SetNoModify(1);
    SetCoordinates("5000,5000,0");
    SetItems( ([
      "tutorial" : "A set of rooms to get familiar with this environment.",
      "town" : "The main town.",
   ]) );
    SetEnters( ([
      //"tutorial" : "/domains/tutorial/room/start",
      "town" : "/domains/town/room/start",
   ]) );
   if(tutorial){
       AddEnter("tutorial", "/domains/tutorial/room/start");
   }
}

void init(){
    ::init();
}
