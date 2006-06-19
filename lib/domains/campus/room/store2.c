#include <lib.h>
inherit LIB_ROOM;
static void create() {
    room::create();
    SetClimate("indoors");
    SetProperties( ([ "light" : 2 ]) );
    SetShort("a generic room");
    SetLong("This is an utterly plain, blank room.");
    SetInventory(([
	"/domains/campus/meals/burger" : 20,
	"/domains/campus/meals/ham_sand" : 20,
	"/domains/campus/meals/milk" : 20,
	"/domains/campus/obj/flashlight" : 20,
	"/domains/campus/meals/salad" : 20,
	"/domains/campus/meals/gator" : 20]));
}
