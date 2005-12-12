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
}

int ReadSign(){
    write( @EndText

	WELCOME TO THE ADVENTURERS' GUILD!

   Things are pretty straightforward here. You can advance
to higher levels by possessing enough experience and having
completed enough quests. 

   Type "status" to see how many xp's you have. Those are
experience points, and with enough of them, you can advance
to the next level. 

   After level 4, you must have enough quest points, as
well. Read the scroll here to see what quests are available.
You only get points the first time you complete a quest.

The advancement requirements break down like this:

Level   Title                       XP                QP
  1     the utter novice            0                 0               
  2     the simple novice           1000              0               
  3     the beginner                1500              0               
  4     the adventurer              2300              0               
  5     the experienced adventurer  3500              5               
  6     the expert adventurer       5100              12              
  7     the great adventurer        7700              21              
  8     the master adventurer       12000             32              
  9     the Freeman                 17000             45              
  10    the Citizen                 26000             60              
  11    the Knight                  39000             77              
  12    the Baron                   59000             96              
  13    the Count                   88000             117             
  14    the Earl                    130000            140             
  15    the Marquis                 198000            165             
  16    the Duke                    297000            192             
  17    the Arch Duke               444444            221             
  18    Praetor                     666667            252             
  19    Quaestor                    1000000           285             
  20    Caesar                      2000000           450
  21+   <See Trans-Human Elder Guildmaster for information>


To advance, ask the guildmaster. 
Example:

ask dirk to advance

EndText
    );
    return 1;
}

int ReadScroll(){
    write( @EndText

The scroll is stained, old, and tattered. It seems that only
a few lines can be read:

* Old Mansion: Legend has it that there are secrets in
the old abandoned mansion south of the church.
Quest Points: 2

* Orcslayer: Find the famed Orcslayer sword and return it to
Leo the archwizard. Leo was last seen going into the church.
Quest Points: 10


EndText
    );
    return 1;
}



