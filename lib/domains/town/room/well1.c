#include <lib.h>
inherit LIB_ROOM;
int humidity = 0;

string GetDryDesc(){
    string ret = "This is the bottom of the old town well. It is quite dark "+
        "and unpleasant down here. Years of disuse have provided a haven for "+
        "vermin, dirt, and dust here. A service door of some kind is set in "+
        "the west wall here.\n%^GREEN%^There is a lever set in the wall here."+
        "%^RESET%^";
    return ret;
}

string GetWetDesc(){
    string ret = "This is the bottom of the old town well. It is quite dark "+
        "and humid down here. A service door of some kind is set in "+
        "the west wall here.\n%^GREEN%^There is a lever set in the wall here."+
        "%^RESET%^";
    return ret;
}

string GetWellDesc(){
    string ret;
    if(humidity) ret = GetWetDesc();
    else ret = GetDryDesc();
    return ret;
}

static void create() {
    room::create();
    SetClimate("indoors");
    SetAmbientLight(0);
    SetShort("Town Well");
    SetLong( (: GetWellDesc :) );
    SetItems( ([
                ({ "bottom","well" }) : "Dirty, musty, "
                "and unpleasant.",
                "haven" : "A nice place for vermin.",
                ({"dirt","dust"}) : "There's plenty of that "
                "here. Empty wells rarely get much "
                "priority on cleaning day.",
                ({"vermin","rats","bugs"}) : "Looks "
                "like they're hiding from you at the "
                "moment."
                ]) );
    SetInventory(([
                "/domains/town/obj/well_lever" : 1,
                ]));
    SetExits( ([ 
                "up" : "/domains/town/room/south_road1",
                "west" : "/domains/town/room/well2",
                ]) );
    SetDoor("west", "/domains/town/doors/welldoor1.c");
    SetFlowLimit(1);
}

void init(){
    ::init();
}

int eventCompleteQuest(object ob){
    string *quests;
    quests = ob->GetQuests();
    if(ob && !ob->GetQuest("Town Well Quest")){
        ob->AddQuest("the Plumber","Town Well Quest");
        tell_player(ob, "%^BOLD%^%^RED%^You have solved the Town Well Quest.");
        tell_player(ob, "%^BOLD%^%^RED%^Congratulations!");
        tell_player(ob, "%^BOLD%^%^RED%^You are awarded 7 quest points and "+
                "1500 experience points.");
        ob->AddQuestPoints(7);
        ob->AddExperiencePoints(1500);
    }
    return 1;
}

int eventReceiveObject(object ob){
    if(base_name(ob) == LIB_FLOW && !humidity){
        object player;
        mixed wheel = objects((: base_name($1) ==
                    "/domains/town/obj/waterwheel" && clonep($1) :));
        if(sizeof(wheel)) wheel = wheel[0];
        if(objectp(wheel)) player = wheel->GetTurner();
        if(player) eventCompleteQuest(player);
        humidity = 1;
        SetItems(([]));
    }
    return ::eventReceiveObject(ob);
}
