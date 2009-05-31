#include <lib.h>

inherit LIB_ROOM;

void create() {
    ::create();
    SetProperty("light", 1);
    SetProperty("indoors", 1);
    SetSmell("default", "The building smells stale and decaying.");
    SetListen("default", "The boards which make up "
            "the floor are creaking under your weight.");
    SetShort( "a broken down building");
    SetLong(
            "Broken boards and rotting thatch have left this place very "
            "dangerous for walking.  The squeaks of diseased rodents "
            "tell you that this place is no longer run by humanoids, but "
            "bits of evidence here and there suggest recent humanoid passage. "
            "The alley lies north of here.");
    SetItems(
            (["board" : "It is rotting from exposure to the weather.",
             "boards" : "You do not want to rely on them for support.",
             "thatch" : "It once formed the roof of the building, but no more.",
             "roof" : "A patchwork of rotting thatch.",
             "evidence" : "Dust left unsettled.",
             "dust" : "It looks like it has not been very long settled in places.",
             "foo" : "What do you mean by foo?"]) );
    SetExits( 
            (["north" : "/domains/Praxis/alley2"]) );
}

void reset() {
    object rat;

    ::reset();
    if(!present("rat")) {
        rat = new("/lib/npc");
        rat->SetKeyName("rat");
        rat->SetId( ({ "rat", "filthy rat", "a filthy rat" }) );
        rat->SetLevel(1);
        rat->SetShort( "A filthy rat");
        rat->SetLong("A disgusting little rodent.");
        rat->SetRace( "rodent");
        rat->SetCurrency("silver", random(10));
        rat->SetHealthPoints(60+random(10));
        rat->SetMorality(-1);
        rat->move(this_object());
    }
    if(!present("torch")) 
        new("/domains/town/obj/torch")->move(this_object());
}
void init(){
    ::init();
}
