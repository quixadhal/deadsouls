#include <lib.h>

static mapping Shadows = ([]);

nomask int AddShadow(object sombra){
    //tc("hit hook");
    if(!sombra) return 0;
    //tc("1");
    if(!Shadows) Shadows = ([]);
    //tc("2");
    if(member_array(sombra, keys(Shadows)) != -1) return 0;
    //tc("4");
    Shadows[sombra] = file_name(sombra);
    //tc("Shadow: "+identify(Shadows));
    return 1;
}

nomask int RemoveShadow(object sombra){
    if(!Shadows){
        Shadows = ([]);
        return 0;
    }
    if(!Shadows[sombra]) return 0;
    map_delete(Shadows,sombra);
    //tc("Shadow: "+identify(Shadows));
    return 1;
}

nomask mapping GetShadows(){
    return copy(Shadows);
}
