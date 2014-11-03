#include <lib.h>
#include <vendor_types.h>
#include <damage_types.h>
inherit LIB_ITEM;
int damage1();
int damage2();
int damage3();
int damage4();
object victim;
int counter, duration;

void create(){
    ::create();
    SetKeyName("meditate mojo");
    SetId(({"meditate mojo"}));
    SetLong(" ");
    SetInvis(1);
    set_heart_beat(1);
    counter = 0;
}

void init(){
    if(living(environment())){
        victim = environment();
    }
}


int eventMojofy(){
    int x;
    x=random(100);
    if(x < 5) damage4();
    else if(x < 20) damage3();
    else if(x < 35) damage2();
    else if(x < 50) damage1();
    return 1;
}

int damage1(){
    if(victim) victim->AddStaminaPoints(random(5)+1);
    return 1;
}

int damage2(){
    if(victim) victim->AddHP(random(2)+1);
    if(victim) victim->AddStaminaPoints(random(5)+1);
    return 1;
}

int damage3(){
    if(victim) victim->AddHP(random(3)+1);
    if(victim) victim->AddStaminaPoints(random(5)+1);
    return 1;
}

int damage4(){
    if(victim) victim->AddHP(random(4)+1);
    if(victim) victim->AddStaminaPoints(random(5)+1);
    return 1;
}

void heart_beat(){
    if(environment() && !living(environment())) this_object()->eventDestruct();
    if(counter == (30 + duration)){
        if(environment()) tell_object(environment(),"%^CYAN%^You feel the effects of your meditation wear off.%^RESET%^");
        this_object()->eventMove("/domains/town/room/furnace");
    }

    counter++;
    eventMojofy();
}

int AddDuration(int x){
    return duration += x;
}

int eventDispel(){
    counter = (30 + duration);
    heart_beat();
    return 1;
}

mixed CanGet(object ob) { return " ";}
mixed CanDrop(object ob) { return " ";}

