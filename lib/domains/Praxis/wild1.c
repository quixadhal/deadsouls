inherit "/lib/std/room";

void create() {
    ::create();
    SetProperty("light", 3);
    SetProperty("night light", 1);
    SetShort( "The wilderness outside of Praxis");
    SetLong(
      "The vegetation in the area outside the village "
      "thickens into jungle as you head east.");
    SetItems(
      (["jungle" : "A wilderness area full of outlaws and "
        "mysterious things.",
        "village" : "The adventurer's town of Praxis.",
        "vegetation" : "It gets thicker to the east."]) );
    SetSkyDomain("town");
    SetExits( 
      (["east" : "/domains/Praxis/wild2",
        "west" : "/domains/Praxis/e_boc_la3"]) );
}
void init(){
    ::init();
}
