#include <lib.h>

inherit LIB_ELEVATOR;

void create(){
    elevator::create();
    SetSpeed(5);
    SetOpacity(0);
    SetFloors( ([
                -1 : ({ "b", "/domains/campus/room/shaftb" }),
                1 : ({ "1", "/domains/campus/room/shaft1" }),
                2 : ({ "2", "/domains/campus/room/shaft2" }),
                ]) );
}

void init(){
    ::init();
}
