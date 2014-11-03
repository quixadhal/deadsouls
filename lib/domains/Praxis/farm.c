#include <lib.h>

inherit LIB_ROOM;

void create() {
    ::create();
    SetProperty("light", 3);
    SetProperty("no castle", 1);
    SetShort( "Praxis farmlands");
    SetLong(
            "The area which you have wandered into is the farmlands of Praxis. "
            "Large cornfields surround you, stretching for miles to both the "
            "east and the west. The stalks grow strong and tall, making "
            "it hard to see. Off in the far distance you can see the speck of "
            "what appears to be a farmhouse.");
    SetItems(
            (["stalks" : "Huge stalks of corn covering so many acres of land.",
             "stalk" : "A huge stalk of corn.",
             "farmhouse" : "The farmhouse is far off in the distance, and "
             "impossible to get to.",
             "field" : "This place must supply the entire area with food.",
             "corn" : "A food some people like to eat.  It is "
             "%^YELLOW%^yellow%^RESET%^."]) );
    SetSkyDomain("town");
    SetExits( 
            (["north"	: "/domains/Praxis/w_boc_la2"]) );
}

void reset() {
    ::reset();
    SetSearch("field", (: this_object(), "stalk_searching" :));
    SetSearch("stalks", (: this_object(), "stalk_searching" :));
}

void stalk_searching() {
    message("my_action", "After searching through all the long stalks "
            "you find a  shovel among the them!", this_player());
    message("other_action", this_player()->query_cap_name()+" finds "
            "a shovel among the stalks.", this_object(), ({ this_player() }));
    new("/domains/Praxis/obj/misc/shovel")->move(this_object());
    RemoveSearch("stalks");
    RemoveSearch("field");
}
void init(){
    ::init();
}
