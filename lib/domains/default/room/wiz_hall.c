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
    SetShort("Creators' Hall");
    SetLong( (: LongDesc :) );
    SetProperty("no attack", 1);
    SetProperty("nopeer",1);
    ob = new("/lib/bboard");
    ob->SetKeyName("chalkboard");
    ob->SetId( ({ "board", "chalkboard", "dusty board", "dusty chalkboard" }) );
    ob->set_board_id("immortal_board");
    ob->set_max_posts(30);
    ob->SetShort("a dusty chalkboard");
    ob->eventMove(this_object());
    SetInventory(([
	"/domains/default/npc/tree" : ({ 10, 1 }),
	"/domains/default/obj/chest" : 1 ]));
    SetItems( ([
	({"sign"}) : "A sign you can read.",
      ]) );
    SetExits( ([
	"east" : "/domains/default/room/wiz_corr1",
	"south" : "/secure/room/arch",
	"north" : "/domains/town/room/adv_guild",
	"west" : "/domains/default/room/wiz_hall2.c",
      ]) );
    SetRead("sign", (: load_object(ROOM_ARCH)->SignRead() :) );
}

int CanReceive(object ob) {
    if(playerp(ob) && !creatorp(ob) && !present("testchar badge",ob) &&
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

int eventReceiveObject(object ob){
    string race = ob->GetRace();
    if(race && race == "orc"){
	ob->eventPrint("Welcome to our inclusive halls, proud orc!");
    }
    return ::eventReceiveObject(ob);
}

void init(){
    ::init();
}
