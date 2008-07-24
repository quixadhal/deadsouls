mixed CanSmoke(object who){
    return 1;
}

int direct_smoke_obj(object who, object target){
    return CanSmoke(this_player());
}

int direct_smoke_from_obj(object who, object target){
    return CanSmoke(this_player());
}
