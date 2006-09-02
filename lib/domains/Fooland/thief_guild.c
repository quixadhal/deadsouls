// thief_guild.c
// The thieves guild in the town of Fooland.
// This inherits the standard guild object. The standard object has the
// interesting comments on how it all works.
// Written by Mobydick@TMI-2, 11-16-92
// Please leave this header intact.

#include <mudlib.h>

inherit "/std/guild" ;
inherit DOORS ;

void create() {
	::create();
	seteuid(getuid()) ;
	set("short", "The Footown Thieves Guild") ;
	set("long", 
"You are in a dark and cramped hut. Around you are the various tools of\n"+
"theft: lockpicks, dark clothing, easily concealed weapons. This is the\n"+
"Footown Thieves Guild, and here you can learn the arts of stealth and\n"+
"treachery. Type \"list\" to get a list of the skills you can train in and\n"+
"the cost, or type \"train\" to train in a particular skill.\n") ;
	set ("exits", ([
		"north" : "/domains/Fooland/cemetery"
	]) ) ;
	set ("light", 1) ;
	set_trainable_skills ( ({ "Theft", "Stealth", "Wilderness" }) ) ;
	set ("objects", ([
		"thief" : "/domains/Fooland/bongo",
		"guard" : "/domains/Fooland/thief"
	]) ) ;
	create_door ("north", "south", "An iron door with a narrow slit", "locked", "silver_key") ;
	reset() ;
}
