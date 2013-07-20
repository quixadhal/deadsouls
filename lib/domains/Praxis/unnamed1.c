#include <lib.h>

inherit LIB_ROOM;

void create() {
    ::create();
    SetProperty("light", 2);
    SetProperty("night light", 1);
    SetProperty("no castle", 1);
    SetShort( "Outside the financial district");
    SetLong(
      "Leading out east into the rural lands outside of Praxis, this unnamed "
      "dirt road sits almost entirely untravelled.  The business district "
      "of Praxis is west.");
    SetItems( 
      (["road" : "It is temporarily blocked going east."]) );
    SetSkyDomain("town");
    SetExits( ([ 
        "west" : "/domains/Praxis/s_centre3",
      ]) );
}
void init(){
    ::init();
}
