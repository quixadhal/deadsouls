inherit "/lib/std/room";

void create() {
    ::create();
    SetProperty("light", 0);
    SetShort( "A hole in the ground");
    SetLong(
            "You are in a deep hole in the ground. The light is very poor "
            "down here.");
    SetNoClean(1);
    SetExits( ([ ]) );
}

void fixing_a_hole() {
    RemoveItem("hole");
    RemoveExit("up");
}

void digging() {
    AddItem("hole", "A hole up to the surface.");   AddExit( "up", "/domains/Praxis/west_road2");

}

void reset() {
    object money;
    ::reset();
    if(!present("match"))
        new("/domains/Praxis/obj/misc/match")->move(this_object());
    if(!present("money")) {
        money = new("/lib/pile");
        money->SetCurrency("platinum", random(5));
        money->SetCurrency("silver", random(100));
        money->SetCurrency("copper", random(1000));
        money->move(this_object());
    }
}
void init(){
    ::init();
}
