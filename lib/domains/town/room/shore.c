#include <lib.h>
#include <daemons.h>

inherit LIB_FISHING;

int ActionFunction(){
    object *cres, dude;
    if(!sizeof(get_livings(this_object()))) return 0;
    //The next line identifies creators in the room
    cres = filter(get_livings(this_object()), (: creatorp($1) :) );
    //The next line picks a living being here at random
    dude = get_random_living(this_object());
    if(!dude) return 0;
    if((RACES_D->GetNonMeatRace(dude->GetRace()))) return 0;
    //The next line makes that person sigh. Note that since
    //creators normally can't be forced by non-privileged objects,
    //only npc's and players will be affected.
    dude->eventForce("sigh");
    //The next line tells any creators present the identitiy of the
    //target of the previous action.
    if(!sizeof(cres)) return 0;
    //The action below is commented out because it confused people.
    //basically it tells creators about this function.
    //cres->eventPrint("%^YELLOW%^The ActionFunction activates. Dude is: "+
    //  identify(dude)+".%^RESET%^");
    return 1;
}

int ActionFunction2(){
    object *cres, dude;
    if(!sizeof(get_livings(this_object()))) return 0;
    cres = filter(get_livings(this_object()), (: creatorp($1) :) );
    dude = get_random_living(this_object());
    if(!dude) return 0;
    if((RACES_D->GetNonMeatRace(dude->GetRace()))) return 0;
    dude->eventForce("ponder");
    if(!sizeof(cres)) return 0;
    //The action below is commented out because it confused people.
    //basically it tells creators about this function.
    //cres->eventPrint("%^GREEN%^ActionFunction2 activates. Dude is: "+
    //  identify(dude)+".%^RESET%^");
    return 1;
}

static void create() {
    fishing::create();
    SetClimate("outdoors");
    SetAmbientLight(30);
    SetShort("Shore Edge");
    SetNightLong("You are on the shore, at the far eastern end of town. A road travels west into the heart of town. A great sea stretches out into the eastern horizon. A lone lamp post lights this area.");
    SetDayLong("You are on the shore, at the far eastern end of town. A road travels west into the heart of town. A great sea stretches out into the eastern horizon.");
    SetExits( ([
                "west" : "/domains/town/room/vill_road4.c",
                "east" : "/domains/town/virtual/surface/33,100000",
                ]) );
    //SetFrequency determines how often the room checks 
    //whether it should perform an action. If it is set
    //at 1, it checks approximately every second. The
    //default is 5.
    SetFrequency(5);
    //The first argument of SetAction is a number, indicating from
    //one to 100 the percentage of likelihood an event will occur.
    //The second argument is an array that contains strings,
    //functions, or both. Every time an action is triggered, an
    //element is picked from the array at random. If it is a string,
    //that string is displayed to the living things in the room.
    //If it is a function, that function is evaluated.
    SetAction(2, ({"A cool breeze flows in from the east, "
                "bringing with it the bracing salty smells of the sea.",
                (: ActionFunction :) }) );
    SetChance(90);
    SetFish( ([
                "/domains/town/meals/shark" : 1,
                "/domains/town/meals/herring" : 10,
                ]) );
    //This is a function that allows for a set of actions
    //with different trigger frequencies.
    SetActionsMap( ([ 
                "A soft breeze cools your brow." : 5,
                "You briefly hear seagulls far in the distance." : 7,
                (: ActionFunction2 :) : 2,
                ]) );
    SetInventory( ([
                "/domains/town/obj/seawater" : 1,
                ]) );
    SetFlyRoom("/domains/town/virtual/sky/32,100000,1");
    AddItem(new("/domains/town/obj/lamp"));
}
void init(){
    ::init();
}

mixed CanReceive(object ob){
    if(ob && ob->GetRace() == "drone"){
        message("info","You are repelled by drone repellent.",ob);
        return 0;
    }
    return 1;
}

