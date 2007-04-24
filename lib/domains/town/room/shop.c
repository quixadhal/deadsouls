#include <lib.h>
inherit LIB_SHOP;

int read_sign();
static void create() {
    ::create();
    SetClimate("indoors");
    SetAmbientLight(30);
    SetShort("The General Store");
    SetLong(" You are in the local general store. You can "
      "buy or sell items here. A sign on the wall hangs here, "
      "explaining how things work. Try \"read sign\". East "
      "Village Road is south of here. The back storeroom is north.");
    SetItems(([
	({"store","shop","place"}) : "This is the local store.",
	({ "sign" ,"sign on the wall" }): "A sign on the wall, meant for reading.",
      ]));
    SetInventory(([
	"/domains/town/obj/bbucket" : 1,
	"/domains/town/npc/otik" : 1,
      ]));
    SetRead("sign", (: read_sign :) );
    SetExits( ([
	"south" : "/domains/town/room/vill_road2",
	"north" : "/domains/town/room/shop2",
      ]) );
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
