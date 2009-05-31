#include <lib.h>
#include <position.h>

inherit LIB_VEHICLE;

static void create() {
    vehicle::create();
    SetKeyName("hoverpod");
    SetId(({"pod", "vehicle","transport"}));
    SetAdjectives(({"small","shiny","cramped","metallic","hovering"}));
    SetShort("a small hoverpod");
    SetLong("This is a shiny metallic sphere about 1.5 meters across, designed for containing "
            "an occupant and their stuff in its cramped interior and providing transportation.");
    SetVehicleInterior("This is the cramped interior of a shiny sphere used for "
            "transportation. Since it hovers, you can use the command \"fly\" to get "
            "around with it, like: fly north");
    SetRace("vehicle");
    SetStat("strength", 50);
    SetPosition(POSITION_FLYING);
    SetMelee(0);
    SetNoClean(1);
    SetMount(1);
    SetMountStyle("driven");
    SetVisibleRiders(0);
    SetOpacity(0);
    SetMaxHealthPoints(20);
}
void init(){
    ::init();
}
