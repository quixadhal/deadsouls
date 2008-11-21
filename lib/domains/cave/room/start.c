#include <lib.h>
#include <rooms.h>

inherit "/domains/cave/etc/cave_room";

void create() {
    ::create();
    SetAmbientLight(30);
    SetShort("a blank room");
    SetLong("A featureless area.");
}
void init(){
    ::init();
}
