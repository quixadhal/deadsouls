// fight_guild.c
// The fighter's guild in the town of Fooland.
// This inherits the standard guild object. The standard object has the
// interesting comments on how it all works.
// Written by Mobydick@TMI-2, 11-16-92
// Please leave this header intact.

inherit "/std/guild" ;

void create() {
	::create();
	seteuid(getuid()) ;
	set("short", "The Footown Fighter's Guild") ;
	set("long", 
"This is the fighter's guild in Footown. This is where players come to\n"+
"train in their combat skills. You can \"list\" the skills you can improve\n"+
"in and the cost of improving that skill, and you can \"train\" in a given\n"+
 "skill.\n") ;
	set ("exits", ([
		"north" : "/domains/Fooland/wbazz"
	]) ) ;
	set ("light", 1) ;
	set_trainable_skills ( ({ "Blunt weapons", "Cutting weapons", "Thrusting weapons", "Shield defense", "Parrying defense", "First aid" }) ) ;
	set ("objects", ([ "hero" : "/obj/hero" ]) ) ;
	reset() ;
}
