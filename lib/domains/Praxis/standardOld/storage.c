inherit "std/room";

void create() {
    int i;
    ::create();
    set_short( "Adventurer's Supply Storage Room");
    set_long( "Horace stores weapons and such here.");
    set_exits( 
      (["south" : "/domains/Praxis/supply"]) );
    new("/domains/Praxis/obj/misc/vial")->move(this_object());
    for( i=0 ; i<5 ; i++ )
	new("/realms/nialson/newspaper/paper")->move(this_object());
}

