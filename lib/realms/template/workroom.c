#include <lib.h>
#include "./customdefs.h"

inherit LIB_ROOM;
int ReadSign();
int ReadSheet();

static void create() {
    string privs;

    privs = query_privs();
    if( privs ) privs = capitalize(privs);
    else privs = "a creator";
    room::create();
    SetClimate("indoors");
    SetAmbientLight(40);
    SetShort(possessive_noun(privs) + " workroom");
    SetLong("You are standing in the workroom of the mighty " + privs + "!  \n"+
      "You may return to the Creators' Hall by going down.\n"+
      "A sample room is east."+
      "\n%^GREEN%^There is a sign here you can read.%^RESET%^"+
      "\n%^YELLOW%^There is a sheet here you can read.%^RESET%^"+
      "");
    SetItems( ([ "workroom" : "It looks very nice.",
	({"sign","hint","hints"}) : "A sign with hints for creators.",
	({"sheet","reminder","reminders"}) : "A sheet with quick reminders for creators.",
      ]) );
    SetRead( ([
	({"sign","hint","hints"}) : (: ReadSign :),
	({"sheet","reminder","reminders"}) : (: ReadSheet :)
      ]) );
    SetInventory( ([
	"/domains/default/obj/chest" : 1,
	"/domains/default/obj/bbucket" :1,
      ]) );
    SetExits( ([ "down" : "/domains/default/room/wiz_hall.c",
	"east" : MY_ROOM "/sample_room"
      ]) );
    SetNoModify(1);
}

int ReadSign(){
    this_player()->eventPage("/news/hints.txt");
    return 1;
}

int ReadSheet(){
    this_player()->eventPage("/news/reminders.txt");
    return 1;
}

void init(){
    ::init();
}
