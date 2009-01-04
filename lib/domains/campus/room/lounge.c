#include <lib.h>
inherit LIB_ROOM;

void AddStuff(){
    object ob;
    ob = new("/lib/bboard");
    ob->SetKeyName("chalkboard");
    ob->SetId( ({ "board", "chalkboard","bulletin board","visitor's board"}));
    ob->set_board_id("visitor_board");
    ob->set_max_posts(30);
    ob->SetShort("The Visitor's Board");
    ob->SetLong("This is the visitor's bulletin board. Students and visitors use it to "+
            "post messages, comments ans questions about LPC University to each other and "+
            "LPC University creators. Occasionally creators also post hints, suggestions and "+
            "clues. To post something, type: post <message name>. When "+
            "finished writing, enter a single period on a blank line, then at "+
            "the colon prompt (:) type a lower-case x and return. \n");
    ob->eventMove(this_object());
    ob->SetNoClean(0);
}
static void create() {
    room::create();
    SetClimate("indoors");
    SetAmbientLight(40);
    SetShort("the student lounge");
    SetLong("This is the LPC University New Main building student lounge. "+
            "It is rather bare at the moment, as the building is still in early stages of "+
            "construction. You see some marks on the floor where furniture is to be "+
            "placed. The main corridor lies north, and a window overlooks the campus "+
            "on the south wall.");
    SetItems(([
                ({"here","room","lounge"}) : "This seems to be intended to be a student "+
                "lounge.",
                "building" : "You are in New Main, LPC University' main administrative building.",
                ({"mark","marks"}) : "Some small chalkmarks and tape strips indicate "+
                "where furniture is to go.",
                ({"floor","ground"}) : "Standard off-white linoleum tile floor, in case clumsy students "+
                "plan on eating in here.",
                ({"corridor","hall"}) : "The main corridor on the first floor of New Main.",
                "window" : "A window set into the south wall, overlooking the campus. Strangely, "+
                "there seems to be nothing outside the window...no campus, no light, no stars...nothing. "+
                "It's as though the administrative building were suspended in a vacuum.",
                "campus" : "It seems oddly absent.",
                ({"wall","south wall"}) : "A wall with a window in it."]));
    SetExits( ([
                "north" : "/domains/campus/room/corridor2.c",
                ]) );
    SetInventory(([
                "/domains/campus/obj/locker" : 1,
                ]));
    SetProperty("no attack", 1);
    AddStuff();
}
void init(){
    ::init();
}
