#include <lib.h>

inherit LIB_ROOM;

void create() {
    ::create();
    SetProperty("light", -2);
    SetProperty("indoors", 1);
    SetProperty("no teleport", 1);
    SetProperty("no castle", 1);
    SetShort( "Deep inside a tunnel underneath the mountains");
    SetLong(
            "The tunnel of goblins comes to an end here, deep inside "
            "the evil Daroq Mountains.");
    SetExits( (["up" : "/domains/Praxis/mountains/tunnel2"]) );
}

void reset() {
    ::reset();
    SetSearch("default", (: this_object(), "ball" :));
    SetSearch("tunnel", (: this_object(), "ball" :));
}

void ball() {
    object ob;

    write("You find a crystal ball.");
    say(this_player()->query_name()+" finds a crystal ball.");
    RemoveSearch("default");
    RemoveSearch("tunnel");
    ob = new("/domains/Praxis/obj/magic/ball");
    if(ob->move(this_player())) {
        message("my_action", "You drop the crystal ball!", this_player());
        message("other_action", this_player()->query_cap_name()+
                " drops the crystal ball!", this_object(),({this_player()}));
        ob->move(this_object());
    }
}

