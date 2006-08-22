// pub.c
// Written by Mobydick 9-3-92.
// This pub does NOT offer drinks for sale. In the default mudlib, alcohol
// does not provide healing, and in fact you can't get alcohol at all. If
// you want to have the usual Lars system of drinking for healing, then you
// should instead use the file drinkingpub.c which you can also find in
// this directory. You'll also need to visit the following files:
// /std/body/attack.c, /cmds/std/_score.c
// and uncomment out the parts of them that are related to drunkenness.

#include <money.h> ;

inherit "/std/room" ;

void create() {
        ::create();
	set ("light", 1) ;
	set ("short", "The Generic Beer Pub") ;
        set ("long",
	"You are in the Generic Beer Pub in Footown. The pub only sells\n"+
	"generic beer which tastes terrible, and the pub's liquor license\n"+
	"hasn't come through yet so right now they're not even selling that.\n");
	set ("exits", ([ "south" : "/domains/Fooland/wbazz" ]) ) ;
	seteuid(getuid()) ;
	set ("no_vision", "The pub is so filled with smoke that you can't see! Looks like Bob is cooking\n"+
	"burgers again.\n") ;
}
