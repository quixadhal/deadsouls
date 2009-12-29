//Written by Lassondra who thinks Descartes is a weenie

inherit "/lib/std/room";

void create() {
    ::create();
    SetProperties( ([ "light" : 1, "no castle" : 1 ]) );
    SetShort( "the Nightmare Inn" );
    SetLong( "The hallway is very quiet, you can barely hear the whisper "
            "of your feet against the rich carpet. Large wooden doors for each room "
            "stand both to your left and right, trimmed with gold. Small lanterns "
            "hang every 5 feet or so, giving off a gentle glow, lighting the way." );
    SetItems( ([ "hallway" : "The hallway is very quiet.",
                "carpet" : "It is a deep burgundy colour.",
                ({ "lanterns", "lanter" }) :
                "The lanterns are gold with glass panes.",
                "doors" : "The doors are large oaken barriers.",
                ]) );
    SetExits( ([ "east" : "/domains/Praxis/inn109",
                "west" : "/domains/Praxis/inn110",
                "south" : "/domains/Praxis/hall4"
                ]) );
    //set_pre_exit_functions( ({ "east", "west" }), ({ "room_109", "room_110" }) );
}

int room_109() {
    if(present("/domains/Praxis/hotel"->query_key_id(109),this_player()))
        return 1;
    write( "You cannot go in there!");
    return 0;
}

int room_110() {
    if(present("/domains/Praxis/hotel"->query_key_id(110),this_player()))
        return 1;
    write( "You cannot go in there!");
    return 0;
}
void init(){
    ::init();
}
