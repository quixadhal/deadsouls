#include <lib.h>
inherit LIB_ROOM;

static void create() {
    room::create();
    SetClimate("indoors");
    SetAmbientLight(30);
    SetShort("The Magic Shop Storeroom");
    SetLong("This is the storeroom where magical items are kept."); 
    SetInventory(([
                "/domains/town/obj/scroll_raise_dead" : 5,
                "/domains/town/meals/potion_bigheal" : 2,
                "/domains/town/meals/potion_healing" : 10,
                "/domains/town/obj/8ball" : 3,
                "/domains/town/obj/d6" : 5,
                "/domains/default/armor/gray_amulet" : 2,
                "/domains/town/obj/omni" : 1,
                "/domains/town/meals/potion_strength" : 5,
                "/domains/town/obj/pipe" : 5,
                "/domains/town/obj/d20" : 5,
                ]));
    SetExits( ([
                "down" : "/domains/town/room/magic_shop.c",
                ]) );
}
int CanReceive(object sneak) {
    object *living_stack = get_livings(sneak);
    if(!living_stack || !arrayp(living_stack)) living_stack = ({ sneak });
    foreach(object ob in living_stack){
        if(living(ob) && !creatorp(ob) && 
                !member_group(ob,"TEST")) {
            message("info","Oana's back room area is for authorized personnel only.", ob);
            return 0;
        }
    }
    return 1;
}

void init(){
    ::init();
}
