#include <lib.h>

inherit LIB_STORAGE;

void create() {
    ::create();
    SetKeyName("box");
    SetAdjectives( ({"test"}) );
    SetId( ({ "container" }) );
    SetShort("a test box");
    SetLong("It is a thing that holds other things.");
    SetCanClose(1);
    SetMass(274);
    SetBaseCost("silver",1);
    SetMaxCarry(500);
}
void init(){
    ::init();
}

int eventReceiveObject(mixed arg){
    if(arg) tc("arg: "+identify(arg));
    tc("carried mass: "+this_object()->GetCarriedMass());
    tc("My mass: "+this_object()->GetMass(),"red");
    ::eventReceiveObject(arg);
    tc("carried mass: "+this_object()->GetCarriedMass(),"blue");
    tc("My mass: "+this_object()->GetMass(),"red");
    return 1;
}

int eventReleaseObject(mixed arg){
    if(arg) tc("arg: "+identify(arg));
    tc("carried mass: "+this_object()->GetCarriedMass());
    tc("My mass: "+this_object()->GetMass(),"red");
    ::eventReleaseObject(arg);
    tc("carried mass: "+this_object()->GetCarriedMass(),"blue");
    tc("My mass: "+this_object()->GetMass(),"red");
    return 1;
}
