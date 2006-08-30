
// File : /domains/Fooland/hall.c
//
// The basic start room. Players begin here, and this is where they can find
// out what quests they haven't yet done. Also, people will probably get
// into the habit of hanging out here. Lord knows all the TMI wizards do.
// Created by Mobydick, September 1992.
// Updated by Watcher and Mobydick (04/93) for formal mudlib release.

#include <mudlib.h>

inherit ROOM ;
inherit DOORS ;

void create() {
 
   object *obs ;
   object ob ;

   ::create();

   seteuid(getuid(this_object()));
//      call_other("/domains/Fooland/mainboard","foo");
 
   set("short", "Adventurer's Hall");
   set("long",
     "You are in the Adventurer's Hall of Footown. This is where players\n" +
     "come to find out which quests remain unsolved. There is a large book\n"+
     "entitled \"Quests\" lying on a nearby table which attracts your notice."+
     "\nEast will take you out onto the fable streets of Footown, while a \n"+
     "spiral staircase leads upwards and a door leads west. There is a very\n"+
     "interesting painting of a grey-bearded wizard hanging on the wall.\n"+
     "There is also a large sign on another wall; you feel strangely\n"+
     "compelled to look at it.\n") ;
 
   set("exit_msg", ([
		"up" : "$N climbs the spiral staircase to the upper floor.\n",
		"east" : "$N heads out into the world of adventure.\n"
	]) ) ;
 
   set("item_desc", ([
		"sign" :
	"Footown is the basic town here. There is a shop, a bank, a money\n"+
	"exchange, and a pub here, plus this hall and some local scenery.\n"+
	"On the north road outside town, there's a hill, and from the top\n"+
	"of the hill you can get a pretty good look at the countryside.\n",
		"table" :
	"It is a large, round, oak table. Many generations of adventurers\n"+
	"have pored over it as they tried to figure out the solutions to\n"+
	"their quests.\n",
		"book" :
	"This is the Great Book of Quests. You can \"read\" it.\n"
		]) ) ;
   set ("item_func", ([ "painting" : "eyes" ]) ) ;
 
   set("light", 1);
 
   set("exits", ([
	"east" : "/domains/Fooland/smain",
	"up" : "/domains/TMI/rooms/quad",
	"west" : "/domains/Fooland/hall2",
	]) ) ;
 
   create_door("east", "west", "A sturdy oak door with a scratch on its side",
	       "closed");

// We clone up Googol the dwarf if either no copy exists, or if only
// the master copy (which is in the void) exists.
   obs = children("/obj/googol") ;
   if (sizeof(obs)==0 || ( sizeof(obs)==1 && !environment(obs[0]) ) ) {
      ob = clone_object("/obj/googol") ;
       if(ob) ob -> move(this_object()) ;
   }
 
   set("objects", ([
	"/obj/orc" : 1,
		"axe" : "/obj/axe",
	]) ) ;
   reset();
}

void init() {
	add_action ("read_book", "read") ;
}

int read_book (string str) {
	if (str!="book" && str!="quests" && str!="book of quests") {
		notify_fail ("What do you want to read?\n") ;
		return 0 ;
	}
	write ("You open the Book of Quests and read:\n"+
"   First Quest. The janitor of the Footown bank desperately wants a nice\n"+
"shrubbery for his bank. Go out into the mountain valley, where shrubs can\n"+
"be found, and bring one back for him.\n"+
"<apologies to Monty Python>\n") ;
	say (this_player()->query("cap_name")+" reads the Book of Quests.\n") ;
	return 1 ;
}
 
int eyes() {
	write ("The eyes of the old wizard look back at you! Suddenly, you\n"+
"feel a tremendous force pulling you into the picture..\n") ;
	this_player()->move_player("/domains/Fooland/tower1",
	this_player()->query("cap_name")+" vanishes into the painting!\n") ;
	return 1 ;
}
