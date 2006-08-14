inherit "std/room";

void create() {
    ::create();
    set_property("light", 3);
    set_property("night light", 1);
    set_short( "The wilderness outside of Praxis");
    set_long(
      "The vegetation in the area outside the village "
      "thickens into jungle as you head east.");
    set_items(
      (["jungle" : "A wilderness area full of outlaws and "
	"mysterious things.",
	"village" : "The adventurer's town of Praxis.",
	"vegetation" : "It gets thicker to the east."]) );
    set_exits( 
      (["east" : "/domains/Praxis/wild2",
	"west" : "/domains/Praxis/e_boc_la3"]) );
}

