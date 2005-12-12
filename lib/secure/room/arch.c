#include <lib.h>
inherit LIB_ROOM;

static void create() {
    object ob;
    room::create();
    SetClimate("indoors");
    SetAmbientLight(30);
    SetShort("Arch Room");
    SetLong("This is a polished, antiseptic room composed of some "
      "white, gleaming material. There is a viewscreen on a wall here, "
      "with a control panel alonside it. A camera is mounted in the wall "
      "on the other side of the screen. There is a shimmering portal "
      "on the north wall.");
    SetItems( ([ ({"wall","walls"}) : "The walls seem composed "
	"of some advanced polymer. They are extremely clean and highly "
	"polished.",
	"room" : "This looks like it might be the control room "
	"for the mud.",
	({"screen","viewscreen"}) : "This is a display screen of some sort.",
	({"panel","control panel"}): "This seems to be the main control "
	"panel for the mud. It contains a bewildering array of "
	"keypads, but the most prominent feature of the control panel "
	"is a metallic plate in its center, shaped in the form of a "
	"human hand.",
	({"camera","hal","HAL"}) : "This is the rectangular faceplate of "
	"a camera mounted within the wall. On the upper end of the "
	"rectangle is a stamped label. On the lower end is the lens, "
	"which has at its center a glowing red light.",
	({"label","stamped label"}) : "A stamped metal label. The label "
	"reads: 'HAL 9000'.",
	({"plate","metallic plate","identification plate"}) : "This "
	"appears to be an identification plate of some sort, designed "
	"to accomodate a human hand.",
	"portal" : "A portal to another place." ]) );
    SetListen("default", "You can faintly hear a low hum coming from the walls.");
    SetListen( ({"wall","walls"}), "You hear a low throbbing sound, as if from machinery.");
    SetExits( ([ "north" : "/domains/default/room/wiz_hall" ]) );
    AddEnter("portal" , "/domains/default/room/wiz_hall");
    SetObviousExits("n");
    ob = new("/lib/bboard");
    ob->SetKeyName("chalkboard");
    ob->SetId( ({ "board", "chalkboard" }));
    ob->set_board_id("admin_board");
    ob->set_max_posts(30);
    ob->SetShort("The Arch Board");
    ob->SetLong("This is the Arch board. You know how to use it.");
    ob->eventMove(this_object());
}
int CanReceive(object ob) {
    if( living(ob) && !archp(ob)  ){
	message("info","The arch room is available only to "+
	  "admins, sorry.",ob);
	return 0;
    }
    return 1;
}
void init(){
    ::init();
}

