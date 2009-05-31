#include <lib.h>
inherit "/lib/std/room";

void create() {
    ::create();
    SetProperty("light", 0);
    SetProperty("indoors", 1);
    SetShort( "An orc soldier's chamber");
    SetLong(
            "You are in a dark living chamber inside the orc fortress. "
            "A passage east leads to the courtyard.");
    SetItems(
            (["chamber" : "It is small with little in the way of furnishings.",
             "furnishings" : "They are nothing of interest.",
             "passage" : "It leads out to the courtyard."]) );
    SetExits( (["east" : "/domains/Praxis/orc_valley/passage2"]) );
}

void reset() {
    object ob, thing;

    ::reset();
    if(!present("orc")) {
        ob = new("/lib/npc");
        ob->SetKeyName("soldier");
        ob->SetId( ({ "orc", "orc soldier", "soldier" }) );
        ob->SetShort( "Orc soldier");
        ob->SetLong( "He is ugly and very unhappy about "
                "your presence.");
        ob->SetClass("fighter");
        ob->SetSkill("two handed", 50);
        ob->SetRace( "orc");
        ob->SetGender("male");
        ob->SetLevel(7);
        ob->SetHealthPoints(129);
        ob->SetMorality(-135);
        ob->SetAggressive( 18);
        ob->SetRace("human");
        ob->SetWielding_limbs( ({ "right hand", "left hand" }) );
        ob->move(this_object());

        thing = new(LIB_ITEM);
        thing->SetKeyName("sword");
        thing->SetId( ({ "sword", "two-handed sword" }) );
        thing->SetShort( "Two-handed sword");
        thing->SetLong( "It was clearly made by orcs.");
        thing->SetType("two handed");
        thing->SetClass(15);
        thing->SetMass(450);
        thing->SetValue(299);
        thing->move(ob);
        ob->eventForce("wield sword in right hand and left hand");
    }
}
void init(){
    ::init();
}

