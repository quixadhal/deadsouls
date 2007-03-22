#include <lib.h>
inherit LIB_ROOM;

int push_it(string str){
    if(str=="wall" || str=="east wall"){
	write("The east wall opens and you fall through!\n");
	say(this_player()->GetName()+" suddenly falls and is gone!\n");
	this_player()->eventMoveLiving("/domains/campus/room/tunnel", "east through a secret exit", this_player()->GetName()+" tumbles in.");
	return 1;
    }
}
static void create() {
    room::create();
    SetClimate("indoors");
    SetAmbientLight(5);
    SetShort("the basement");
    SetLong("You are in the basement of the LPC University administrative building. It "+
      "is very dark here, and rather damp. The fluorescent lighting seems to have "+
      "completely failed. The west side "+
      "of the basement seems somewhat better lit. The east wall seems heavily "+
      "smudged...it's even grungier than the rest of this place.");
    SetItems(([
	"basement" : "This is a dark, creepy basement.",
	"building" : "You are in the basement of the LPC University administrative building.",
	({"wall","east wall"}) : "The wall seems to have a lot of smudged handprints "+
	"on it.",
	({"dark","darkness"})  : "There's a lot of it, all around.",
	({"light","lighting","fluorescent lighting"}) : "The lighting here has failed "+
	"completely.",
	({"prints","print","handprint","handprints"}) : "Smudgy handprints, as if someone "+
	"had been against the wall here.",
	"air" : "It feels heavy, making it hard to breathe. It's hard to imagine "+
	"who or what would enjoy spending time down here."]));
    SetExits( ([
	"west" : "/domains/campus/room/basement.c",
      ]) );
    SetProperty("no attack", 1);
}
void init(){
::init();
    add_action("push_it","push");
    add_action("push_it","search");
    add_action("push_it","touch");
    add_action("push_it","open");
}