#include <lib.h>
#include <props.h>

inherit LIB_BED;
inherit LIB_SMELL;

static void create() {
    ::create();
    SetKeyName("orcish altar");
    SetId( ({ "altar","platform","pedestal" }) );
    SetAdjectives( ({ "simple","stone" }) );
    SetShort("a simple stone altar, dripping blood");
    SetLong("This is a ceremonial orcish  altar, where "+
      "holy offerings and sacrifices are made. It is "+
      "sheeted in dried blood and rotting tissue.");
    SetItems( ([
        ({"blood","dried blood","tissue","rotting tissue"}) : ""+
        "This appears to be the accumulated detritus of "+
        "numerous sacrifices.",
      ]) );
    SetMass(3000);
    SetBaseCost("silver",15);
    SetMaxSitters(2);
    SetMaxLiers(1);
    SetMaxCarry(5000);
    inventory_visible();
    inventory_accessible();
    SetSmell( ([ "default" : "The altar is nauseatingly rank."]) );
}
mixed CanGet(object ob) { return "The altar is dug into the floor and does not move.";}
