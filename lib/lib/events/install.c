varargs mixed CanInstall(object who, object what){
    if(who && environment() && environment() != environment(who) &&
            environment() != who && environment(environment()) != environment(who)
            && environment(environment()) != who )
        return "#That's not accessible to you.";
    else return 1;
}

varargs mixed CanUninstall(object who, object what){
    if(who && environment() && environment() != environment(who) &&
            environment() != who && environment(environment()) != environment(who)
            && environment(environment()) != who )
        return "#That's not accessible to you.";
    else return 1;
}

varargs mixed eventInstall(object what, object where){
    return 1;
}

varargs mixed eventUninstall(object what, object where){
    return 1;
}

int direct_install_obj(object target){
    return CanInstall(this_player());
}

int direct_uninstall_obj(object target){
    return CanUninstall(this_player(), target);
}

int direct_install_obj_word_obj(object thingus, object target){
    return CanInstall(this_player(), target);
}

int indirect_install_obj_word_obj(object target, object thingus){
    return CanInstall(this_player(), target);
}

int direct_uninstall_obj_word_obj(object thingus, object target){
    return CanUninstall(this_player(), target);
}

int indirect_uninstall_obj_word_obj(object one, object two, object three){
    return CanUninstall(this_player(), one);
}
