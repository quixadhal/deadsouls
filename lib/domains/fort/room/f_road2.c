#include <lib.h>
#include <rooms.h>

#include "../customdefs.h"

inherit LIB_ROOM;

void create() {
    room::create();
    SetDayLight(30);
    SetNightLight(20);
    SetShort("Fort Road");
    SetDayLong("This cobblestone road wends its way south toward a town, and north to the main gate of the fortress. The road descends south toward the town, where it is at ground level.");
    SetNightLong("This cobblestone road wends its way south into darkness, and north to the glittering night lights of the fortress. The ground at the sides of the road slopes down sharply, making it impossible to go off the road.");
    SetItems(([
	({ "battlement", "battlements", "battlements on the massive walls" }) : "Up high, atop the massive walls of this fortress are crenellated battlements which permit defenders to rain hell on attackers while remaining behind cover.",
	({ "fortress", "wall", "walls", "fort", "fortress on the frontier", "fortress on the frontiers" }) : "The Fortress on the Frontier is built on a high plateau with slopes too steep to climb. Fort road is built on an incline, and rises far above ground level to meet the fortress gate. The battlements on the massive walls and the strategic position of the fort seem to make it a formidable redoubt in the event of a siege.",
	"town" : "You can't quite see it well from here, but it appears that Fort Road intersects an east-west path south of here, which leads toward a small town in the distance.",
	({ "stones", "cobblestones", "ruts", "deep ruts" }) : "Closer inspection of the road confirms that it has indeed been used a great deal.",
	({ "gate", "fortress gate", "glittering night lights of the fortress gate", "lights of the fortress gate", "night lights of the fortress gate" }) : "The road turns here, and northwest is the main entrance to the fortress.",
	({ "road", "cobblestone road", "fort road", "fortress road" }) : "This cobblestone road has been worn smooth by many years of frequent use. Deep ruts are worn where thousands of wagons have traveled. It rises north toward the fortress, and south toward a town.",
      ]));
    SetExits( ([
	"south" : "/domains/fort/room/f_road3",
	"north" : "/domains/fort/room/f_road1.c",
      ]) );
    SetClimate("temperate");

    SetTown("The Fortress");
}
void init(){
    ::init();
}
