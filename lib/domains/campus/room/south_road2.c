#include <terrain_types.h>
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
    SetObviousExits("north, south");
    SetShort("South Saquivor Road");
    SetLong("This is a cobblestone road, leading north into town and south away from it. South of here appears to be some sort of University campus.");
    SetItems( ([
        ({"road","cobblestone road"}) : "A "
        "cobblestone road, running north and south.",
        "sign":"A large sign on the road. To read it, 'read sign'.",
        ({"grate","drainage grate","sewer","sewer grate"}) : "A grate that "+
        "seems to lead into sewers. It can't be opened from here.",
      ]) );
    SetSkyDomain("town");
    AddTerrainType(T_ROAD);
    SetExits( ([
        "south" : "/domains/campus/room/npath2.c",
      ]) );
    if(file_exists("/domains/campus/doors/grate.c")){
        AddExit("down", "/domains/campus/room/sewer3");
        SetDoor("down","/domains/campus/doors/grate");
    }
    if(strsrch(mud_name(), "Dead Souls")){
        AddExit("north", "/domains/town/room/south_road1");
    }
    SetInventory(([
      ]));
    SetRead("sign", (: readSign :) );
    AddItem(new("/domains/town/obj/lamp"));
}
void init(){
    ::init();
}
