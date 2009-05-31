
inherit "/lib/std/room";

void create() {
    ::create();
    SetProperty("light", -1);
    SetProperty("indoors", 1);
    SetShort( "A disgusting sewer");
    SetLong(
            "These sewers under Praxis are filled with slime. "
           );
    SetItems(
            (["sewers" : "The disgusting tunnels of human waste under Praxis.",
             "slime" : "Nasty, nasty.",
             ({"wall", "east wall"}) : "A disgusting slime-coated wall.",
             "ceiling" : "A disgusting slime-coated ceiling.",
             ]));
    SetExits(
            (["west" : "/domains/Praxis/sewer_ent", 
             ]));
}
void init(){
    ::init();
}
