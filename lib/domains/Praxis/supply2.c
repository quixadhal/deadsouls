#include <lib.h>

inherit LIB_ROOM;

void create() {
    ::create();
    SetProperties( ([ "light" : 2, "indoors" : 1, "no castle" : 1 ]) );
    SetShort("Horace's General Supply Shop");
    SetLong(
            "Welcome to Horace's General Supply Shop!\n"
            "Horace buys and sells goods created for and found "
            "by adventurer's when he is in.  The store opens out "
            "onto Boc La Road south.  There is a passage to the "
            "north guarded by a magic field.");
    SetItems(
            (["shop" : "You can buy and sell things here.",
             "passage" : "Horace keeps the things he has for sale back there.",
             "road" : "Boc La Road."]) );
    SetExits( ([ "south" : "/domains/Praxis/e_boc_la2",
                "north" : "/domains/Praxis/storage" ]) );
    set_pre_exit_functions( ({ "north" }), ({ "go_north" }) );
    add_sky_event( (: "shop_closing" :) );
}

void reset() {
    ::reset();
    if(query_night()) return;
    if(!present("horace")) 
        new("/domains/Praxis/obj/mon/horace")->move(this_object());
}

    string shop_long(string str) {
        if(query_night()) 
            return "Horace's is now closed.";
    }

void shop_closing(string str) {
    object ob;

    if(str == "night" && ob = present("horace", this_object())) 
        ob->destruct();
    else if(str == "dawn" && !present("horace", this_object()))
        new("/domains/Praxis/obj/mon/horace")->move(this_object());
}

int go_north(string str) {
    if(!creatorp(this_player())) {
        message("my_action", "The magic of Horace stops you.", this_player());
        message("other_action", this_player()->query_cap_name()+
                " is stopped by the magic of Horace.", this_object(),
                ({ this_player() }));
        return 0;
    }
    message("other_action", this_player()->query_cap_name()+
            " cannot be stopped by Horace's magic.", this_object(),
            ({ this_player() }));
    return 1;
}
void init(){
    ::init();
}
