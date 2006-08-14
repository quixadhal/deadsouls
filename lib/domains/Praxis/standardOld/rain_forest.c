inherit "std/room";

void create() {
    ::create();
    set_property("light", 1);
    set_short( "Deep in the rain forest");
    set_long(
      "You are deep inside a tropical rain forest.");
    set_items(
      ([ "forest" : "A very humid jungle away from civilization."]) );
    set_exits( 
      (["southwest" : "/domains/Praxis/jungle",
	"east" : "/domains/Jungle/jungle1"]) );
}

