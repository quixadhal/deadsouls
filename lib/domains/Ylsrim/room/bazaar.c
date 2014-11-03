/*    /domains/Ylsrim/room/bazaar.c
 *    From the Dead Souls Mud Library
 *    An example of a simple room
 *    created by Descartes of Borg 950928
 */

#include <lib.h>

inherit LIB_ROOM;

int ReadSign() {
    this_player()->more("/domains/town/txt/warning_sign.txt");
    return 1;
}

static void create() {
    object ob; // this will be used to create dummy items

    // setup the stuff you inherit
    room::create();
    // set the town, as native Ylsrimites can do special things in Ylsrim
    SetTown("Ylsrim");
    // the climate in Ylsrim is arid because it is a desert town
    SetClimate("arid");
    // the is the amount of light the street lamps make at night
    SetAmbientLight(30);
    // the obvious exits tell people how to leave the room
    // set the sounds people hear
    SetListen("Vendors are begging you to come look at their goods.");
    // set the short description... should not be capitalized
    SetShort("the central bazaar of Ylsrim");
    // this is the description people see at day
    SetDayLong("Shops and vendors selling all types of goods fill "
            "the central bazaar of Ylsrim.  Local villagers and "
            "exotic travellers are passing about you in search of a "
            "bargain, or even in some cases, in search of trouble.  "
            "Kaliid Road crosses Ylsrim from the west to the east just "
            "north of you, with an armory and a weapon shop straddling "
            "both sides of the bazaar.  The bazaar stretches on south."
            "\n%^RED%^There is a sign here you can read.%^RESET%^");
    SetSkyDomain("town");
    // this is the description people see at night
    SetNightLong("Though most areas of Ylsrim are peaceful right now, "
            "the central bazaar is still filled with people doing "
            "business, even though most of the shops are closed.  "
            "Kaliid Road is just north of here, and a closed armory "
            "and weapons shop sit on both sides of the bazaar.  "
            "The bazaar stretches south."
            "\n%^RED%^There is a sign here you can read.%^RESET%^");
    // set up dummy items... the first few are simple
    AddItem(({ "villager", "traveller" }),
            "Both villagers and travellers are out looking for good "
            "deals.");
    AddItem(({ "sign"}), "A sign. To read it, 'read sign'.");
    AddItem(({ "road" }), "It leads through Ylsrim from the Great Desert "
            "in the west towards more fertile land east.", ({ "kaliid" }));
    // people need to enter the armory, so we need to do something special
    // first create an armory dummy item
    SetRead("sign", (: ReadSign :) );
    ob = new(LIB_DUMMY, ({ "armory", "shop" }),
            function(string str) {
            if( query_night() ) {
            return "It is closed.";
            }
            else {
            return "It is open.";
            }
            }, ({ "armor" }));
    // set it so when people 'enter armory', they move to the armory
    ob->SetEnter("/domains/Ylsrim/room/"+ "armory");
    // now add it as an item
    AddItem(ob);
    // do the same stuff for the weaponry
    ob = new(LIB_DUMMY, ({ "weaponry", "shop" }),
            function(string str) {
            if( query_night() ) {
            return "It is closed.";
            }
            else {
            return "It is open.";
            }
            }, ({ "weapon" }));
    ob->SetEnter("/domains/Ylsrim/room/"+ "weaponry");
    AddItem(ob);
    // set the directional exits
    SetExits( ([ "north" : "/domains/Ylsrim/room/"+ "kaliid4",
                "south" : "/domains/Ylsrim/room/"+ "s_bazaar" ]) );
}
void init(){
    ::init();
}
