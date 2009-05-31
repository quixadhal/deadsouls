#include <lib.h>
#include <props.h>

inherit LIB_BED;
inherit LIB_SMELL;

static void create() {
    bed::create();
    SetKeyName("altar");
    SetId( ({ "altar","platform","pedestal" }) );
    SetAdjectives( ({ "orcish", "simple","stone" }) );
    SetShort("a simple stone altar, dripping blood");
    SetLong("This is a ceremonial orcish  altar, where "+
            "holy offerings and sacrifices are made. It is "+
            "sheeted in dried blood and rotting tissue.");
    SetItems( ([
                ({"blood","dried blood","tissue","rotting tissue"}) : ""+
                "This appears to be the accumulated detritus of "+
                "numerous sacrifices.",
                ]) );
    SetPreventGet("The altar is dug into the floor and does not move.");
    SetInventory(([
                "/domains/town/weap/boobytrap_dagger" : 1,
                "/domains/town/armor/boobytrap_ring" : 1,
                ]));
    SetMass(3000);
    SetBaseCost("silver",15);
    SetMaxSitters(2);
    SetMaxLiers(1);
    SetMaxCarry(5000);
    SetSmell( ([ "default" : "The altar is nauseatingly rank."]) );
}

void init(){
    bed::init();
}
