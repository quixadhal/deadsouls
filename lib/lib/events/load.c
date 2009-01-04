int MaxLoad = 1;
int Loaded = 0;

varargs mixed CanLoad(object who, object what){
    if(who && environment() && environment() != environment(who) &&
            environment() != who && environment(environment()) != environment(who)
            && environment(environment()) != who ){
        return "#That's not accessible to you.";
    }
    else return 1;
}

varargs mixed CanUnload(object who, object what){
    if(who && environment() && environment() != environment(who) &&
            environment() != who && environment(environment()) != environment(who)
            && environment(environment()) != who ){
        return "#That's not accessible to you.";
    }
    else return 1;
}

varargs mixed eventLoad(object what, object where){
    return 1;
}

varargs mixed eventUnload(object what, object where){
    return 1;
}

int direct_load_obj(object target){
    return CanLoad(this_player());
}

int direct_unload_obj(object target){
    return CanUnload(this_player(), target);
}

int direct_load_obj_word_obj(object thingus, object target){
    return CanLoad(this_player(), target);
}

int indirect_load_obj_word_obj(object target, object thingus){
    return CanLoad(this_player(), target);
}

int direct_unload_obj_word_obj(object thingus, object target){
    return CanUnload(this_player(), target);
}

int indirect_unload_obj_word_obj(object one, object two, object three){
    return CanUnload(this_player(), one);
}

int SetMaxLoad(int i){
    return MaxLoad = i;
}

int GetMaxLoad(int i){
    return MaxLoad;
}

int SetLoaded(int i){
    return Loaded = i;
}

int GetLoaded(int i){
    return Loaded;
}

int GetMaxLoaded(){
    return Loaded;
}
