#include <lib.h>

inherit LIB_ROOM;

void init() {
    ::init();
    add_action("go_north", "go");
}

void create() {
    ::create();
    SetProperties( ([ "light" : 2, "indoors" : 1, "no castle" : 1 ]) );
    SetShort("Horace's general supply shop");
    SetLong(
            "Welcome to Horace's General Supply Shop!\n"
            "Horace's supply shop is the largest business running in Praxis "
            "to date. It was established long ago, and has earned itself "
            "a very high reputation over the years. A long counter stands "
            "at the back of the store, where you can often see Horace himself "
            "standing, bargaining with customers. Two windows frame the front "
            "wall of the store, and a cheery red door exits you back out to "
            "the main street. Horace buys and sells goods created for and found "
            "by adventurer's when he is in. The pasasge north is guarded "
            "by a magic field.");
    SetItems(
            (["shop" : "You can buy and sell things here.",
             "passage" : "Horace keeps the things he has for sale back there.",
             "road" : "Boc La Road."]) );
    SetInventory(([
                "/domains/Praxis/obj/mon/horace" : 1,
                ]));
    SetExits( 
            (["south" : "/domains/Praxis/e_boc_la2"]) );
    SetProperty("no castle", 1);
}

void reset() {
    ::reset();
    if(!present("horace")) 
        new("/domains/Praxis/obj/mon/horace")->move(this_object());
}

int go_north(string str) {
    if(str !="north") return notify_fail("What?\n");
    if(!creatorp(this_player())) {
        message("my_action", "The magic of Horace stops you.", this_player());
        message("other_action", this_player()->query_cap_name()+
                " is stopped by the magic of Horace.", this_object(),
                ({ this_player() }));
        return 1;
    }
    message("other_action", this_player()->query_cap_name()+
            " cannot be stopped by Horace's magic.", this_object(),
            ({ this_player() }));
    this_player()->eventMoveLiving("/domains/Praxis/storage", "north");
    return 1;
}
