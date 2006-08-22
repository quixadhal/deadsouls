// hall.c
// The basic start room. Players begin here, and this is where they can find
// out what quests they haven't yet done. Also, people will probably get
// into the habit of hanging out here. Lord knows all the TMI wizards do.
// Created by Mobydick, September 1992.

#include <mudlib.h>

inherit ROOM ;
inherit DOORS ;

void create() {
	::create();
	seteuid(getuid()) ;
	set ("short", "Adventurer's Hall") ;
	set ("long",
	"You are in the adventurer's hall in Footown. This is where you\n"+
	"come to find out which quests you have not solved, and it's also\n"+
	"a good place to hang out and meet people. There is an uncommonly\n"+
	"well-done painting hanging on the wall, and a large book titled\n"+
	"\"Quests\" lies on a large table here.\n\n"+
	"East will take you out onto the streets of Footown. To the west\n"+
	"is a strange-looking door which you could walk through. A spiral\n"+
	"staircase leads up to the second floor.\n"+
	"There is a sign on the wall; you feel strangely compelled to look\n"+
	"at it.\n") ;
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
        set("item_func", ([ "painting" : "eyes" ]) );
        set("light", 1);
     	reset();
	set ("exits", ([
	"east" : "/domains/Fooland/smain",
	"up" : "/domains/Fooland/hall2",
	"west" : "/domains/TMI/tower1"
	]) ) ;
	create_door ("west", "east", "A sturdy oak door, scratched on this side", "closed") ;
	set ("objects", ([
		"orc" : "/obj/orc",
		"axe" : "/obj/axe"
	]) ) ;
	reset() ;
}

string eyes() {
	write ("It's a painting of the TMI-2 development tower.\n") ;
	write ("Suddenly you are sucked into the painting! You feel\n"+
	"faint and when you recover you are elsewhere...\n\n") ;
	say (this_player()->query("cap_name")+" is suddenly sucked into the painting and is gone!\n") ;
	this_player()->move_player("/domains/TMI/tower1") ;
	return "Change this to something more meaningful.\n" ;
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
