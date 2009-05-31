#include <lib.h>
inherit LIB_SHOP;

int read_sign();
static void create() {
    ::create();
    SetClimate("indoors");
    SetAmbientLight(30);
    SetShort("The Magic Shop");
    SetLong("This is the local magic shop. Magical items can be bought and sold here. The storeroom is upstairs and the town Mages' Guild is south.");
    SetItems(([
                ({"store","shop","place"}) : "This is the local magic shop.",
                ({ "sign" ,"sign on the wall" }): "A sign on the wall, meant for reading.",
                ]));
    SetExits( ([
                "south" : "/domains/town/room/magic_guild",
                "east" : "/domains/town/room/south_road1",
                "up" : "/domains/town/room/magic_shop2.c",
                ]) );
    SetRead("sign", (: read_sign :) );
    SetInventory( ([
                "/domains/town/npc/oana" : ({60, 1}),
                "/domains/town/obj/bbucket" : 1,
                ]) );
}
int read_sign(){
    write("HOW TO CONDUCT BUSINESS HERE\n");
    write("list : Get a list of all the items Oana has for sale");
    write("appraise <item> : Ask Oana to tell you how much she would pay you for your item");
    write("price <item> : Ask Oana the price of her item");
    write("show <item> : Ask Oana for a closer look at her item\n");
    write("\nbuy <item> from oana\nsell <item> to oana\n");
    write("Prices shown are in silver coins.");
    write("\n\nWe're still under construction, so there's");
    write("not much to buy. But if you want, you can go ");
    write("south to the mage's guild and join up!");

    return 1;
}
void init(){
    ::init();
}
