#include <lib.h>
inherit LIB_ROOM;

static void create() {
    room::create();
    SetClimate("indoors");
    SetAmbientLight(30);
    SetShort("Dining Room");
    SetLong("This large room is rectangular in shape, "
            "and evidently is used as a formal "
            "dining room. Beautiful tapestries and "
            "paintings decorate the walls, and there "
            "is a sense of dignified majesty about "
            "this place.");
    SetItems( ([
                ({"tapestry","tapestries"}) : "Fascinating "
                "tapestries here apear to document the conquest "
                "of Anglia by a Norman king.",
                ({"painting","paintings"}) : "These are "
                "extraordinary oil portraits of renaissance "
                "nobility, and they are rendered with an "
                "eerily lifelike realism.",
                ]) );
    SetInventory( ([ 
                "/domains/town/obj/dining_table" : 1,
                ]) );
    SetExits( ([
                "east" : "/domains/town/room/mansion_dhall2"
                ]) );
}
void init(){
    ::init();
}
