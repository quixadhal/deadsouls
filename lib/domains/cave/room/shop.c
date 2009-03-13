#include <lib.h>
inherit LIB_SHOP;

int read_sign();
static void create() {
    ::create();
    SetClimate("indoors");
    SetAmbientLight(30);
    SetShort("The General Store");
    SetLong("This is the local general store. You can buy or sell items here. A sign on the wall hangs here, explaining how things work. Try 'read sign'. East Village Road is south of here. The storeroom is below.");
    SetItems(([
                ({"store","shop","place"}) : "This is the local store.",
                ({ "sign" ,"sign on the wall" }): "A sign on the wall, meant for reading.",
                ]));
    SetExits( ([
                "south" : "/domains/cave/room/cavepass9",
                "down" : "/domains/cave/room/shop2.c",
                ]) );
    SetInventory(([
                "/domains/cave/npc/gorm" : 1,
                "/domains/cave/obj/bbucket" : 1,
                ]));
    SetRead("sign", (: read_sign :) );
}

int read_sign(){
    write("HOW TO CONDUCT BUSINESS HERE\n");
    write("list : Get a list of all the items Otik has for sale");
    write("appraise <item> : Ask Otik to tell you how much he would pay you for your item");
    write("price <item> : Ask Otik the price of his item");
    write("show <item> : Ask Otik for a closer look at his item\n");
    write("\nbuy <item> from otik\nsell <item> to otik\n");
    write("Cash only!");
    return 1;
}
void init(){
    ::init();
}
