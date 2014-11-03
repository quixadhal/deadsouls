#include <lib.h>
inherit LIB_ROOM;

static void create() {
    room::create();
    SetClimate("outdoors");
    SetAmbientLight(30);
    SetNightLight(10);
    SetDayLight(30);
    SetShort("Orc Valley");
    SetLong("You are in a small valley below the sheer cliffs to the west. A band of orcs appears to have made this place their home, and you can see their ramshackle fortress toward the north. A well-traveled path leads east into a forest. A narrow path snakes its way southwest into the woods.");
    SetSkyDomain("town");
    SetExits( ([
                "east" : "/domains/town/room/clearing",
                "north" : "/domains/town/room/orc_fortress",
                "southwest" : "/domains/town/room/narrow_path.c",
                ]) );
    SetInventory(([
                "/domains/town/npc/orc" : 1,
                "/domains/town/npc/orc2" : 1,
                ]) );
}

void init(){
    ::init();
}

int clean_up(int ref_exists){
    return ::clean_up();
}

void reset(){
}
