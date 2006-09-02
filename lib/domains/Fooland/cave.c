// A cave with a pile of coins in it.
// Mosty for texture. It rewards players who explore, though.

#include <mudlib.h>

inherit ROOM ;

void create() {
	seteuid(getuid()) ;
	::create();
	set ("light", 0) ;
	set ("long",
"You are in the cavern where the mighty dragon Bazz once lived. Here Sir\n"+
"Foo fought and slew him. Most of his treasure trove is gone, but you can\n"+
"still find a few valuable coins tucked away.\n") ;
	set ("short", "Bazz's cave") ;
	set ("objects", ([ "sword" : "/obj/light_sword" ]) ) ;
	reset() ;
	set ("exits", ([
		"north" : "/domains/Fooland/fissure"
	]) ) ;
}

void reset() {

	object ob ;

	::reset() ;
	if (present("coins", this_object())) return ;
	ob = clone_object("/std/coins") ;
        if (random(2)) {
                ob->set_type("gold") ;
                ob->set_number(60) ;
        } else {
                ob->set_type ("platinum") ;
                ob->set_number(10) ;
                ob->set_number(10) ;
        }
        ob->move(this_object()) ;
}
