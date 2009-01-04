#include <lib.h>
#include <props.h>

inherit LIB_BED;
inherit LIB_SMELL;

static void create() {
    ::create();
    SetKeyName("beat up couch");
    SetId( ({ "couch","sofa","recliner" }) );
    SetAdjectives( ({ "beat up","beat-up","old" }) );
    SetShort("a beat-up old couch");
    SetLong("This is a beat-up, worn and dirty couch, the kind "
            "one might expect in a university dorm room.");
    SetMass(500);
    SetBaseCost("silver",15);
    SetMaxSitters(2);
    SetMaxLiers(1);
    SetSmell( ([
                "default" : "It smells dirty and gross. There's a "
                "faint whiff of dog urine."
                ]) );
}

void init(){
    ::init();
}
