#include <lib.h>
#include ROOMS_H
#include "./area/customdefs.h"

inherit LIB_ROOM;
int ReadSign();
int ReadSheet();

static void create() {
    room::create();

    SetClimate("indoors");
    SetAmbientLight(40);
    SetShort("A workroom");
    SetLong("This is a builder's workroom. The Builders' Hall is down. "+
            "A sample room is east.");
    SetItems( ([ "workroom" : "It looks very nice.",
                //({"sign","hint","hints"}) : "A sign with hints for creators.",
                //({"sheet","reminder","reminders"}) : "A sheet with quick reminders for creators.",
                ]) );
    SetRead( ([
                //({"sign","hint","hints"}) : (: ReadSign :),
                //({"sheet","reminder","reminders"}) : (: ReadSheet :)
                ]) );
    SetInventory( ([
                MY_OBJ "/builder_chest" : 1,
                "/domains/default/obj/bbucket" :1,
                ]) );
    SetExits( ([ "down" : "/domains/default/room/builder_hall.c",
                "east" : MY_ROOM "/sample_room"
                ]) );
    //SetNoModify(1);
}

int ReadSign(){
    this_player()->eventPage("/news/hints.txt");
    return 1;
}

int ReadSheet(){
    this_player()->eventPage("/news/reminders.txt");
    return 1;
}

void init(){
    ::init();
}
