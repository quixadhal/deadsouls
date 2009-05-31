#include <lib.h>
inherit LIB_ROOM;

static void create() {
    room::create();
    SetClimate("indoors");
    SetAmbientLight(30);
    SetShort("the Arena");
    SetLong("You are in a large room with blank cement walls. This room was built so Creators may test their armor, weapons and NPC's in combat. A large steel door is here, which can be used to prevent wimpy creatures from escaping.");
    SetItems( ([
                ({"wall","walls"}) : "The walls are smooth and cement.",
                ({"floor","ceiling"}) : "The floor and ceiling are, like the walls, made "+
                "of smooth cement." ]) );
    SetInventory(([
                "/domains/default/npc/fighter" : 1,
                "/domains/default/obj/locker" : 1,
                "/domains/default/npc/dummy" : 1,
                "/domains/default/obj/case" : 1,
                "/domains/default/obj/abox" : 1,
                "/domains/default/obj/javelin_bin" : 1,
                "/domains/default/obj/rack" : 1,
                ]));
    SetExits( ([ 
                "south" : "/domains/default/room/wiz_corr_east",
                ]) );
    SetPlayerKill(1);

    SetDoor("south", "/domains/default/doors/steel_door2.c");

}

int CanReceive(object sneak) {
    object *living_stack = get_livings(sneak);
    if(!living_stack || !arrayp(living_stack)) living_stack = ({ sneak });
    foreach(object ob in living_stack){
        if(playerp(ob) && !creatorp(ob) &&
                !member_group(ob,"TEST")) {
            message("info","Creator staff only, sorry.", ob);
            return 0;
        }
    }
    return 1;
}

void init(){
    ::init();
}
