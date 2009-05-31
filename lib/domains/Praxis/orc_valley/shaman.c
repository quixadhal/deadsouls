#include <lib.h>

inherit LIB_ROOM;

void create() {
    ::create();
    SetProperty("light", 1);
    SetProperty("indoors", 1);
    SetProperty("no castle", 1);
    SetShort( "The chamber of the orc shaman");
    SetLong(
            "This secret room is the chamber of the orc shaman.  "
            "It is most certain that you should not be here.");
    SetItems(
            (["chamber" : "It is a roundish room and dimly lit."]) );
    SetExits( 
            (["south" : "/domains/Praxis/orc_valley/open",
             "north" : "/domains/Praxis/orc_valley/treasure"]) );
    AddExit("north", "/domains/Praxis/orc_valley/treasure", (: "go_north" :) );
}

void reset() {
    object mon, ob;

    ::reset();
    if(!present("shaman")) {
        mon = new("/domains/Praxis/obj/mon/orc_shaman");
        mon->move(this_object());
        ob = new(LIB_ITEM);
        ob->SetKeyName("key");
        ob->SetId( ({ "key", "stone key", "a stone key", "orc_treasure" }) );
        ob->SetShort( "A stone key");
        ob->SetLong( "There is nothing special about it.");
        ob->SetMass(190);
        ob->SetValue(30);
        ob->move(mon);
    }
}

int go_north() {
    if(!present("shaman")) return 1;
    write("The shaman smashes you as you try to go north!");
    this_player()->add_hp(-(random(10)));
    return 0;
}
void init(){
    ::init();
}

