#include <lib.h>
#include ROOMS_H

inherit LIB_ROOM;
int ds;

string LongDesc(){
    string desc = "Immortals come here to communicate with each other about "+
        "the world they are building. The default start room is "+
        "north. The Arch Room is south."+
        (!(ds) ? " To visit the Dead Souls "+
         "test and development mud, visit the upstairs annex." : "")+
        " The test lab facilities are east.";
    desc += "\nA sign reads: "+load_object(ROOM_ARCH)->SignRead();
    return desc;
}

static void create() {
    object ob;
    room::create();
    if(mud_name() == "Dead Souls"){
        ds = 1;
    }
    SetClimate("indoors");
    SetAmbientLight(30);
    SetShort("Creators' Hall");
    SetLong( (: LongDesc :) );
    SetProperty("no attack", 1);
    SetProperty("nopeer",1);
    ob = new("/lib/bboard");
    ob->SetKeyName("chalkboard");
    ob->SetId( ({ "board", "chalkboard", "dusty board", "dusty chalkboard" }) );
    ob->set_board_id("immortal_board");
    ob->set_max_posts(30);
    ob->SetShort("a dusty chalkboard");
    ob->eventMove(this_object());
    SetInventory(([
                "/domains/default/npc/tree" : ({ 10, 1 }),
                "/domains/default/obj/chest" : 1 ]));
    SetItems( ([
                ({"sign"}) : "A sign you can read.",
                ]) );
    SetExits( ([
                "south" : "/secure/room/arch",
                "north" : "/domains/default/room/start",
                "east" : "/domains/default/room/wiz_corr1",
                "up" : "/domains/default/room/wiz_hall2.c",
                ]) );
    SetRead("sign", (: load_object(ROOM_ARCH)->SignRead() :) );
}

int CanReceive(object sneak) {
    object *living_stack = get_livings(sneak);
    if(!living_stack || !arrayp(living_stack)) living_stack = ({ sneak });
    foreach(object ob in living_stack){
        if(living(ob) && !creatorp(ob) && 
                base_name(ob) != "/domains/default/npc/tree" &&
                base_name(ob) != "/secure/npc/arch_wraith" &&
                base_name(ob) != "/domains/default/npc/drone3" &&
                base_name(ob) != "/secure/obj/floodmapper" &&
                !member_group(ob,"TEST")) {
            message("info","Creator staff only, sorry.", ob);
            return 0;
        }
    }
    return ::CanReceive(sneak);
}

int eventReceiveObject(object ob){
    string race = ob->GetRace();
    int ret = ::eventReceiveObject(ob);
    if(!ret) return 0;
    if(race && race == "orc"){
        ob->eventPrint("Welcome to our inclusive halls, proud orc!");
    }
    if(ob->GetInvis()){
        tell_room(this_object(), capitalize(ob->GetKeyName())+
                " enters invisibly.", ({ ob }) );
        ob->eventPrint("%^BOLD%^%^RED%^Your invisible entry has "+
                "been announced.%^RESET%^");
    }
    return ret;
}

int eventReleaseObject(object ob){
    int ret = ::eventReleaseObject(ob);
    if(!ret) return 0;
    if(ob->GetInvis()){
        tell_room(this_object(), capitalize(ob->GetKeyName())+
                " exits invisibly.", ({ ob }) );
        ob->eventPrint("%^BOLD%^%^RED%^Your invisible exit has "+
                "been announced.%^RESET%^");
    }
    return ret;
}

void init(){
    ::init();
}
