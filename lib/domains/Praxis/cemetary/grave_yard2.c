#include <lib.h>
inherit LIB_ROOM;

void create() {
    ::create();
    SetProperty("light", 1);
    SetShort( "Entrance to graveyard");
    SetLong(
      "You have entered an old, disreputable cemetary.  There are a few "
      "new graves, but most are very ancient and untended.  You see a "
      "cracked marble headstone on the grave here.");
    SetItems(
      (["depths" : "You look down into Melissa's grave and see "
	"a swirling white haze.",
	"haze" : "The haze moves like it is alive, and takes "
	"on what looks like a female form at times.",
	"haziness" : "The haziness swirls constantly and covers the grave.",
	"depression" : "The depression is caused by the soil over "
	"the grave sinking.  There is a haze covering it.",
	"headstone" : "An ancient headstone, perhaps the first.  "
	"Its writing is illegible except for the name Melissa Praxis.",
	"graves" : "Most graves here are overgrown and untended. "
	"You stand next to a very old and shabby, sunken grave.",
	"cemetary" : "The residents of Praxis get buried there. "
	"It looks spooky.",
	"grave" : "You look down at Melissa's grave.  "
	"You see an odd haziness in the depths of the depression "
	"where the grave has sunken over the years."]) );
    SetExits( 
      (["west" : "/domains/Praxis/cemetary/grave_yard",
	"northeast" : "/domains/Praxis/cemetary/grave_yard6",
	"east" : "/domains/Praxis/cemetary/grave_yard5",
	"southeast" : "/domains/Praxis/cemetary/grave_yard4",
	"south" : "/domains/Praxis/cemetary/grave_yard3",
	"down" : "/domains/Praxis/cemetary/grave_yard7",
	"north"	 : "/domains/Praxis/cemetary/grave_yard8"]) );
}

