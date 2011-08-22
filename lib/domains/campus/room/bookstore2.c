#include <lib.h>
inherit LIB_ROOM;

static void create() {
    room::create();
    SetAmbientLight(30);
    SetClimate("indoors");
    SetShort("a generic room");
    SetLong("This is an utterly plain, blank room.");
    SetInventory(([
                "/domains/campus/obj/diamond" : 1,
                "/domains/default/obj/phints" : 5,
                "/domains/default/obj/handbook" : 5,
                "/domains/campus/obj/pack" : 30,
                "/domains/campus/obj/bag" : 3,
                "/domains/default/obj/manual" : 5,
                "/domains/default/obj/guide" : 5,
                "/domains/campus/armor/coat" : 1,
                ]));
    SetExits( ([
                "down" : "/domains/campus/room/bookstore.c",
                ]) );
}
int CanReceive(object sneak) {
    object *living_stack = get_livings(sneak);
    if(!living_stack || !arrayp(living_stack)) living_stack = ({ sneak });
    foreach(object ob in living_stack){
        if(living(ob) && !creatorp(ob) &&
                !member_group(ob,"TEST")) {
            message("info","The storeroom is for authorized personnel only.", ob);
            return 0;
        }
    }
    return ::CanReceive(sneak);
}
void init(){
    ::init();
}
