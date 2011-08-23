#include <lib.h>
#include ROOMS_H

inherit LIB_ROOM;

string LongDesc(){
    string desc = "Immortals come here to visit parts of "+
        "the world they are building. The Creators' Hall is above. "+
        "To visit the Dead Souls "+
        "test and development mud, go west.";
    return desc;
}

static void create() {
    room::create();
    SetClimate("indoors");
    SetAmbientLight(30);
    SetShort("Domains Room");
    SetLong("This room provides a convenient access point to various domain start rooms and featured realms.");
    SetItems(([
                "Ylsrim" : "This is the entry point for the Ylsrim domain.",
                "campus" : "This is the entry point for the campus domain.",
                "examples" : "This is the entry point for the examples domain.",
                "town" : "This is the entry point for the town domain.",
                "cave" : "This is the entry point for the cave domain.",
                "praxis" : "This is the entry point for the praxis domain.",
                "learning" : "This is the entry point for the tutorial",
                ({ "sign" }) : "A sign you can read.",
                ]));
    SetEnters( ([
                "campus" : "/domains/campus/room/start",
                "ylsrim" : "/domains/Ylsrim/room/bazaar",
                "examples" : "/domains/examples/room/start.c",
                "cave" : "/domains/cave/room/start",
                "town" : "/domains/town/room/start",
                "learning" : "/domains/learning/room/start",
                "praxis" : "/domains/Praxis/square",
                ]) );
    SetProperty("no attack", 1);
    SetProperty("nopeer",1);
    SetExits(([
                "north" : "/domains/default/room/wiz_hall2",
                ]));

    SetInventory(([
                ]));

    SetRead("sign", (: load_object(ROOM_ARCH)->SignRead() :) );
}

int CanReceive(object ob) {
    if(playerp(ob) && !creatorp(ob) && !present("testchar badge",ob) &&
            !member_group(ob,"TEST")) {
        message("info","Creator staff only, sorry.", ob);
        return 0;
    }

    if(ob->GetRace() == "rodent"){
        message("info","You are repelled by rodenticide.",ob);
        return 0;
    }
    return 1;
}
void init(){
    ::init();
}
