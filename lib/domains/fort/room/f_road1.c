#include <lib.h>
#include <rooms.h>

#include "../customdefs.h"

inherit LIB_ROOM;

void create() {
    room::create();
    SetDayLight(30);
    SetNightLight(25);
    SetShort("Fort Road");
    SetDayLong("This cobblestone road wends its way south toward a town, and northwest to the main gate of the fortress. The road descends toward the town, and to the far south it is at ground level. The walls of the fort loom large here, and you can see you are well in range of the defensive positions of its battlements.");
    SetNightLong("This cobblestone road wends its way south into darkness, and northwest to the glittering night lights of the fortress gate. The ground at the sides of the road slopes down sharply, making it impossible to go off the road.");
    SetItems(([
	({ "battlement", "battlements", "battlements on the massive walls" }) : "Up high, atop the massive walls of this fortress are crenellated battlements which permit defenders to rain hell on attackers while remaining behind cover.",
	({ "fortress", "wall", "walls", "fort", "fortress on the frontier", "fortress on the frontiers" }) : "The Fortress on the Frontier is built on a high plateau with slopes too steep to climb. Fort road is built on an incline, and rises far above ground level to meet the fortress gate. The battlements on the massive walls and the strategic position of the fort seem to make it a formidable redoubt in the event of a siege.",
	"town" : "You can't quite see it well from here, but it appears that Fort Road intersects an east-west path south of here, which leads toward a small town in the distance.",
	({ "stones", "cobblestones", "ruts", "deep ruts" }) : "Closer inspection of the road confirms that it has indeed been used a great deal.",
	({ "gate", "fortress gate", "glittering night lights of the fortress gate", "lights of the fortress gate", "night lights of the fortress gate" }) : "The road turns here, and northwest is the main entrance to the fortress.",
	({ "road", "cobblestone road", "fort road", "fortress road" }) : "This cobblestone road has been worn smooth by many years of frequent use. Deep ruts are worn where thousands of wagons have traveled. It rises north toward the fortress, and south toward a town.",
      ]));
    SetClimate("temperate");
    SetTown("The Fortress");
    SetExits( ([
	"northwest" : "/domains/fort/room/gate",
	"south" : "/domains/fort/room/f_road2.c",
      ]) );
}
void init(){
    ::init();
}
