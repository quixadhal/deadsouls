#include <lib.h>
#include ROOMS_H

inherit LIB_ROOM;

int initialized = 0;

void create() {
    room::create();
    SetAmbientLight(5);
    SetShort("A Dark Cave");
    SetLong("This is a small cave at the base of a cliff. A stone door is set into the north wall. There is some writing scrawled on the wall.");
    SetExits( ([
        "out" : "/domains/town/virtual/forest/-4,25",
        "south" : "/domains/town/virtual/forest/-4,25",
        "north" : "/domains/town/room/cave1.c",
      ]) );
    SetItems(([
        ({ "writing", "writing on the wall","scrawl" }) : "A rough scrawl you can read", 
        ({ "wall" }) : "A rocky surface in this cave.",
    ]) );
    SetRead(({ "writing", "writing on the wall" })  
        , "Friend, dial campus lab" );
    SetDoor("north", "/domains/town/doors/stone.c");
}

void init(){
    ::init();
    if(!initialized){
        object door = present("stone door",this_object());
        if(!door) return;
        door->SetItems( ([
            ({"inscription"}) : "Words in the language of the elves.",
          ]) );
        SetInventory(([
            "/domains/town/obj/stargate3" : 1,
          ]));
        SetClimate("indoors");
        door->SetRead( ([
            ({"inscription"}) : "Speak, friend, and enter.",
            "default" : "Try: read inscription on door"
          ]) );
        door->SetLanguage("Edhellen");
        initialized = 1;
    }
}

varargs mixed eventHearTalk(object who, object target, int cls, string verb,
  string msg, string lang) {
    int decoded = 0;
    object door = present("stone door",this_object());
    if(door) door = door->GetDoor();
    if(door && door->GetClosed() && lower_case(lang) == "edhellen"
      && who->GetLanguageLevel(lang) > 50 && 
      grepp(lower_case(msg),"friend"))
        decoded = 1;
    room::eventHearTalk(who,target,cls,verb,msg,lang);
    if(decoded){
        eventPrint("With a great roar, the stone door rumbles open!");
        door->SetClosed(0);
    }
    return 1;
}
