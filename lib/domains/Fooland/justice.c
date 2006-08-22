// justice.c
// The justice of the peace of Footown. 
// Nominally performs marriages, although the marriage code is in the
// courthouse and not in the monster.

#include <mudlib.h>

inherit MONSTER ;

void create () {
	::create() ;
	set("short", "The Justice of the Peace of Footown") ;
	set("long", "The justice can perform marriages for you.\n") ;
	set ("id", ({ "justice", "judge" }) ) ;
	set ("name", "justice") ;
	set_living_name("justice") ;
	enable_commands() ;
	set ("no_attack", 1) ;
	set("gender", "female") ;
	set ("inquiry", ([
"marriage" : "If you type \"help marriage\", I'll tell you all about it.",
"marriages" : "Typing \"help marriage\" will give you all the details.",
"guard" : "He's a nice man, and very persistant."
	]) ) ;
}
