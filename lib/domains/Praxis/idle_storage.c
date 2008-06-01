inherit "/lib/std/room";

void create() {
    ::create();
    SetShort( "Adventurer's Supply Storage Room");
    SetLong( "Horace stores weapons and such here.\n");
    SetExits( (["south" : "/domains/Praxis/supply"]) );
}
void init(){
    ::init();
}
