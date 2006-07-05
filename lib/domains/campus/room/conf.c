#include <lib.h>
inherit LIB_ROOM;

static void create() {
    room::create();
    SetClimate("indoors");
    SetAmbientLight(50);
    SetShort("Conference Room");
    SetLong("This is the Virtual Campus ad hoc conference room. It is "+
      "elegantly appointed with richly carved mahogany paneling and gold-leaf trim. "+
      "The luxurious carpeting and moody lighting make this conference room "+
      "a very comfortable, relaxing environment. The main hallway is north of here.");
    SetItems(([
	({"panels","paneling","wall","walls","mahogany","wood"}) : "The walls are "+
	"paneled in rich, dark mahogany. The wood is intricately carved with "+
	"exquisite designs. This sort of craftmanship must have cost a fortune.",
	({"room","conference room","ad hoc conference room"}) : "This is a "+
	"well-appointed, comfortable room to hold meetings in.",
	({"ad","hoc","ad hoc"}) : "Perhaps you should invest in a dictionary.",
	({"carving","carvings","design","designs","craftmanship"}) : "The carvings "+
	"are mostly abstract shapes that seem so detailed and tightly interwoven "+
	"they almost squirm before your eyes.",
	({"leaf","gold-leaf trim","trim","gold leaf trim"}) : "The trim along the walls' "+
	"baseboards and doorframe is a beautiful gold leaf design.",
	({"floor","carpet","carpeting","luxurious carpeting"}) : "The carpet is thick, and "+
	"a rich maroon color.",
	({"lighting","lights","moody lighting"}) : "The lighting here is indirect and diffused, "+
	"giving everything a warm glow.",
	"environment" : "Very comfortable indeed.",
      ]));
    SetExits( (["north" : "/domains/campus/room/corridor4",
      ]));
    SetObviousExits("n");
    SetInventory((["/domains/campus/obj/podium" : 1,
      ]));
    SetProperty("no attack", 1);
}
int CanReceive(object ob) {
    if(ob && living(ob)){
	if(file_name(environment(ob)) != "/domains/campus/room/corridor4"
	  && !archp(ob)){
	    message("info","You must enter the conference room through the normal "+
	      "entrance, like everyone else.",ob);
	    return 0;
	}
	return 1;
    }
    return 1;
}
void init(){
    ::init();
}
