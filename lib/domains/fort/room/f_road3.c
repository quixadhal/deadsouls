#include <lib.h>
#include <rooms.h>

#include "../customdefs.h"

inherit LIB_ROOM;

void create() {
    room::create();
    SetDayLight(30);
    SetNightLight(15);
    SetShort("Fort Road");
    SetDayLong("This cobblestone road wends its way south toward a town, and north to the main gate of a fortress high up on a plateau. The road descends south toward the town, where it is at ground level.");
    SetItems(([
	({ "road", "cobblestone road", "fort road", "fortress road" }) : "This cobblestone road has been worn smooth by many years of frequent use. Deep ruts are worn where thousands of wagons have traveled. It rises north toward the fortress, and south toward a town.",
	({ "fort", "fortress", "plateau", "fortress on the frontier", "fortress on the frontiers" }) : "The road rises steeply to the north, reaching a high plateau where the Fortress on the Frontiers stands. ",
	({ "gate", "fortress gate", "glittering night lights of the fortress gate", "lights of the fortress gate", "night lights of the fortress gate" }) : "The road turns here, and northwest is the main entrance to the fortress.",
	"town" : "You can't quite see it well from here, but it appears that Fort Road intersects an east-west path south of here, which leads toward a small town in the distance.",
	({ "battlement", "battlements", "battlements on the massive walls" }) : "Up high, atop the massive walls of this fortress are crenellated battlements which permit defenders to rain hell on attackers while remaining behind cover.",
	({ "stones", "cobblestones", "ruts", "deep ruts" }) : "Closer inspection of the road confirms that it has indeed been used a great deal.",
      ]));
    SetExits( ([
	"north" : "/domains/fort/room/f_road2",
	"south" : "/domains/fort/room/f_road4.c",
      ]) );
    SetNightLong("This cobblestone road wends its way south into darkness, and north to the glittering night lights of a fortress on a high plateau. The ground at the sides of the road slopes down sharply, making it impossible to go off the road.");

    SetClimate("temperate");

    SetTown("The Fortress");
}
void init(){
    ::init();
}
