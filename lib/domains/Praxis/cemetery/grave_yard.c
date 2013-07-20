#include <lib.h>

inherit LIB_ROOM;

void create() {
    ::create();
    SetProperty("light", 2);
    SetExits( 
      (["west" : "/domains/Praxis/east_road2",
        "east" : "/domains/Praxis/cemetery/grave_yard2"]) );
    SetShort("Cemetary Entrance");
    SetLong(
      "You stand between the gates of an ancient, nearly "
      "forgotten cemetery. The air is very still here, and "
      "preternaturally quiet.  You feel uneasy and look over "
      "your shoulder.  No one comes here anymore for some reason. "
      "You see a headstone off to the east. ");
    SetItems(
      (["headstone" : "The headstone is too far away to read.",
        "gates" : "The gates are antiques.  No one makes gates "
        "like these anymore."]) );
    SetSkyDomain("town");
}
void init(){
    ::init();
} 
