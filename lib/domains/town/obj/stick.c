// A bonus stick!
// Adds 2 points to strength and coordination stats of
// whoever is holding it.

#include <lib.h>
#include <vendor_types.h>
inherit LIB_ITEM;

int CheckBonus(){
    object who = previous_object();
    if(!who) return 0;
    if(!environment() || environment() != who) return 0;
    return 2;
}

void create(){
    ::create();
    SetKeyName("stick");
    SetId( ({"branch","piece","piece of a tree"}) );
    SetAdjectives( ({"small"}) );
    SetShort("a small stick");
    SetLong("This is a piece of a tree.");
    SetMass(20);
}

void init(){
    ::init();
}

int eventMove(mixed dest){
    int ret = ::eventMove(dest);
    object env = environment();
    if(env && living(env)){ 
        env->AddStatBonus("strength", (: CheckBonus :));
        env->AddStatBonus("coordination", (: CheckBonus :));
    }
    return ret;
}
