#include <lib.h>

inherit LIB_ROOM;

void init() {
    ::init();
    add_action("read", "read");
}

void create() {
    ::create();
    SetProperties( ([ "light": 3, "indoors" : 1, "no castle" : 1 ]) );
    SetShort( "Last Dragon Restaurant");
    SetLong(
            "Welcome to the Last Dragon Restaurant!\n"
            "The Last Dragon Restaurant is home to the finest cusine "
            "in Praxis! The atmosphere is lively and friendly, as is the service."
            " The restaurant itself is an oaken structure, well polished and "
            "maintained. A menu is displayed on the wall, and the waitress "
            "will be happy to serve you when she is arround. South of here "
            "runs Sun Alley." );
    SetItems(
            (["restaurant" : "The Last Dragon Restaurant, home of "
             "great Praxian cuisine.",
             "wall" : "The menu is posted there.",
             "menu" : "You may read it if you like."]) );
    SetExits( 
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
    write("Praxian lobster\t\t\t"+ ob->get_price("lobster") + " gold");
    write("A plate of sea worms\t\t"+ob->get_price("worms")+" gold");
    write("A serving of fish		"+ob->get_price("fish")+" gold");
    write("Some fries\t\t\t"+ ob->get_price("fries")+" gold");
    write("-----------------------------------------------------------");
    write("<buy dish_name> gets you the food.");
    return 1;
}

