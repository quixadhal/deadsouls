#include <lib.h>
#include <dirs.h>
#include <rooms.h>

inherit LIB_ROOM;

string FunkyPic();
int CheckChat();
int StartHeart(object ob);

static void create() {
    room::create();
    SetClimate("indoors");
    SetAmbientLight(30);
    SetShort("off the mortal coil");
    SetLong( (:FunkyPic:) );
    SetObviousExits("no exit");
    set_heart_beat(10);
}

void init(){
    ::init();
    add_action("regenerate","regenerate");
    add_action("wander","wander");
    this_object()->CheckChat();
}

string FunkyPic(){
    return read_file("/domains/default/etc/death.txt");
}

int regenerate(){
    write("With a great rush of matter and energy, you rematerialize "+
      "into a corporeal state, and find yourself in a familiar place...");
    this_player()->eventRevive();
    this_player()->eventMoveLiving(ROOM_START);
    return 1;
}

int wander(){
    write("There is a strange, hollow vibration all around you, and you "+
      "realize that some force is compelling your ethereal form elsewhere..."+
      "you find yourself in a place that is known to you, yet oddly new.");
    this_player()->eventMoveLiving("/domains/campus/room/admissns");
    return 1;
}

void heart_beat(){
    tell_room(this_object(), "A voice whispers: \" You may choose to "+
      "regenerate into a new body here.\"");
    return;
}
