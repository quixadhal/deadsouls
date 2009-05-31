inherit "/lib/std/room";

void create() {
    int i;
    ::create();
    SetShort( "Adventurer's Supply Storage Room");
    SetLong( "Horace stores weapons and such here.");
    SetExits( 
            (["south" : "/domains/Praxis/supply"]) );
    new("/domains/Praxis/obj/misc/vial")->move(this_object());
    //for( i=0 ; i<5 ; i++ )
    //new("/realms/nialson/newspaper/paper")->move(this_object());
}
void init(){
    ::init();
}
