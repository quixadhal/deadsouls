#include <lib.h>
#include <vendor_types.h>

inherit LIB_ITEM;

int range = 10;
int speed = 2;
string omsg = "A missile flies $D.";
string imsg = "A missile flies in.";
object owner;

void create(){
    ::create();
    SetKeyName("generic missile");
    SetId( ({"missile"}) );
    SetAdjectives( ({"generic","sample","template"}) );
    SetShort("a generic missile");
    SetLong("This is an object that is launched.");
    SetMass(20);
    SetBaseCost("silver",10);
    SetVendorType(VT_TREASURE);
}

void init(){
    ::init();
    add_action("launch","launch");
}

int GetRange(){
    return range;
}

int SetRange(int x){
    if(x > 1) range = x;
    else range = 0;
    return range;
}

int GetSpeed(){
    return speed;
}

int SetSpeed(int x){
    if(x > 1) speed = x;
    else speed = 0;
    return speed;
}

mixed eventNegotiateObstacles(){
    return 1;
}

mixed eventRunOut(){
    return 1;
}

int SetArmed(int x){
    return 1;
}

mixed eventEncounterBlock(){
    if(owner) tell_player(owner, "poop");
    range = 0;
    return 1;
}

string SetCruiseInMessage(string str){
    if(str){
        imsg = str;
    }
    return imsg;
}

string SetCruiseOutMessage(string str){
    if(str){
        omsg = str;
    }
    return omsg;
}

mixed eventCruiseMessages(object this_room, object next_room, string dir){
    string tmp = replace_string(omsg,"$D",dir);
    if(this_room) this_room->eventPrint(tmp);
    if(next_room) next_room->eventPrint(imsg);
    return 1;
}

int eventCruise(string str){
    object this_room, next_room;
    string exit;
    int err, success;
    this_room = room_environment(this_object());
    exit = this_room->GetExit(str);
    if(exit) err = catch(next_room = load_object(exit));
    if(!err && next_room){
        success = this_object()->eventMove(next_room);
    }
    if(success && environment() == next_room){
        eventCruiseMessages(this_room, next_room, str);
        success = eventNegotiateObstacles();
    }
    else {
        eventEncounterBlock();
    }
    return success;
}

void eventDeploy(string str){
    int i, ret;
    if(!owner) owner = find_player("cratylus");
    for(i=speed;i > 0; i--){
        if(range){
            range--;
            ret = eventCruise(str);
            if(!ret) break;
        }
    }
    if(range){
        call_out("eventDeploy", 1, str);
        if(owner) tell_player(owner, "%^GREEN%^I am "+identify(this_object())+
                ", range level: " + range + ",  " + " cruising in " +
                identify(room_environment(this_object())));
    }
    else {
        if(owner) tell_player(owner, "%^CYAN%^I am "+identify(this_object())+
                " and I ran out of range at "+
                identify(room_environment(this_object())));
        eventRunOut();
    }
}

int launch(string str){
    object room = room_environment(this_object());
    string exit;
    if(!this_player() || !creatorp(this_player())) return 0;
    owner = this_player();
    if(room) exit = room->GetExit(str);
    if(exit) eventDeploy(str);
    SetArmed(1);
    return 1;
}
