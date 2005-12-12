#include <lib.h>

inherit LIB_ROOM;

static void create() {
    object ob;
    room::create();
    SetClimate("indoors");
    SetAmbientLight(30);
    SetShort("Creators' Hall");
    SetLong("A chalkboard sits in the middle of this circular room. Immortals "
      "come here to communicate with each other about the world they are "
      "building. \n"+
      "An imposing steel door leads east to the Combat Arena. "+
      "The Adventurer's Guild is north. The Arch Room is south. "+
      "To visit Frontiers, go west.");
    SetExits( ([ "north" : "/domains/town/room/adv_guild",
	"south" : "/secure/room/arch",
	"west" : "/domains/default/room/telnet_room",
	"east" : "/domains/town/room/arena" ]) );
    SetDoor("east","/domains/town/doors/steel_door");

    SetObviousExits("n, s, e, w");
    SetProperty("no attack", 1);
    SetProperty("nopeer",1);
    ob = new("/lib/bboard");
    ob->SetKeyName("chalkboard");
    ob->SetId( ({ "board", "chalkboard", "dusty board", "dusty chalkboard" }) );
    ob->set_board_id("immortal_board");
    ob->set_max_posts(30);
    ob->SetShort("a dusty chalkboard");
    ob->SetLong("This old chalkboard is worn and abused.  Immortals use it "
      "to scribble messages to one another. To post something, type: post <message>. "
      "When finished writing, enter a single period on a blank line, then at the "
      "colon prompt (:) type a lower-case x and return. \n");
    ob->eventMove(this_object());
    SetInventory(([
	"/domains/default/npc/tree" : 1,
	"/domains/default/obj/chest.c" : 1 ]));
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
