inherit "/lib/std/room";

void create() {
    ::create();
    SetProperty("light", 1);
    SetShort( "Descartes' grave");
    SetLong(
      "This is the cold and silent northeast corner of the ancient "
      "cemetary. You feel shivers go up your spine as you look at "
      "the tortured form of the statue mounted over this grave.  "
      "There are small heaps of nuts and bolts lying on the grave. "
      "You notice a plaque on the statue.");
    SetItems(
      (["cemetary" : "All about you stretch the silent graves.  "
	"You feel like you are being watched.",
	"grave" : "The grave is covered with small metal nuts and "
	"bolts and other metal tributes.",
	"nuts" : "The grave is covered with heaps of small pieces of metal.",
	"bolts" : "The grave is covered with small bits of metal, "
	"including nuts and bolts.",
	"statue" : "The statue is of a large cube, made of many "
	"metal bits stuck together.",
	"writing" : "Here lies the Borg, unit #82664302-tr, "
	"unmourned and unnoticed when it ceased to function.",
	"plaque" : "The plaque is of silver metal.  You might be "
	"able to read the engraving on it."]) );
    SetExits( 
      (["northwest" : "/domains/Praxis/cemetary/grave_yard8",
	"southwest" : "/domains/Praxis/cemetary/grave_yard2",
	"southeast" : "/domains/Praxis/cemetary/grave_yard5"]) );
}

void reset() {
    ::reset();
    if (!present("ghost"))
	new("/domains/Praxis/cemetary/mon/ghost6")->move(this_object());
}

void init() {
    ::init();
    add_action("read","read");
}

int read(string str) {
    if (str == "plaque") {
	write (::GetLong("writing"));
	return 1;
    }
    notify_fail("Read what?\n");
    return 0;
}

