#include <std.h>

inherit ROOM;

void create() {
    ::create();
    set_properties( ([ "light" : 2, "indoors" : 1, "no attack" : 1, "no steal" : 1 ]) );
    set_exits( 
      (["south" : "/domains/Praxis/monastery"]) );
    set_short("The chapel");
    set_long( 
      sprintf("The %s chapel is solemn and ancient with stained glass "
	"windows and dark oak pews.  Most wedding ceremonies "
	"are performed here.  Type <help marriage> for "
	"information on weddings.", mud_name()) );
    set_items(
      (["pews" : "Ancient oak seats, darkened by eons of "
	"bottoms resting on them.",
	"windows" : "The windows show highlights of "
	"mud history, many of them gorey.",
	"ceremony" : "The ceremony is only visible when you "
	"scroll back."]) );
}

