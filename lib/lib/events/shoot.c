static int mustcarry = 1,mustwield = 1;

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

mixed eventShoot(mixed shooter, mixed target){
    string tmp;
    if(target && objectp(target)){
        tmp=target->GetName();
        target = tmp;
    }
    if(target) write("You shoot at "+target+" with your weapon.");

    return 1;
}

mixed CanShoot(object shooter, mixed target){

    if(this_object() == shooter && mustcarry > 0 && environment(this_object()) != this_player()){
        return "#You are not holding the "+remove_article(shooter->GetShort())+".";
    } 
    if(this_object() == shooter && mustwield > 0 && this_object()->GetWorn() == 0 && !creatorp(this_player())){
        return "#You are not wielding the "+remove_article(shooter->GetShort())+".";
    }

    return 1;
}

varargs mixed direct_shoot_obj_at_obj(mixed args...){
    return CanShoot(args[0],args[1]);
}

varargs mixed direct_shoot_obj_with_obj(mixed args...){
    return 1;
}

varargs mixed indirect_shoot_obj_with_obj(mixed args...){
    return CanShoot(args[1],args[0]);
}

varargs mixed indirect_shoot_obj_at_obj(mixed args...){
    return 1;
}

