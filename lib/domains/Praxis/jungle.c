inherit "/lib/std/room";

void create() {
    ::create();
    SetProperty("light", 2);
    SetShort( "The jungle");
    SetLong(
            "The jungle is ever increasing in humidity. "
            "The vegetation that imposes itself upon you from "
            "every direction becomes even thicker to the north and east. "
            "It looks a bit tamer as you look south and west.");
    SetItems(
            (["jungle" : "A growing rain forest full of green vegetation.",
             "vegetation" : "All sorts of plant life growing as you have "
             "never before seen."]) );
    SetSkyDomain("town");
    SetExits( 
            (["northeast" : "/domains/Praxis/rain_forest",
             "southwest" : "/domains/Praxis/wild2"]) );
}
void init(){
    ::init();
}
