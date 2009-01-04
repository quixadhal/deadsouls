#include <lib.h>

inherit LIB_ROOM;

void create() {
    ::create();
    SetProperties( ([ "no castle" : 1,"light" : 2, "indoors" : 1]) );
    SetShort("a house with a thatched roof");
    SetLong(
            "The small thatch covered house has been standing for many years. "
            "It is the dwelling of an old, dishonored knight. The house "
            "is very sparse, devoid of any decorations or furnishings. The knight "
            "is quite poor and cannot afford any more than a few meager belongings. "
            "A rough wooden  door leads into a room to the south, "
            "and north leads back to Sun Alley.");
    SetItems(
            (["thatch" : "It is beginning to suffer from the weather "
             "and ill-keeping.",
             "house" : "It is very unimpressive.",
             "furnishing" : "Nothing.",
             "decoration" : "Absolutely nothing.",
             "door" : (: this_object(), "look_at_door" :) ]) );
    SetExits( ([ 
                "north" : "/domains/Praxis/sun1",
                ]) );
    SetDoor("/domains/Praxis/locked", "south");
    //present("knight")->eventForce("close door");
    //present("knight")->eventForce("lock door with key");
}

void reset() {
    object mon, key;

    ::reset();
    if(!present("knight")) {
        mon = new("/domains/Praxis/obj/mon/knight");
        mon->move(this_object());
        new("/domains/Praxis/obj/armour/helm")->move(mon);
        mon->command("wear helm");
        key = new(LIB_ITEM);
        key->SetKeyName("key");
        key->SetId( ({ "key", "honor key" }) );
        key->SetShort( "a wooden key");
        key->SetLong("The wooden key is very coarse, and rough. "
                "It doesn't look like it would lead to anything of "
                "importance.");
        key->SetValue(15);
        key->SetMass(40);
        key->move(mon);
    }
    if(query_reset_number() != 1) {
        //present("knight")->eventForce("close door");
        //present("knight")->eventForce("lock door with key");
    }
}

void look_at_door() {
    write("It is a door.");
}
void init(){
    ::init();
}
