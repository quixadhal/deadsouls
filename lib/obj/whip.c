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
        what = owner->GetName()+"'s energy whip";
    }
    else what = "The energy whip";
    tell_room(env,what+" fades away and disappears.", ({ owner }));
    if(owner){
        tell_object(owner,"%^CYAN%^Your energy whip fades away and  "
                "disappears.%^RESET%^");
    }
    ::eventDestruct();
    return 1;
}

int eventDispel(){
    return eventDematerialize();
}

static void create() {
    item::create();
    AddSave( ({ "Duration" }) );
    SetKeyName("energy whip");
    SetId( ({ "whip"}));
    SetAdjectives( ({ "energy","magic","magical" }));
    SetShort("an energy whip");
    SetLong("A whip made of magical energy");
    SetMass(0);
    SetVendorType(VT_WEAPON);
    SetClass(30);
    SetDamageType(MAGIC);
    SetWeaponType("magic");
    set_heart_beat(1);
}

mixed CanDrop(object who){ return 0; }

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
        if(!GetWielded() || Duration < 1){
            call_out("eventDematerialize",1);
        } 
    }
}

int CanSell(){
    return 0;
}
