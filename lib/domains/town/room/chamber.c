#include <lib.h>
inherit LIB_ROOM;

static void create() {
    room::create();
    SetClimate("indoors");
    SetAmbientLight(30);
    SetShort("The Healing Chamber");
    SetLong("This is the work chamber of Clepius the healer. "
            "This is where he performs medical procedures on "
            "his customers. A desk in the corner overflows with "
            "notes and books. The reception and waiting area is "
            "east of here.");
    SetItems( ([
                "desk" : "This is where Clepius does "
                "paperwork and research.",
                ({"notes","books","notes and books"}) : "These "
                "are the contents of the doctor's desk.",
                "corner" : "Location of the desk."
                ]) );
    SetExits( ([ 
                "east" : "/domains/town/room/healer",
                ]) );
    SetRead( ({"notes","books","notes and books"}) , "That belongs to Clepius and it's private." ); 
    SetProperty("no attack", 1);
    SetInventory(([
                "/domains/town/obj/bbucket" :1,
                "/domains/town/npc/clepius" : ({60, 1})
                ]) );
    SetProperty("busy",0);

}
int CanReceive(object ob) {
    if(playerp(ob) && !creatorp(ob) && GetProperty("busy") == 1){
        message("info","The doctor is with a patient right now. Please "+
                "wait until you are called.",ob);
        return 0;
    }
    return 1;
}
void init(){
    ::init();
}
