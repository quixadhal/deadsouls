#include <lib.h>

inherit LIB_ROOM;

static void create() {
    object ob;
    room::create();
    SetClimate("indoors");
    SetAmbientLight(30);
    SetShort("Disease Lab Annex");
    SetLong("This room is designed for testing the effects and transmissibility of disease. The main testing area is west.");
    SetInventory(([
      ]));
    SetExits(([
	"west" : "/domains/default/room/quarantine2",
      ]));

    SetProperty("no attack", 1);

}
int CanReceive(object ob) {
    return room::CanReceive();
}
void init(){
    ::init();
}
