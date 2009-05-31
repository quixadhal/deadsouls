#include <lib.h>
#include ROOMS_H

inherit "/domains/cave/etc/cave_room";

void create() {
    ::create();
    SetAmbientLight(5);
    SetNightLight(15);
    SetDayLight(25);
    SetShort("A Dark Cave");
    SetNightLong("This is a cave carved into the rocky riverbank wall. The cave opens west into the night. To the east is darkness.");
    SetDayLong("This is a cave carved into the rocky riverbank wall. Daylight shines in from the west, where the river flows. To the east, the cave continues into darkness.");
    SetLong("This is a small cave near a riverbank.");
    SetItems(([
                ({ "wall", "rocky wall", "river bank wall", "riverbank wall", "rocky river bank wall", "rocky riverbank wall" }) : "The local river carved a narrow and steep valley here over the years. The walls of that valley is what this cave has been dug out of.",
                ]));
    SetExits( ([
                "west" : "/domains/town/room/riverbank",
                "east" : "/domains/cave/room/cavepass1.c",
                ]) );
    SetInventory(([
                ]));
}
void init(){
    ::init();
}
