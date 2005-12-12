#include <lib.h>
inherit LIB_ROOM;
static void create() {
    room::create();
    SetClimate("indoors");
    SetAmbientLight(30);
    SetShort("The Village Church");
    SetLong("Decades ago this tiny, one-room church was the center "
      "of activity in the village. As the village prospered and secularized, "
      "townsfolk attended less and less, until it became "
      "dilapidated and abandoned, as you see here. The place "
      "is overrun with vermin and cobwebs, but there has been "
      "no vandalism. Just dusty neglect. West Village road "
      "is south of here, and what looks like an elevator is "
      "in the west wall.\n%^GREEN%^There is a button next "
      "to the elevator.%^RESET%^");
    SetItems(([
	({"elevator","elevator door","door","wall","west wall"}) : "Set into the "
	"west wall is a thoroughly modern-looking elevator, which seems "
	"out of place in this rustic, colonial church. There is a button "+
	"next to it, presumably to call the elevator car.",
	"road" : "The road is south of here.",
	({"church","place"}) : "An old church.",
	({"cobwebs","webs","web","vermin","dust","dirt"}) : "No vermin can presently "
	"be seen, but from the cobwebs and dirt it's pretty clear "
	"minor pests have made their home here.",
	"vandalism" : "You don't see any of that."
      ]) );
    //AddItem(new("/domains/town/obj/church_wall"));
    AddItem(new("/domains/town/obj/church_button"));
    SetExits( ([
	"south" : "/domains/town/room/road1",
	"west" : "/domains/town/room/elevator",
      ]) );
    SetInventory( ([
	"/domains/town/obj/donation_box" : 1,
      ]) );
    SetObviousExits("s,w");
}

void init(){
    //	add_action("lookit",({"look","exa","examine"}) );
}

void lookit(string str){
    string s1,s2;

    if(sscanf(str,"at%sbutton%swall",s1,s2) == 2){
	write("A button made for pushing.");
	return;	
    }
    if(sscanf(str,"at%sbutton%selevator",s1,s2) == 2){
	write("A button made for pressing.");
	return;
    }
}
//void init(){
//	if(present("wall",this_object()) && !present("button",present("wall",this_object())) ){
//		new("/domains/town/obj/church_button")->eventMove(present("wall",this_object()));
//	}
//}
