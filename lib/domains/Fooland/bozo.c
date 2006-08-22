// Bozo the archmage, who lives in Footown.

#include <mudlib.h>

inherit MONSTER ;

void create () {
	::create() ;
	set("short", "Bozo the archmage") ;
	set("long", "You sense that this old man is extremely skilled in magical arts.\n") ;
	set ("id", ({ "bozo", "Bozo", "mage", "archmage" }) ) ;
	set_name("bozo") ;
	set_living_name("bozo") ;
	enable_commands() ;
	set ("no_attack", 1) ;
	set("gender", "male") ;
	set ("race", "elf") ;
	set ("stat/intelligence", 24) ;
	set_skill ("Magic of nasty sorts", 85, "intelligence") ;
	set ("inquiry", ([
"magic" : "Magic is a complex and dangerous art. First, you must learn\n"+
"the read magic spell, which I will teach you. Second, you must find a\n"+
"scroll of a new spell and cast the read spell on the scroll. This will\n"+
"teach you the new spell. You can cast the new spell if you are skilled\n"+
"enough and if you have enough spell points.",
"spells" : "There are many spells you can learn; type \"help spells\" to\n"+
"see them all. I will teach you the read spell but you must learn the\n"+
"rest from scrolls."
	]) ) ;
}
