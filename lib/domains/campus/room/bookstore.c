#include <lib.h>
inherit LIB_SHOP;
int read_sign();
static void create() {
    ::create();
    SetClimate("indoors");
    SetAmbientLight(30);
    SetShort("the campus bookstore");
    SetLong("This is the Virtual Campus Bookstore. It is a fairly small place, "+
      "and in fact has no books available, since the campus is not open for classes as of yet. "+
      "There is, however, a wide assortment of useful items available, which you can "+
      "browse by typing the \"list\" command. There is a register on the counter, and a "+
      "sign on the wall behind it.");
    SetItems(([
	({"store","bookstore"}) : "This is the Virtual Campus bookstore.",
	({"store","bookstore","place"}) : "This is the Virtual Campus bookstore.",
	"books" : "There aren't any yet. Weren't you paying attention?",
	({"assortment","items","stuff"}) : "To browse what you can buy, type: list.",
	"register" : "A device to facilitate the organized collection of your money. A bit "+
	"large and intimidating, actually.",
	"counter" : "A cheap wood-textured counter designed to make the surrender "+
	"of your money more convenient.",
	"sign" : "A sign on the wall, meant for reading.",
      ]));
    SetRead("sign", (: read_sign :) );
    SetExits((["south" : "/domains/campus/room/corridor4"
      ]));
    SetProperty("no attack", 1);
    SetInventory(([
	"/domains/campus/obj/bbucket" :1,
	"/domains/campus/npc/kim" :1
      ]) );
}

void init(){
    ::init();
    if(!present("bookstore employee",this_object())){
	new("/domains/campus/npc/kim")->eventMove(this_object());
    }
}
int read_sign(){
    //this_player()->eventPrint("Cash only, U.S. dollars. No exceptions!\n");
    write("HOW TO CONDUCT BUSINESS HERE\n");
    write("list : Get a list of all the items Kim has for sale");
    write("appraise <item> : Ask Kim to tell you how much she would pay you for your item");
    write("price <item> : Ask Kim the price of her item");
    write("show <item> : Ask Kim for a closer look at her item\n");
    write("\nbuy <item> from kim\nsell <item> to kim\n");
    write("Cash US Dollars only!");
    return 1;
}
