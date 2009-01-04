#include <lib.h>
inherit LIB_ROOM;

int doorcounter,callfloor,closed,floor,my_counter,moving;
string floorname;
static void create() {
    room::create();
    SetClimate("indoors");
    SetAmbientLight(30);
    SetShort("Church Elevator");
    SetLong("This is the elevator in the village church. The elevator door is on the east wall. Two buttons are set into the wall next to the door, labeled '1' and 'b'.");
    floor=1;
    moving = 0;
    closed=1;
    callfloor=0;
    SetExits( ([
                "east" : "/domains/town/room/church",
                ]) );
    SetItems(([
                //new("/domains/town/obj/ebutton1") : 1,
                //new("/domains/town/obj/ebutton2") : 1,
                "elevator" : "A means of vertical indoors transportation.",
                "wall" : "The buttons are on the wall.",
                ({"elevator door","door"}) : "The door to the outside."
                ]) );
    AddItem(new("/domains/town/obj/ebutton2"));
    AddItem(new("/domains/town/obj/ebutton1"));
    set_heart_beat(1);
}
void init(){
    ::init();
    set_heart_beat(1);
}
int CallMe(int i){
    if(i == floor && moving == 0) {
        this_object()->SetDoorClosed(0);
        return 1;
    }
    else callfloor = i;
    return 1;
}
int SetDoorClosed(int i){
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
#if 1
    if(living(ob) && closed > 0 && query_verb() != "goto" &&
            query_verb() != "trans"  ){
        message("info","The elevator door is closed.", ob);
        return 0;
    }
#endif
    return 1;
}
int CanRelease(object ob){
    if(archp(ob)) {
        tell_object(ob,"%^RED%^As archwizard, you are permitted to "
                "exit the elevator at any time. Normal creators and "
                "players cannot do this.%^RESET%^\n");
    }
    if(closed > 0 && query_verb() == "go" ){
        message("info","The elevator door is closed.", ob);
        return 0;
    }
    return 1;
}
varargs int eventRoll(int i){
    if(!i) i = 10;
    moving = 1;
    SetDoorClosed(1);
    my_counter = i;
    return i;
}
void heart_beat(){
    if(doorcounter > 0){
        doorcounter--;
        if(doorcounter  < 2) SetDoorClosed(1);
    }

    if(moving == 0 && closed == 1 && callfloor > 0){
        tell_room(this_object(),"The elevator lurches into motion.");
        eventRoll();
    }

    if(moving && moving > 0){
        my_counter--;
        if(my_counter % 5  == 0) {
            tell_room(this_object(),"The elevator continues...");
        }

        if(my_counter < 2) {
            my_counter = 0;
            moving = 0;
            SetFloor(callfloor);
            tell_room(this_object(),"The elevator arrives at its destination.");
            SetDoorClosed(0);
            callfloor = 0;
        }
    }
}
