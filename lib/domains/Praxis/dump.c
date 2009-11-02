#include <lib.h>

inherit LIB_ROOM;

void create() {
    ::create();
    SetSmell("default", "You nearly pass out from the stench of the place.");
    SetSmell("piles", "They smell of rotten elf food.");
    SetSmell("food", (: this_object(), "smell_food" :));
    SetProperty("light", 3);
    SetShort( "Praxis dump");
    SetLong(
            "The waste of an entire town accumulates here at the Praxis dump.  "
            "All around you are mounds upon mounds of trash and other stinking "
            "unidentifiable things.  A small alley leads east.");
    SetItems(
            (["mounds" : "Who knows what might be in amoung that crap?",
             "dump" : "The people of Praxis bring their junk here.",
             "trash" : "Anything and everything.",
             "alley" : "It leads back to Centre Path."]) );
    SetExits( 
            (["east" : "/domains/Praxis/alley2"]) );
}

void reset() {
    ::reset();
    SetSearch("mound", (: this_object(), "mound_searching" :));
    SetSearch("mounds", (: this_object(), "mound_searching" :));
}

void mound_searching() {
    message("my_action", "You found a dagger in one of the mounds of trash!",
            this_player());
    message("other_action", this_player()->query_cap_name()+
            " found a dagger in one of the mounds of trash.", this_object(),
            ({ this_player() }));
    new("/domains/Praxis/obj/weapon/dagger")->move(this_object());
    RemoveSearch("mounds");
    RemoveSearch("mound");
}

void smell_food(string str) {
    message("my_action", "You pass out from the stench of rotten elf food.",
            this_player());
    message("other_action", this_player()->query_cap_name()+
            " passes out from the stench of rotten elf food.", this_object(),
            ({ this_player() }));
    this_player()->add_sp(-3);
    this_player()->add_hp(-3);
    this_player()->set_paralyzed(10, "You are too nauseated to move!");
}
void init(){
    ::init();
}
