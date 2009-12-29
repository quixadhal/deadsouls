#include <lib.h>

inherit LIB_ROOM;

void init() {
    ::init();
    add_action("read", "read");
}

void create() {
    ::create();
    SetProperties( ([ "light": 2, "indoors" : 1, "no castle" : 1 ]) );
    SetShort( "Lars' tavern and bar");
    SetLong(
            "Welcome to Lars' Tavern!\n"
            "Lars' Tavern is a very old and well established pub. It "
            "has recently been remodelled, and you can tell that "
            "business is picking up by the general hum of chatter "
            "in the room. The floor is made out of polished wood, "
            "as is the counter that stands in the back of the room. "
            "Pictures of the different lands of Nightmare hang "
            "on the wall, as do some wanted posters of the hunted "
            "rogues of this land.");
    SetItems(
            (["pub" : "The most hoppin' place in Praxis.",
             "wall" : "The walls are covered with various pictures.",
             ({ "posters", "poster", "wanted posters" }) :
             "The wanted posters are very old and brittle. They mark "
             "the faces of the rogues of this land.",
             ({ "pictures" , "picture" }) :
             "The pictures are of different parts of Nightmare, painted "
             "by several of the villages' aspiring artists.",
             "bar" : "The bar is actually Lars' Tavern. It is a very "
             "clean and well run establishment.",
             "counter" : "The wooden counter stands at the back of the "
             "room and houses all of Lars' drinks.",
             "room" : "You are in the main room of Lars' Tavern.",
             "foo" : "bar",
             "tavern" : "The tavern is the place that the people of "
             "Praxis come to when they want to have a drink or just "
             "be social.",
             "floor" : "The floor is made out of solid oak.",
             "list" : "It looks like a list of drinks. Read it to see "
             "what's available."]) );
    SetExits( 
            (["north" : "/domains/Praxis/w_boc_la1",
             "east" : "/domains/Praxis/s_centre1"]) );
    SetProperties( ([  "no castle": 1, "no attack" : 1 ]) );
}

void reset() {
    ::reset();
    if(!present("list")) 
        //new("std/obj/player_list")->move(this_object());
        if(!present("lars")) 
            new("/domains/Praxis/obj/mon/lars")->move(this_object());
}

int read(string str) {
    object ob;
    int i;

    ob = present("lars");
    if(!ob) {
        write("You cannot read the menu, as it is splattered with blood.");
        return 1;
    }
    message("Ninfo", "The following classic drinks are served at the Lars Pub!\n", this_player());
    message("Ninfo", "-----------------------------------------------------------\n", this_player());
    message("Ninfo", "A firebreather\t\t\t"+ ob->get_price("firebreather")+" gold\n", this_player());
    message("Ninfo", "A special of the house\t\t"+ob->get_price("special")+" gold\n", this_player());
    message("Ninfo", "A pale ale\t\t\t"+ob->get_price("ale")+" gold\n", this_player());
    message("Ninfo", "-----------------------------------------------------------\n", this_player());
    message("Ninfo", "<buy drink_name> gets you a drink.\n", this_player());
    return 1;
}









