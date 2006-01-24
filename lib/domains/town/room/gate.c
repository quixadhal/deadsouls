#include <lib.h>
inherit LIB_ROOM;

int PreExit(){
    if(this_player()->GetLevel() > 3 && !creatorp(this_player())){
	if(present("gate guard",this_object())){
	    present("gate guard",this_object())->eventForce("say You're too big to slip by me now. You're not going to the mansion any more.");
	}
	else {
	    tell_object(this_player(),"A reality distortion forms around you as you try to go south, and you wind up where you started.");
	}
	return 0;
    }
    if(present("gate guard",this_object())){
	tell_object(this_player(),"You are such a newbie that the gate guard doesn't even notice you slip by him.");
	tell_room(this_object(),this_player()->GetName()+" sneaks past the gate guard.",({ this_player() }) );
    }
    return 1;
}	



static void create() {
    room::create();
    SetClimate("outdoors");
    SetAmbientLight(30);
    SetShort("Mansion Gate");
    SetLong("You are standing just north of the gate to a large, "+
      "beautiful mansion, which stands to the south. The "+
      "Corinthian capitals on the front pillars bespeak "+
      "of the wealth and importance of the person who "+
      "lives here. Lush ivy wraps around the gate and the "+
      "brick wall surrounding the estate.");
    SetItems( ([
	"gate" : "A handsome, wrought-iron entry control point.",
	({"estate", "mansion"}) : "The grounds of a mansion are to the south.",
	({"capital","capitals","corinthian capital","corinthian capitals"}) :
	"These are the headpieces of the pillars supporting the "
	"mansion's front overhang. The beautifully detailed "
	"carvings of encanthus leaves distinguish them as "
	"Corinthian.",
	({"overhang","front overhang"}) : "A fancy and unnecessary "
	"structure in front of the mansion supported by "
	"columns.",
	({"pillar","pillars","column","columns"}) : "Load-bearing "
	"structures supporting the mansion's front overhang.",
	({"ivy","lush ivy"}) : "Vines of the ivy plant run "
	"over and along the walls and gate, so thickly that "
	"they nearly obscure them.",
	({"wall","walls"}) : "Architectural features which prevent "
	"casual entry. They are made of brick and appear old "
	"and strong.",
      ]) );
    SetExits( ([
	"north" : "/domains/town/room/road1",
      ]) );
    AddExit("south", "/domains/town/room/mansion_ext", (: PreExit :));
    AddStuff( ({
	"/domains/town/npc/mp" 
      }) );

    SetProperty("no attack", 1);
}
