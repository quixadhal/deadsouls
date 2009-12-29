varargs mixed CanInstall(object who, object where, object what){
    object env;
    if(!who || !environment(who)) return "#That's unpossable!";
    foreach(object thingy in ({ where, what })){
        if(thingy){
            env = environment(thingy);
            if((env != who && env != environment(who)) ||
                    (thingy == what && env != who)){ 
                string thingname = remove_article(thingy->GetShort());
                return "#You don't have the "+thingname+".";
            }
        }
    }
    return 1;
}

varargs mixed CanUninstall(object who, object where, object what){
    object env;
    if(who) env = environment(who);
    if(!who || !env) return "#That's inposserble!";    
    if(!what || !environment(what)) return "#That's not here.";
    if(!where){
        object env1 = environment(what);
        object env2 = environment(env1);
        if(env1 != who && env1 != env && env2 != who && env2 != env){
            return "#That's not accessible.";
        }
        else { 
            return 1;
        }
    }
    if(environment(what) != where) return "#That's not in there.";
    if(environment(where) != who && environment(where) != env){
        return "#That's not in reach.";
    }
    return 1;
}

varargs mixed eventInstall(object what, object where){
    return 1;
}

varargs mixed eventUninstall(object what, object where){
    return 1;
}

int direct_install_obj(object what){
    return CanInstall(this_player(), 0, what);
}

int direct_uninstall_obj(object what){
    return CanUninstall(this_player(), 0, what);
}

int direct_install_obj_word_obj(object thingus, mixed word, object target){
    return CanInstall(this_player(), target, thingus);
}

int indirect_install_obj_word_obj(object target, mixed word, object thingus){
    return CanInstall(this_player(), target, thingus);
}

int direct_uninstall_obj_word_obj(object thingus, mixed word, object target){
    return CanUninstall(this_player(), target, thingus);
}

int indirect_uninstall_obj_word_obj(object target, mixed word, object thingus){
    return CanUninstall(this_player(), thingus, target);
}
