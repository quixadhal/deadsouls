#include <lib.h>
inherit "/lib/std/room";

void create() {
    ::create();
    SetProperty("light", 0);
    SetProperty("indoors", 1);
    SetShort( "An orc soldier's chamber");
    SetLong(
            "You are in a dark living chamber inside the orc fortress. "
            "A passage west leads to the courtyard.");
    SetItems(
            (["chamber" : "It is small with little in the way of furnishings.",
             "furnishings" : "They are nothing of interest.",
             "passage" : "It leads out to the courtyard."]) );
    SetExits( (["west" : "/domains/Praxis/orc_valley/passage1"]) );
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
        ob->SetSkill("blunt", 50);
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
        thing->SetKeyName("mace");
        thing->SetId( ({ "mace", "battle mace" }) );
        thing->SetShort( "Battle mace");
        thing->SetLong( "It was clearly made by orcs.");
        thing->SetType("blunt");
        thing->SetClass(14);
        thing->SetAC(3);
        thing->SetMass(312);
        thing->SetValue(171);
        thing->move(ob);
        ob->eventForce("wield mace in right hand");
    }
}
void init(){
    ::init();
}

