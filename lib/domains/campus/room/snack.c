#include <lib.h>
#include <dirs.h>
#include <message_class.h>
inherit LIB_ROOM;

int eventReadMenu(string str);
static void create() {
    room::create();
    SetClimate("indoors");
    SetAmbientLight(40);
    SetShort("The Campus Snack Bar");
    SetLong("You are in a small, linoleum-tiled room lit by bright "+
            "fluorescent lights. There are a few uncomfortable-looking chairs "+
            "around two tables, and a sort of bar set into the north wall, "+
            "behind which a food service employee stands. There is a menu "+
            "hanging on the wall next to the bar. The main hallway lies south of here.");
    SetItems(([
                "menu": "A menu of snack items available here.",
                "sign":"A sign over the snack bar.",
                ({"tile","tiles","floor","linoleum"}):"These are here because you "+
                "are not trusted to keep food off of carpets.",
                ({"chair","chairs","table","table"}):"Token furniture, not very functional "+
                "or comfortable-looking. You aren't really expected to hang "+
                "around in here, apparently.",
                "bar" : "Really a window set into the wall and connecting the snack bar "+
                "to the adjoining kitchen.",
                ({"wall","walls"}) : "The walls are painted blue and white, the school's colors. "+
                "It's fairly unattractive and institutional.",
                "hallway" : "The administration building's main hallway lies south.",
                ({"light","lights","fluorescent lights","fluorescents"}) : "Cheap, standard "+
                "lighting. Like all fluorescents, these lights give everything an unappealing, "+
                "sickly look.",
                "employee" : "An underpaid, unappreciated, and resentful state employee. "+
                "No surprise there.",
                ]));
    SetExits( ([
                "south" : "/domains/campus/room/corridor3.c",
                ]) );
    SetRead("menu", (: eventReadMenu :));
    SetInventory(([
                "/domains/campus/npc/gloria.c" : ({60, 1}) ]));
    SetProperty("no attack", 1);
}
int eventReadMenu(string str){
    write("\n"+
            "1) ham sandwich: 2 dollars\n"+
        "2) hamburger: 4 dollars\n"+
        "3) salad of the day: 3 dollars\n\n"+
        "Drinks:\n"+
        "1) milk: 1 dollar\n"+
        "2) gatorade: 2 dollars\n");
    return 1;
}
void init(){
    ::init();
}
