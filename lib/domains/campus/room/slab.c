#include <lib.h>
#include <daemons.h>

inherit LIB_ROOM;

int SignRead(){
    string list;
    list = implode(keys(STARGATE_D->GetStargates()),", ");
    write("These are Stargate operation instructions.");
    write("The Dead Souls stargate system provides teleportation within "
            "the stargate network. To use the stargate network, one finds an "
            "idle stargate, then dials the name of some other known stargate. "
            "One then enters the outbound stargate, and if things go well, teleportation "
            "to the dialed stargate occurs. To travel to Uruk, if that were a "
            "valid stargate name, you would: dial uruk\n"
            "Once the stargate activates: enter stargate");
    write("Currently available stargates are:");
    write("%^BOLD%^%^YELLOW%^"+list+"%^RESET%^");
    return 1;
}

static void create() {
    object ob;
    room::create();
    SetClimate("indoors");
    SetAmbientLight(30);
    SetShort("Science Building Stargate Lab");
    SetLong("This large room is the testing ground for the newly discovered stargate technology that allows for instant teleportation between distant locations. The main Science Building hallway is south.\n"
            "%^BOLD%^%^GREEN%^An instruction sign is here.%^RESET%^");
    SetItems(([
                ({ "sign", "instruction sign", "instructions sign", "instruction", "instructions" }) : "A sign you can read. It appears to be instructions for operating the stargate.",
                ]));
    SetInventory(([
                "/domains/campus/obj/stargate" : 1,
                "/domains/campus/npc/kleiner" : 1,
                ]));
    SetExits(([
                "south" : "/domains/campus/room/science2",
                ]));
    SetRead("sign", (: SignRead() :) );
}
int CanReceive(object ob) {
    if(!ob) return 0;
    return room::CanReceive(ob);
}

void init(){
    ::init();
}
