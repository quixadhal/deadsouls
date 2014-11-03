inherit "/lib/std/room";

void create() {
    ::create();
    SetProperty("light", 1);
    SetShort( "Deep in the rain forest");
    SetLong("You are deep inside a tropical rain forest. There is a shack east of here.");
    SetItems(
            ([ "forest" : "A very humid jungle away from civilization."]) );
    SetSkyDomain("town");
    SetExits( ([
                "southwest" : "/domains/Praxis/jungle",
                "east" : "/domains/Praxis/rogue_join.c",
                ]) );
}
void init(){
    ::init();
}
