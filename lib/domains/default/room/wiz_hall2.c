#include <lib.h>
#include ROOMS_H

inherit LIB_ROOM;
int ds;

static void create() {
    object ob;
    room::create();
    if(mud_name() == "Dead Souls"){
        ds = 1;
    }
    SetClimate("indoors");
    SetAmbientLight(30);
    SetShort("Creators' Hall Upstairs");
    SetLong("This is the upstairs annex of the Creators' Hall. East is the telnet room where you can connect to the Dead Souls test and development mud. South is the domains room, where you can conveniently visit featured domains or realms. The main hall is below.");
    SetProperty("no attack", 1);
    SetProperty("nopeer",1);
    ob = new("/lib/bboard");
    ob->SetKeyName("chalkboard");
    ob->SetId( ({ "board", "chalkboard", "dusty board", "dusty chalkboard" }) );
    ob->set_board_id("immortal_board");
    ob->set_max_posts(30);
    SetShort("Creators' Hall West Wing");
    ob->eventMove(this_object());
    SetItems( ([
                ({"sign"}) : "A sign you can read.",
                ]) );
    SetExits( ([
                "south" : "/domains/default/room/domains_room",
                "down" : "/domains/default/room/wiz_hall",
                "east" : "/domains/default/room/telnet_room.c",
                ]) );
    if(!ds){
    }
    SetInventory(([
                ]));

    SetRead("sign", (: load_object(ROOM_ARCH)->SignRead() :) );
}

int CanReceive(object ob) {
    if(playerp(ob) && !creatorp(ob) &&
            !member_group(ob,"TEST")) {
        message("info","Creator staff only, sorry.", ob);
        return 0;
    }

    if(ob->GetRace() == "rodent"){
        message("info","You are repelled by rodenticide.",ob);
        return 0;
    }
    return 1;
}
void init(){
    ::init();
}
