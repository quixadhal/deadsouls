#include <lib.h>
inherit LIB_ROOM;

object ob;

int ReadSign();
int ReadScroll();
static void create() {
    room::create();
    SetClimate("indoors");
    SetAmbientLight(30);
    SetShort("The Adventurers' Guild");
    SetLong("This small building is where adventurers can "
      "record their adventures and leave messages and announcements "
      "for other brave souls. This is also the place one can "
      "seek advancement, so if you deserve to be a higher level "
      "adventurer, asking the guild master for it will make it so. "
      "A scroll lists quests one may perform, and a sign contains "
      "some instructions. The Creator's Hall is south of here. "
      "A special skills training area is west.");
    SetItems( ([
	({ "list","scroll"}) : "A scroll hangs from the wall, listing "
	"some adventures you can perform which may have rewards "
	"for you.",
	({"sign","instructions"}) : "This sign contains some "
	"general instructions on how to gain experience and "
	"attain advancement.",
	({"building","small building","here"}) : "You are in the "
	"Adventurers' Guild.",
      ]) );
    SetInventory(([
	"/domains/town/obj/bin" : 1,
	"/domains/town/obj/table" : 1,
      ]));
    SetRead( ([
	({"list","scroll"}) : (: ReadScroll :),
	({"instructions","sign"}) : (: ReadSign :)
      ]) );

    SetExits( ([
	"south" : "/domains/default/room/wiz_hall",
	"north" : "/domains/town/room/vill_road2",
	"west" : "/domains/town/room/training",
	"out" : "/domains/town/room/vill_road2"
      ]) );
    AddStuff( ({ "/domains/town/npc/dirk"
      }) );
    SetObviousExits("n, s, w");
    SetProperty("no attack", 1);
    ob = new("/lib/bboard");
    ob->SetKeyName("chalkboard");
    ob->SetId( ({ "board", "chalkboard", "dusty board", "dusty chalkboard" }) );    ob->set_board_id("adv_guild_board");
    ob->set_max_posts(30);
    ob->SetShort("a dusty chalkboard");
    ob->SetLong("This old chalkboard is worn and abused.  Adventurers use it "
      "to scribble messages to one another. To post something, type: post <message>. "
      "When finished writing, enter a single period on a blank line, then at the "
      "colon prompt (:) type a lower-case x and return. \n");
    ob->eventMove(this_object());
    SetNoClean(1);
}
mixed ReadSign(){
    return (mixed)this_player()->eventPage("/domains/town/txt/advancement.txt");
}

mixed ReadScroll(){
    return (mixed)this_player()->eventPage("/domains/town/txt/quests.txt");
}
void init(){
    ::init();
}
