// This room lets you access the board archives in /domains/Fooland/data/attic.
// It uses an archive-reading board created by Zak@TMI-2 in March 1993.
// Mobydick created this room on 3-19-93.

#include <mudlib.h>

inherit ROOM ;

void create() {

	object brd ;

	::create() ;
	seteuid(getuid()) ;
	set("light", 1) ;
	set ("short", "The archive board room") ;
	set ("long",
"In this corner of the archive stands a bulletin board with a few extra\n"+
"nifty buttons on it. It lets you access the archives of old posts on the\n"+
"various boards of this MUD.\n") ;
	set ("exits", ([ "south" : "/domains/Fooland/tower1" ]) ) ;
	//brd=clone_object("/std/board/archive");
    //brd->set_attic_dir("/domains/Fooland/data/attic");
 	//brd->set_location("/domains/Fooland/archiverm") ;
}
