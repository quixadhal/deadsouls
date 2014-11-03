#include <lib.h>

inherit LIB_ROOM;

void create() {
    ::create();
    SetProperty("light", 2);
    SetProperty("no castle", 1);
    SetShort( "The entrance to the Valley of the Orcs");
    SetLong(
            "In the shadow of the Daroq Mountains to the west, and just north "
            "of the great highway is the valley of the orcs.  Here you find "
            "the entrance to a huge stone orc fortress surrounded by dense "
            "forest.  The great highway is south of here.");
    SetItems(
            (["mountains" : "They are a dark range of mountains to the "
             "northwest. To the southwest, the mountains are go by "
             "the name of Destiny Mountains.",
             "highway" : "You cannot see it through the trees.",
             "valley" : "It sits in the shadow of the Daroq Mountains.",
             "entrance" : "A grand stone gateway into the orc fortress.",
             "stone" : "It is an old grey stone that has stood here for ages.",
             "fortress" : "It is the home of the orcs who live in Orc Valley.",
             "forest" : "It is terribly dense here.",
             "trees" : "They are huge and grow close together.",
             "gateway" : "The entrance to the orc fortress."]) );
    SetSkyDomain("town");
    SetExits( 
            (["north" : "/domains/Praxis/orc_valley/open",
             "south" : "/domains/Praxis/highway3"]) );
    AddExit("north", "/domains/Praxis/orc_valley/open", (: "go_north" :) );
}

void reset() {
    int i, x;
    object ob, thing;

    ::reset();
    if(!present("orc")) {
        for(i=0; i< 3; i++) {
            ob = new(LIB_NPC);
            ob->SetKeyName("guard");
            ob->SetId( ({ "orc", "orc guard", "guard" }) );
            ob->SetShort( "Orc guard");
            ob->SetLong( "He is not happy to have people like you around.");
            ob->SetRace( "orc");
            ob->SetGender("male");
            ob->SetLevel(4);
            ob->SetHealthPoints(77);
            ob->SetMorality(-75);
            ob->SetAggressive( 15);
            ob->SetWielding_limbs( ({ "right hand", "left hand" }) );
            ob->move(this_object());

            thing = new(LIB_ITEM);
            thing->SetKeyName("axe");
            thing->SetId( ({ "axe", "hand axe" }) );
            thing->SetShort( "Hand axe");
            thing->SetLong( "It was clearly made by orcs.");
            thing->SetType("knife");
            thing->SetClass(8);
            thing->SetMass(223);
            thing->SetValue(90);
            thing->move(ob);
            ob->eventForce("wield axe in right hand");
        }
    }
}

int go_north() {
    if(present("orc")) {
        present("orc")->eventForce("speak in orcish No one gets past us!");
        return 0;
    }
    else return 1;
}

void init(){
    ::init();
}
