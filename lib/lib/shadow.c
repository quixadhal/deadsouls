#include <lib.h>
#include <dirs.h>

static object shadowed;
static object shadow_thing = this_object();
static int shadowing;

int eventShadow(object ob){
    if(!shadowing){
        shadowing = 1;
        shadowed = ob;
        ob->AddShadow(shadow_thing);
        shadow(ob);
        return 1;
    }
    else return 0;
}

mixed GetShadowed(){
    return shadow(shadowed,0);
}

string GetShadowedName(){
    return shadowed->GetName();
}

object GetShadowedObject(){
    return shadowed;
}

int eventUnshadow(){
    if(shadowed){
        shadowed->RemoveShadow(shadow_thing);
    }
    if(shadow_thing){
        return destruct(shadow_thing);
    }
    else return 0;
}
