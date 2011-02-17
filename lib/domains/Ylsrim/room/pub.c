/*    /domains/Ylsrim/room/pub.c
 *    From the Dead Souls Mud Library
 *    An example pub room
 *    Created by Descartes of Borg 960302
 */

#include <lib.h>
#include <message_class.h> // defines MSG_SYSTEM

inherit LIB_ROOM;

// This is a special SetRead() function that allows players to read
// the menu

int readMenu(object who, string str) {
    string array tmp2 = ({});
    string array tmp = ({ sprintf("%:-20s %:-7s", "Drink", "Cost") });
    int langlevel = this_player()->GetLanguageLevel("Edhellen");
    object ob = present_file("/domains/Ylsrim/npc/lars");

    if( !ob ) { // lars is dead!
        this_player()->eventPrint("The menu is bloodstained and hard to read.");
        tmp = ({ translate("Bad wolf.", langlevel) });
    }
    else {
        foreach(string *item in keys(ob->GetMenuItems())) {
            tmp += ({ sprintf("%:-20s %d electrum", capitalize(item[0]),
                        to_int(ob->GetCost(item))) });
        }
        foreach(string element in tmp){
            element = translate(element, langlevel); 
            tmp2 += ({ element });
        }
        tmp = tmp2;
    }
    // show the menu a page at a time
    this_player()->eventPage(tmp, MSG_SYSTEM); // MSG_SYSTEM means ignore blocking
    return 1;
}
static void create() {
    room::create();
    SetTown("Ylsrim");
    SetClimate("indoors");
    SetAmbientLight(25);
    SetShort("Lars' Pub");
    SetLong("Lars' Pub is one of the most famous gathering places in all "
            "Ylsrim.  As you can see about you, however, it is not among the "
            "more homely ones.  Tables and stools are littered all about "
            "with a hodge-podge of writing in all different languages "
            "covering the wall.  A menu of drinks is about the only "
            "readable thing on the wall.  If you read Edhellen.");
    SetLanguage("Edhellen");
    SetInventory(([
                "/domains/Ylsrim/npc/lars" : 1,
                ]));
    SetListen("Rowdy party sounds make it hard to hear anything else.");
    SetSmell("The place smells like it is soaked in ale.");
    AddItem(({ "tables", "stools" }), "The tables and stools begin "
            "to migrate as the day goes on and customerizes socialize.  "
            "Of course, nothing moves them better than a good brawl.");
    AddItem(({ "menu", "drinks", "menu of drinks" }), "The menu is "
            "written in Edhellen.  Can you read it?", ({ "drink" }));
    AddItem(({ "writing", "walls" }), "Scribble in all different "
            "languages, and in many pens and many hands covers much of "
            "each wall.");
    SetExits(([ "east" : "/domains/Ylsrim/room/"+ "s_bazaar" ]));
    // Bring in the Bar Keep
    // make the writing and the menu readable
    SetRead("writing", (: readMenu :));
    SetRead("menu", (: readMenu :));
}
void init(){
    ::init();
}
