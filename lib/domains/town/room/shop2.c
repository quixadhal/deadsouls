#include <lib.h>
inherit LIB_ROOM;

static void create() {
    room::create();
    SetClimate("indoors");
    SetAmbientLight(30);
    SetShort("Otik's storage room");
    SetLong("This is a small, bare room where Otik keeps his goods. The main store is above.");
    SetInventory(([
                "/domains/town/weap/sword" : 30,
                "/domains/town/obj/rucksack" : 10,
                "/domains/town/obj/rayovac" : 10,
                "/domains/town/obj/watch" : 10,
                "/domains/town/armor/chainmail" : 3,
                "/domains/town/weap/verysharpsword" : 2,
                "/domains/town/obj/dcell_std" : 30,
                "/domains/town/obj/flask" : 10,
                "/domains/town/obj/match" : 15,
                "/domains/town/obj/torch" : 15,
                "/domains/town/armor/helmet" : 3,
                "/domains/town/weap/dagger" : 30,
                "/domains/town/obj/pack" : 10,
                "/domains/town/obj/ladder" : 2,
                "/domains/town/obj/maglite" : 3,
                "/domains/town/obj/canteen" : 5,
                ]));
    SetExits( ([ 
                "up" : "/domains/town/room/shop",
                ]) );
    SetNoClean(1);

    SetDoor("up", "/domains/town/doors/otik_d.c");

}
int CanReceive(object sneak) {
    object *living_stack = get_livings(sneak);
    if(!living_stack || !arrayp(living_stack)) living_stack = ({ sneak });
    foreach(object ob in living_stack){
        if(living(ob) && !creatorp(ob) &&
                !member_group(ob,"TEST")) {
            message("info","Otik's back room is for authorized personnel only.", ob); 
            return 0;
        }
    }
    return 1;
}
void init(){
    ::init();
}
