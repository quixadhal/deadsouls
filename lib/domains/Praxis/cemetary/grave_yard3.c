#include <lib.h>
inherit LIB_ROOM;

void create() {
    ::create();
    SetProperty("light", 1);
    SetShort( "Darkone's grave");
    SetLong(
      "You are in the southern-most area of the cemetary.  You hear "
      "no friendly sounds, only an ominous silence.  There is a "
      "solitary grave here, with a small headstone at the head of it.");
    SetItems(
      (["headstone" : "The headstone is a deep cobalt blue, and "
	"has writing on it.",
	"cemetary" : "All about you stretch the silent graves.  "
	"You feel like you are being watched.",
	"grave" : "The grave is covered with soft grass, and has "
	"been here a long time",
	"writing" : "Here lies Darkone.  He was lynched, suitably "
	"enough, by an irate mob of young immortals for nit-picking "
	"in their castles."]) );
    SetExits( 
      (["north" : "/domains/Praxis/cemetary/grave_yard2",
	"northeast" : "/domains/Praxis/cemetary/grave_yard4"]) );
}

void reset() {
    ::reset();
    if (!present("ghost"))
	new("/domains/Praxis/cemetary/mon/ghost3")->move(this_object());
}

void init() {
    ::init();
    add_action("read","read");
}

int read(string str) {
    if (str == "writing")  {
	write (::GetLong("writing"));
	return 1;
    }
    notify_fail("Read what?\n");
    return 0;
}

