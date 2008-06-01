inherit "/lib/std/room";

void create() {
    ::create();
    SetShort( "The cache");
    SetLong( "Things are hidden here.");
    SetProperty("storage room", 1);
}
void init(){
    ::init();
}
