#include <lib.h>
#include <rooms.h>

inherit LIB_ROOM;

string LongDesc(){
    string desc = "Immortals come here to communicate with each other about "+
    "the world they are building. The Adventurer's Guild "+
    "is north. The Arch Room is south. To visit the Dead Souls "+
    "test and development mud, go west. The test lab facilities are east.";
    desc += "\nA sign reads: "+load_object(ROOM_ARCH)->SignRead();
    return desc;
}

static void create() {
    object ob;
    room::create();
    SetClimate("indoors");
    SetAmbientLight(30);
    SetShort("Creators' Hall West Wing");
    SetLong("This is the west wing of the Creators' Hall. North is the telnet room where you can connect to the Dead Souls test and development mud. South is the domains room, where you can conveniently visit featured domains or realms.");
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
	"east" : "/domains/default/room/wiz_hall",
	"north" : "/domains/default/room/telnet_room",
	"south" : "/domains/default/room/domains_room.c",
      ]) );
    SetInventory(([
      ]));

    SetRead("sign", (: load_object(ROOM_ARCH)->SignRead() :) );
}

int CanReceive(object ob) {
    if(playerp(ob) && !creatorp(ob) && !present("testchar badge",ob)) {
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
