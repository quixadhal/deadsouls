// janitor.c
// This cute little fellow is part of the shrubbery quest.
// If a player carrying the shrubbery enters the room, he takes the shrub
// and gives them credit for the quest.

#include <mudlib.h>

inherit MONSTER ;

void create() {
	::create() ;
	set("short", "the bank janitor") ;
	set ("long", "This man would love to have a nice shrubbery for the bank lobby.\n") ;
	set ("id", ({ "janitor" }) ) ;
// You cannot attack him, else people could not do the quest with him dead.
	set ("no_attack", 1) ;
	set_name ("Dave the janitor") ;
	set_living_name ("janitor") ;
	enable_commands() ;
	set ("gender", "male") ;
	set ("capacity", 3000) ;
	set ("volume", 500) ;
	set ("max_cap", 3000) ;
	set ("max_vol", 500) ;
	set ("mass", 5000) ;
	set ("bulk", 750) ;
	set ("inquiry", ([ "shrubbery" : "I hear that they grow very nice shrubberies along the creek\nnorth of town." ]) ) ;
}

void init() {

	object ob ;

	if ((int)this_player()->query("quest/shrub")==1) return ;
	ob = present("shrubbery",this_player()) ;
	if (!ob) return ;
	if ((string)ob->query("quest_recognize")!="shrub") return ;
	write ("The janitor thanks you effusively for the shrub.\n") ;
	ob->remove() ;
	write ("Congratulations! You have solved the shrubbery quest!\n") ;
	this_player()->set("quest/shrub",1) ;
}
