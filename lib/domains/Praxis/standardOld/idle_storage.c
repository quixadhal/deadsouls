inherit "std/room";

void create() {
    ::create();
    set_short( "Adventurer's Supply Storage Room");
    set_long( "Horace stores weapons and such here.\n");
    set_exits( (["south" : "/domains/Praxis/supply"]) );
}

