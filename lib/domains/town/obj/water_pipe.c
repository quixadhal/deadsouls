#include <lib.h>
#include <vendor_types.h>

inherit LIB_ITEM;
static int open = 0;

void create(){
    ::create();
    SetKeyName("pipe");
    SetId( ({"pipe","tube"}) );
    SetAdjectives( ({"water", "thick","metal"}) );
    SetShort("a thick metal pipe embedded in the bedrock");
    SetLong("This is a metal pipe embedded in the bedrock.");
    SetMass(20);
    SetBaseCost("silver",10);
    SetVendorType(VT_TREASURE);
    SetInvis(0);
    SetPreventGet("It's planted firmly in the bedrock.");
    SetNoCondition(1);
    set_heart_beat(1);
}

void init(){
    ::init();
}

int openpipe(int i){
    if(open && !i) open = 0;
    else open = 1;
    if(open){
        object flow;
        int tmp;
        object env = environment(this_object());
        if(!env) return 0;
        flow = present("flow object", env);
        if(!flow){
            flow = new(LIB_FLOW);
            if(!flow) return;
            tmp = flow->eventMove(environment(this_object()));
            if(!tmp) return;
        }
        flow->SetPressure(40);
    }
    return open;
}

void heart_beat(){
    object env;
    if(!clonep() || !env) return;
    if(open){
        object flow;
        int tmp;
        env = environment(this_object());
        flow = present("flow object", env);
        if(!flow){  
            flow = new(LIB_FLOW);
            if(!flow) return;
            tmp = flow->eventMove(environment(this_object()));
            if(!tmp) return;
        }
        tmp = flow->GetPressure();
        if(tmp < 5) flow->SetPressure(20);
    }
}
