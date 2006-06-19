#include <lib.h>
inherit LIB_ROOM;

static void create() {
    room::create();
    SetClimate("indoors");
    SetAmbientLight(40);
    SetShort("Corridor");
    SetLong("You are in an east-west corridor on the first "
      "floor of the Virtual Campus administrative building. "
      "North is the campus bookstore, and there is a conference "+
      "room south of here.");
    //"room south of here. It seems the bookstore is closed today."); 
    SetItems( (["corridor" : "This is a carpeted corridor, running east-west.",
	"floor" : "The carpet is purple, and somewhat institutional.",
	"carpet" : "The carpet is purple, and somewhat institutional."]) );
    SetExits( ([
	"south" : "/domains/campus/room/conf",
	"north" : "/domains/campus/room/bookstore",
	"west" : "/domains/campus/room/corridor2.c",
      ]) );
    SetProperty("no attack", 1);
}
int CanReceive(object ob){
    string str;
    str=file_name(environment(ob));
    if(ob->GetRace() == "rodent"){
	message("info","You are repelled by rodenticide.",ob);
	return 0;
    }
    return 1;
}
void init(){
    ::init();
}
