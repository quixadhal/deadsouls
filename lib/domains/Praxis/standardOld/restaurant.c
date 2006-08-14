#include <std.h>

inherit ROOM;

void init() {
    ::init();
    add_action("read", "read");
}

void create() {
    ::create();
    set_property("light", 3);
    set_property("indoors", 1);
    set_property("no castle", 1);
    set_short( "Last Dragon Restaurant");
    set_long(
      "Welcome to the Last Dragon Restaurant!\n"
      "Here the people of Praxis and visiting adventurers come for "
      "good food.  When the waitress is around, she will happily "
      "serve you anything on the menu which is displayed on the "
      "far wall.  Sun Alley is out to the north.");
    set_items(
      (["restaurant" : "The Last Dragon Restaurant, home of "
	"great Praxian cuisine.",
	"wall" : "The menu is posted there.",
	"menu" : "You may read it if you like."]) );
    set_exits( 
      (["north" : "/domains/Praxis/sun2"]) );
}

void reset() {
    ::reset();
    if(!present("waitress")) 
	new("/domains/Praxis/obj/mon/waitress")->move(this_object());
}

int read(string str) {
    object ob;
    int i;

    ob = present("waitress");
    if(!ob) {
	write("You cannot read the menu, as it is splattered with blood.");
	return 1;
    }
    write("The following great foods are served here at the Last Dragon Restaurant.");
    write("--------------------------------------------------------------------");
    write("Praxian lobster\t\t\t"+ (int)ob->get_price("lobster") + " gold");
    write("A plate of sea worms\t\t"+(int)ob->get_price("worms")+" gold");
    write("A serving of fish		"+(int)ob->get_price("fish")+" gold");
    write("Some fries\t\t\t"+ (int)ob->get_price("fries")+" gold");
    write("-----------------------------------------------------------");
    write("<buy dish_name> gets you the food.");
    return 1;
}

