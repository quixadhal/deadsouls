#include <lib.h>

inherit LIB_ROOM;

void create() {
    ::create();
    SetProperties( ([ "light" : 2, "indoors" : 1, "no attack" : 1, "no steal" : 1 ]) );
    SetExits( 
            (["south" : "/domains/Praxis/monastery"]) );
    SetShort("The chapel");
    SetLong( 
            sprintf("The %s chapel is solemn and ancient with stained glass "
                "windows and dark oak pews.  Most wedding ceremonies "
                "are performed here.  Type <help marriage> for "
                "information on weddings.", mud_name()) );
    SetItems(
            (["pews" : "Ancient oak seats, darkened by eons of "
             "bottoms resting on them.",
             "windows" : "The windows show highlights of "
             "mud history, many of them gorey.",
             "ceremony" : "The ceremony is only visible when you "
             "scroll back."]) );
}
void init(){
    ::init();
}
