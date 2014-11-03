#include <lib.h>
#include ROOMS_H

inherit LIB_ROOM;

static void create() {
    object ob;
    room::create();
    SetClimate("indoors");
    SetAmbientLight(30);
    SetShort("Builders' Lounge");
    SetLong("This is the comfortable lounge where builders can congregate to chat about their work and leave messages for each other on the board. The Adventurers' Guild is north.");
    SetProperty("no attack", 1);
    SetProperty("nopeer",1);
    ob = new("/lib/bboard");
    ob->SetKeyName("chalkboard");
    ob->SetId( ({ "board", "chalkboard", "dusty board", "dusty chalkboard" }) );
    ob->set_board_id("builder_board");
    ob->set_max_posts(30);
    ob->SetShort("a dusty chalkboard");
    ob->eventMove(this_object());
    SetItems( ([
                ({"sign"}) : "A sign you can read.",
                ]) );
    SetExits( ([ 
                "north" : "/domains/town/room/adv_guild",
                ]) );
    SetInventory(([
                "/domains/default/obj/couch" : 1,
                ]));
}

int CanReceive(object sneak) {
    object *living_stack = get_livings(sneak);
    if(!living_stack || !arrayp(living_stack)) living_stack = ({ sneak });
    foreach(object ob in living_stack){
        if(playerp(ob) && !builderp(ob) && !present("testchar badge",ob) &&
                !member_group(ob,"TEST")) {
            message("info","Staff only, sorry.", ob);
            return 0;
        }
    }
    return 1;
}

void init(){
    ::init();
}
