#include <lib.h>
#include <dirs.h>

object shadowed;

int eventShadow(object ob){
    shadow(ob);
    shadowed = ob;
    return 1;
}

int wtf(){
    return 42;
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



//void create(){};
