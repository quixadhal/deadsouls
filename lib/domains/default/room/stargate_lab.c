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
    SetShort("Stargate Lab");
    SetLong("This is the test laboratory for the new Dead Souls Stargate system.\n"
            "%^BOLD%^%^GREEN%^An instruction sign is here.%^RESET%^");
    SetItems(([
                ({ "sign", "instruction sign", "instructions sign", "instruction", "instructions" }) : "A sign you can read. It appears to be instructions for operating the stargate.",
                ]));
    SetExits( ([
                "north" : "/domains/default/room/wiz_corr_east2.c",
                ]) );
    SetInventory(([
                "/domains/default/obj/stargate" : 1,
                ]));
    SetRead("sign", (: SignRead() :) );
    SetProperty("no attack", 1);

}
int CanReceive(object ob) {
    return room::CanReceive();
}

void init(){
    ::init();
}
