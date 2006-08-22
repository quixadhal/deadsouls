// Bongo the master thief, who lives in Footown.

#include <mudlib.h>

inherit MONSTER ;

void create () {

	object ob ;

	seteuid(getuid()) ;
	set("short", "Bongo the master thief") ;
	set ("long", "He has an evil glint in his eye. You sense that he is an extremely dangerous\nperson.\n") ;
	set ("id", ({ "bongo", "Bongo", "thief", "master" }) ) ;
	set_name("bongo") ;
	set_living_name("bongo") ;
	enable_commands() ;
	set ("no_attack", 1) ;
	set("gender", "male") ;
	set ("race", "gnome") ;
	set ("stat/dexterity", 24) ;
	set_skill ("Deviant social behavior", 85, "dexterity") ;
	set ("volume", 1000) ;
	set ("capacity", 10000) ;
	ob = clone_object ("/obj/dagger") ;
	ob->move(this_object()) ;
	set ("no_attack", 1) ;
}
