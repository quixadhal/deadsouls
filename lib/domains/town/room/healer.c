#include <lib.h>
inherit LIB_SHOP;
inherit LIB_AMBIANCE;

int ReadList(){
    write("AVAILABLE PROCEDURES:\n"
            "\thealing, cost 200:\t\tfacilitate rapid recovery from wounds\n"
            "\tregeneration, cost 1200:\tgrow back severed limbs\n"
            "\texcision, cost 700:\t\tremove bullets\n"
            "\tpoison antidote, cost 200:\tmitigate the effects of poisoning\n"
            "\tclaritin, cost 10:\t\talleviate cold symptoms\n"
            "\n"
            "Example: If you are badly hurt and need healing:\n\n"
            "\"buy healing slip from james\"\n"
            "\n"
            "If you've been poisoned: \n\n"
            "\"buy antidote from james\"\n"
            "\n"
            "Once you have your slip, go west to see the doctor and "
            "give the slip to him.\n"

         );
    return 1;
}
static void create() {
    ::create();
    SetClimate("indoors");
    SetAmbientLight(30);
    SetShort("The Healers Guild");
    SetLong("You are in the Healer's Guild. People come "
            "here when they have medical problems, and "
            "for a price, they are helped. "
            "It seems that the doctor's work area is "
            "west of here. Saquivor Road is east."
            "\n%^GREEN%^There is a list here you can read.%^RESET%^");
    SetItems( ([
                ({"list","list on the wall"}) : "A list of available procedures",
                "wall" : "A flat, vertical structure supporting the ceiling.",
                ({"work area","doctor's work area"}) : "It is west of here.",
                ({"road","tavern road"}) : "The road is outside, east of here."
                ]) );
    SetRead( ({"list","list on wall"}) , (: ReadList :) );
    SetInventory(([
                "/domains/town/obj/bbucket" :1,
                "/domains/town/npc/james" : ({60, 1})
                ]) );
    SetExits( ([
                "east" : "/domains/town/room/road",
                "out" : "/domains/town/room/road",
                "west" : "/domains/town/room/chamber",
                ]) );
    SetProperty("no attack", 1);

}
void init(){
    ::init();
}
