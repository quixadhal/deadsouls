static int mustcarry = 1,mustwield = 1;
object targ;

int MustCarry(int i){
    if(i) mustcarry = i;
    if(mustcarry) return mustcarry;
    else return 0;
}

int MustWield(int i){
    if(i) mustwield = i;
    if(mustwield) return mustwield;
    else return 0;
}

varargs mixed eventAim(object who, mixed target, string dir, string whom){
    string tmp;
    if(target){
        if(objectp(target)){
            tmp=target->GetName();
            target = tmp;
        }
        else tmp = target;
    }
    if(!stringp(tmp)) tmp = "something";
    if(target) write("You aim the "+this_object()->GetShort()+" at "+
      target+".");
    targ = target;
    return 1;
}

mixed CanAim(object aimer, mixed target){
    object env = environment(this_player());
    if(this_object() == aimer && mustcarry > 0 && environment(this_object()) != this_player()){
        return "#You are not holding the "+remove_article(aimer->GetShort())+".";
    } 
    if(this_object() == aimer && mustwield > 0 && this_object()->GetWorn() == 0 && !creatorp(this_player())){
        return "#You are not wielding the "+remove_article(aimer->GetShort())+".";
    }
    return 1;
}

varargs mixed direct_aim_obj_at_obj(mixed args...){
    mixed ret;
    ret = CanAim(args[0],args[1]);
    return CanAim(args[0],args[1]);
}

varargs mixed direct_aim_obj_with_obj(mixed args...){
    return 1;
}

varargs mixed indirect_aim_obj_with_obj(mixed args...){
    mixed ret;
    ret = CanAim(args[1],args[0]);
    return CanAim(args[1],args[0]);
}

varargs mixed indirect_aim_obj_at_obj(mixed args...){
    return 1;
}

varargs mixed direct_aim_obj_at_liv(mixed args...){
    return direct_aim_obj_at_obj(args...);
}

varargs mixed direct_aim_liv_with_obj(mixed args...){
    return direct_aim_obj_with_obj(args...);
}

varargs mixed indirect_aim_liv_with_obj(mixed args...){
    return indirect_aim_obj_with_obj(args...);
}

varargs mixed indirect_aim_obj_at_liv(mixed args...){
    return indirect_aim_obj_at_obj(args...);
}

varargs mixed direct_aim_obj_wrd(mixed args...){
    return direct_aim_obj_at_obj(args...);
}

varargs mixed direct_aim_obj_wrd_at_wrd(mixed args...){
    return direct_aim_obj_at_obj(args...);
}

