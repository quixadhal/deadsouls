inherit "std/room";

void create() {
    ::create();
    set_short( "The cache");
    set_long( "Things are hidden here.");
    set_property("storage room", 1);
}
