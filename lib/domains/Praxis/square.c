#include <lib.h>

inherit LIB_ROOM;

string ReadSign(){
    this_player()->more("/domains/town/txt/praxis_sign.txt");
    return "";
}

void create() {
    object borg;

    ::create();
    SetProperty("light", 3);
    SetProperty("night light", 2);
    SetProperty("no steal", 1);
    SetProperty("no magic", 1);
    SetProperty("no attack", 1);
    SetProperty("no castle", 1);
    SetListen("default", "The sounds of a busy adventuring town are all about.");
    SetShort( "Monument Square");
    SetLong( "You are in Monument Square, once known as Krasna Square. "
            "The two main roads of Praxis intersect here, where all of "
            "Nightmare's people gather in joy and sorrow.  The road running "
            "north and south is called Centre Path, while Boc La Road is the "
            "name of the road running east and west."
            "\n%^RED%^There is a sign here you can read.%^RESET%^");
    SetExits( 
            (["north" : "/domains/Praxis/n_centre1",
             "south" : "/domains/Praxis/s_centre1",
             "east" : "/domains/Praxis/e_boc_la1",
             "west" : "/domains/Praxis/w_boc_la1"]) );

    //new("/domains/Praxis/obj/misc/watchtower")->move(this_object());
    SetItems(
            (["road" : "Boc La Road leads east toward the adventurer's "
             "hall and the Praxis Monastary.",
             "path" : "Centre Road leads south toward the bank and post office.",
             "sign" : "A sign you can read.",
             "square" : "Krasna Square is the central point of Praxis."]) );
    SetInventory(([
                "/domains/Praxis/obj/misc/stargate" : 1,
                ]));
    SetRead("sign", (: ReadSign :) );
    SetCoordinates("1000,-1000,0");
    SetSkyDomain("town");
}

int CanReceive(object ob) { 
    string start;

    if (previous_object()->is_player()) {
        start = previous_object()->getenv("start");
        if (!start) return ::CanReceive(ob);
        if (9>strlen(start)) return ::CanReceive(ob);
        if ("/d/" == start[0..2]) 
            previous_object()->setenv("start", 
                    "/domains/"+start[3..strlen(start)]);
        if ("d/" == start[0..1]) 
            previous_object()->setenv("start", 
                    "/domains/"+start[2..strlen(start)]);
        if ("/wizards/" == start[0..8]) 
            previous_object()->setenv("start", 
                    "/realms/"+start[9..strlen(start)]);
        if ("wizards/" == start[0..7]) 
            previous_object()->setenv("start", 
                    "/realms/"+start[8..strlen(start)]);

        if("/domains/standard/" ==start[0..17])
            previous_object()->set_primary_start("/domains/Praxis/"+
                    start[18..strlen(start)]);
        if("domains/standard/"==start[0..16])
            previous_object()->set_primary_start("/domains/Praxis/"+
                    start[17..strlen(start)]);
    }
    return ::CanReceive(ob);
}
void init(){
    ::init();
}
