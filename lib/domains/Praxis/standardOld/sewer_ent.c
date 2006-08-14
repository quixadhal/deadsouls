#include <std.h>

inherit VAULT;

void create() {
    ::create();
    set_short( "The Praxis sewers");
    set_long(
      "You are underneath the town of Praxis inside their "
      "small sewer system.  There is a manhole to the surface above "
      "you.");
    set_smell("default", "The sewers stink of humanoid waste.");
    set_door("manhole","/domains/Praxis/yard","up",0);
    set_exits( 
      (["north" : "/domains/Under/entry",
	"east" : "/domains/Praxis/sewer",
	"up" : "/domains/Praxis/yard"]) );
    set_property("light", 0);
    set_property("indoors", 1);
    set_items(
      ([ "manhole" : "It is a hole that leads to the surface."]) );
}

void reset() {
    ::reset();
    if(!present("sword")) 
	new("/domains/Praxis/obj/weapon/sword")->move(this_object());
}

