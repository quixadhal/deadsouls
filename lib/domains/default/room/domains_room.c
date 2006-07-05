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
    SetShort("Domains Room");
    SetLong("This room provides a convenient access point to various domain start rooms and featured realms.");
    SetItems(([
	"Ylsrim" : "This is the entry point for the Ylsrim domain.",
	"campus" : "This is the entry point for the campus domain.",
	"examples" : "This is the entry point for the examples domain.",
	({ "sign" }) : "A sign you can read.",
      ]));
    SetEnters( ([
	"campus" : "/domains/campus/room/start",
	"ylsrim" : "/domains/Ylsrim/room/bazaar",
	"examples" : "/domains/examples/room/start.c",
      ]) );
    SetProperty("no attack", 1);
    SetProperty("nopeer",1);
    ob = new("/lib/bboard");
    ob->SetKeyName("chalkboard");
    ob->SetId( ({ "board", "chalkboard", "dusty board", "dusty chalkboard" }) );
    ob->set_board_id("immortal_board");
    ob->set_max_posts(30);
    SetShort("Domains Room");
    ob->eventMove(this_object());
    SetExits(([
	"north" : "/domains/default/room/wiz_hall2",
      ]));

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
