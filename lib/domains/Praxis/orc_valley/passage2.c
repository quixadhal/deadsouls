inherit "/lib/std/room";

void create() {
    ::create();
    SetProperty("light", 1);
    SetProperty("indooors", 1);
    SetShort( "A dark passage");
    SetLong(
            "You are in a dark passage inside the orc fortress.");
    SetItems(
            (["passage" : "You cannot see much of anything."]) );
    SetExits( 
            (["west" : "/domains/Praxis/orc_valley/chamber2",
             "east" : "/domains/Praxis/orc_valley/open"]) );
}
void init(){
    ::init();
}
