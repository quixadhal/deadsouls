static mapping Shadows = ([]);

nomask int AddShadow(object sombra){
    if(!sombra) return 0;
    if(!Shadows) Shadows = ([]);
    if(member_array(sombra, keys(Shadows)) != -1) return 0;
    Shadows[sombra] = file_name(sombra);
    return 1;
}

nomask int RemoveShadow(object sombra){
    if(!Shadows){
        Shadows = ([]);
        return 0;
    }
    if(!Shadows[sombra]) return 0;
    map_delete(Shadows,sombra);
    return 1;
}

nomask mapping GetShadows(){
    return copy(Shadows);
}
