#include <lib.h>
inherit LIB_ROOM;

static void create() {
    room::create();
    SetClimate("indoors");
    SetAmbientLight(30);
    SetShort("Gorm's storage room");
    SetLong("This is a small, bare room where Gorm keeps his goods. The main store is above.");
    SetInventory(([
                "/domains/cave/obj/skin" : 5,
                "/domains/cave/obj/match" : 5,
                "/domains/cave/obj/torch" : 5,
                "/domains/cave/weap/axe" : 5,
                "/domains/cave/weap/dagger" : 5,
                "/domains/cave/weap/javelin" : 5,
                "/domains/cave/weap/sword" : 5,
                "/domains/cave/armor/armored_boot_l" : 2,
                "/domains/cave/armor/armored_boot_r" : 2,
                "/domains/cave/armor/armored_glove_r" : 2,
                "/domains/cave/armor/armored_glove_l" : 2,
                "/domains/cave/armor/chainmail" : 5,
                "/domains/cave/armor/leather_armor" : 5,
                "/domains/cave/armor/orc_boot_r" : 5,
                "/domains/cave/armor/orc_boot_l" : 5,
                "/domains/cave/armor/orc_tallboot_r" : 5,
                "/domains/cave/armor/orc_tallboot_l" : 5,
                "/domains/cave/armor/orc_helmet" : 5,
                "/domains/cave/armor/platemail" : 1,
                "/domains/cave/armor/pants" : 10,
                "/domains/cave/armor/orc_dress" : 10,
                ]));
    SetExits( ([ 
                "up" : "/domains/cave/room/shop",
                ]) );
    SetNoClean(1);

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
