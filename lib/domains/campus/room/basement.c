#include <lib.h>
inherit LIB_ROOM;

static void create() {
    room::create();
    SetClimate("indoors");
    SetAmbientLight(29);
    SetShort("the basement");
    SetLong("You are in the basement of the LPC University administrative building. It "+
            "is very dark here, and rather damp. The fluorescent lighting seems to be "+
            "on the verge of total failure, with only one or two flickering on and off enough "+
            "to see anything. The air here is damp, thick and musty. There is a small crawlspace you can enter under "+
            "the stairs here, and the basement continues west into darkness.");
    SetItems(([
                "stairs" : "A flight of stairs leading up. There is a crawlspace beneath them.",
                "basement" : "This is a dark, creepy basement.",
                "building" : "You are in the basement of the LPC University administrative building.",
                ({"dark","darkness"})  : "There's a lot of it, all around.",
                ({"light","lighting","fluorescent lighting"}) : "The lighting here is extremely bad. "+
                "The lights show just enough to let you know you'd rather not spend too "+
                "much time in here.",
                "air" : "It feels heavy, making it hard to breathe. It's hard to imagine "+
                "who or what would enjoy spending time down here.",
                "crawlspace" : "It looks like you might be able to enter the crawlspace, if you "+
                "really wanted to."]));
    SetInventory((["/domains/campus/npc/rat" : 1]));
    AddExit("up","/domains/campus/room/stairwell");
    AddExit("west","/domains/campus/room/basement2");
    AddEnter("crawlspace" , "/domains/campus/room/crawlspace");
    SetProperty("no attack", 1);
}

void init(){
    ::init();
}
