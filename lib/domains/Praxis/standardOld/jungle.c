inherit "std/room";


void create() {
    ::create();
    set_property("light", 2);
    set_short( "The jungle");
    set_long(
      "The jungle is ever increasing in humidity. "
      "The vegetation that imposes itself upon you from "
      "every direction becomes even thicker to the north and east. "
      "It looks a bit tamer as you look south and west.");
    set_items(
      (["jungle" : "A growing rain forest full of green vegetation.",
	"vegetation" : "All sorts of plant life growing as you have "
	"never before seen."]) );
    set_exits( 
      (["northeast" : "/domains/Praxis/rain_forest",
	"southwest" : "/domains/Praxis/wild2"]) );
}

