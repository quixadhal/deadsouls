#include <lib.h>
inherit LIB_ROOM;

static void create() {
    room::create();
    SetClimate("indoors");
    SetAmbientLight(30);
    SetShort("Mansion Guest Room East");
    SetLong("You are in the east upstairs guest room of the mansion. "
      "The room is sumptuously appointed and furnished. "
      "The state of the bed suggests that someone has "
      "been staying here recently.");
    SetItems( ([
        ({"furniture","furnishings"}) : "Comfortable and attractive."
      ]) );
    SetExits( ([
        "west" : "/domains/town/room/mansion_uhall1"
      ]) );
    SetInventory(([
        "/domains/town/obj/obag" :1,
        "/domains/town/obj/gbed" :1,
        "/domains/town/armor/boot_l" :1,
        "/domains/town/armor/boot_r" :1,
      ]) );

    SetDoor("west", "/domains/town/doors/m1.c");

}
void init(){
    ::init();
}
