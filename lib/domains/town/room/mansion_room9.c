#include <lib.h>
inherit LIB_ROOM;

int revealed;
int PreExit(){
    object ob = present("thief in a bathtowel",this_object());
    if(ob && base_name(ob) != "/lib/std/corpse"){
        write("The wet thief bars your way!");
        return 0;
    }
    return 1;
}
static void create() {
    room::create();
    SetClimate("indoors");
    SetAmbientLight(30);
    SetShort("Sitting Room");
    SetLong("You are in a luxurious sitting room, decorated "
            "and furnished with the same excellent taste and "
            "attention to detail as the rest of the "
            "mansion.");
    SetItems( ([
                ({"furniture","furnishings","decorations"}) :
                "You see evidence of a refined aesthetic "
                "sensibility."
                ]) );
    SetExits( ([
                "west" : "/domains/town/room/mansion_int.c",
                ]) );
    SetInventory( ([
                "/domains/town/obj/rug" :1,
                "/domains/town/npc/thief" :1
                ]) );
    revealed = 0;
}
int TellRevealed(){
    if(revealed) return revealed;
    else return 0;
}
int RevealDoor(){
    if(revealed == 1) {
        tell_room(this_object(),"The trapdoor is already visible.");	
        return 1;
    }
    revealed = 1;

    tell_room(this_object(),"A trapdoor is revealed!");
    AddExit("down","/domains/town/room/mansion_room12", (: PreExit :));
    SetDoor("down","/domains/town/doors/trapdoor");
    return 1;
}
void init(){
    ::init();
}
