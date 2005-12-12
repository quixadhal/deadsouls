#include <lib.h>
inherit LIB_ROOM;
int doorcounter,callfloor,closed,floor,counter,moving;
string floorname;
static void create() {
    room::create();
    SetClimate("indoors");
    SetAmbientLight(30);
    SetShort("Church Elevator");
    SetLong("This is the elevator in the village church. The "
      "elevator door is on the east wall. Two buttons are "
      "set into the wall next to the door.");
    floor=1;
    moving = 0;
    closed=1;
    callfloor=0;
    SetExits( ([
	"east" : "/domains/town/room/church",
      ]) );
    SetObviousExits("e");
    SetItems(([
	"elevator" : "A means of vertical indoors transportation.",
	"wall" : "The first and second buttons are on the wall.",
	({"elevator door","door"}) : "The door to the outside."

      ]) );
    AddItem(new("/domains/town/obj/ebutton2"));
    AddItem(new("/domains/town/obj/ebutton1"));
    set_heart_beat(1);
}

void init(){
    set_heart_beat(1);
}

int CallMe(int i){
    if(i == floor && moving == 0) {
	this_object()->SetDoor(0);
	return 1;
    }
    else callfloor = i;
    return 1;
}

int SetDoor(int i){
    if(i && i == closed) return i;
    if(!i && closed == 0) return i;
    if(i == 0 && closed == 0) return i;
    if(i) closed = i;
    else if( closed == 1 ) closed = 0;
    else if( closed == 0 ) closed =1;
    if(floor == 1) floorname = "/domains/town/room/church";
    if(floor == 2) floorname = "/domains/town/room/basement";
    if(closed < 1){
	tell_room(this_object(),"The elevator door opens.");
	tell_room(load_object(floorname),"The elevator door opens.");
	doorcounter = 10;
    }
    if(closed > 0) {
	tell_room(this_object(),"The elevator door closes.");
	tell_room(load_object(floorname),"The elevator door closes.");
	doorcounter = 0;
    }
    return closed;
}

int SetFloor(int i){
    if(floor == i) return 0;
    RemoveExit("east");
    floor = i;
    if(i == 1) AddExit("east", "/domains/town/room/church");
    if(i == 2) AddExit("east", "/domains/town/room/basement");
    return 1;
}


int CanReceive(object ob) {
    if(closed > 0){
	message("info","The elevator door is closed.", ob);
	return 0;
    }
    return 1;
}

int CanRelease(object ob){
    if(closed > 0 && !creatorp(ob)){
	message("info","The elevator door is closed.", ob);
	return 0;
    }
    return 1;
}

varargs int eventRoll(int i){
    if(!i) i = 10;
    moving = 1;
    SetDoor(1);
    counter = i;
    return i;
}

void heart_beat(){
    if(doorcounter > 0){
	doorcounter--;
	if(doorcounter  < 2) SetDoor(1);
    }

    if(moving == 0 && closed == 1 && callfloor > 0){
	tell_room(this_object(),"The elevator lurches into motion.");
	eventRoll();
    }

    if(moving && moving > 0){
	counter--;
	if(counter % 5  == 0) {
	    tell_room(this_object(),"The elevator continues...");
	}

	if(counter < 2) {
	    counter = 0;
	    moving = 0;
	    SetFloor(callfloor);
	    tell_room(this_object(),"The elevator arrives at its destination.");
	    //tell_room(this_object(),"A voice says \"Floor "+cardinal(floor)+".\"");
	    SetDoor(0);
	    callfloor = 0;
	}
    }
}

