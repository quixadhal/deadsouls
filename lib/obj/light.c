#include <lib.h>
#include <damage_types.h>
#include <vendor_types.h>

inherit LIB_ITEM;

int Duration = 2;

int eventDematerialize(){
    object env = environment();
    object owner;
    string what;
    if(env && living(env)){
        owner = env;
        env = environment(owner);
    }
    if(!env && clonep(this_object())){
        ::eventDestruct();
        return 1;
    }
    if(owner){
        what = owner->GetName()+"'s magical glow";
    }
    else what = "The magical glow";
    tell_room(env,what+" fades away.", ({ owner }));
    if(owner){
        tell_object(owner,"%^BOLD%^%^CYAN%^Your magical glow fades away."+
                "%^RESET%^");
    }
    ::eventDestruct();
    return 1;
}

static void create() {
    item::create();
    AddSave( ({ "Duration" }) );
    SetKeyName("magic glow");
    SetId( ({ "magic glow spell"}));
    SetShort("a brilliant glow");
    SetLong("A magical source of light.");
    SetMass(0);
    SetPreventDrop(1);
    SetInvis(1);
    SetRadiantLight(30);
    set_heart_beat(1);
}

int eventDeteriorate(int type){
    return type;
}

int SetDuration(int i){
    Duration = abs(i);
    return Duration;
}

void init(){
    ::init();
}

void heart_beat(){
    Duration--;
    if(clonep(this_object())){
        if(Duration < 1){
            call_out("eventDematerialize",1);
        } 
    }
}

string GetAffectLong(object ob) {
    if(!ob || !living(ob)) return 0;
    return "There is a magical glow around " + ob->GetName() + ".";
}

int CanSell(){
    return 0;
}
