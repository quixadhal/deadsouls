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

int eventShoot(mixed shooter, mixed target){
    string tmp;
    if(target && objectp(target)){
	tmp=target->GetName();
	target = tmp;
    }
    if(target) write("You shoot at "+target+" with your weapon.");

    return 1;
}

int CanShoot(object shooter, mixed target){
    object cible;
    mixed attackable;

    if(living(shooter)) return 0;
    if(mustcarry > 0 && environment(this_object()) != this_player()) {
	write("You are not holding the weapon.");
	return 1;
    } 
    if(mustwield > 0 && this_object()->GetWorn() == 0 && !creatorp(this_player())) {
	write("You are not wielding the weapon.");
	return 1;
    }

    if(!present(target,environment(this_player())) && !present(target,environment(this_object()))){
	write("That target is not here.");
	return 1;
    }
    if(stringp(target) && !cible=present(target,environment(this_object()))){
	cible=present(target,environment(this_player()));
    }

    if(objectp(target)) cible = target;

    attackable = cible->GetAttackable();

    if(!attackable || !intp(attackable) || attackable != 1){
	write("You are unable to shoot "+objective(cible)+".");
	return 0;
    }

    if(this_object()->eventFireWeapon(shooter,cible)) return 1;

    eventShoot(shooter,target);
    return 1;
}

int direct_shoot_obj_at_obj(object shooter, object target){
    return 1;
}

int direct_shoot_obj_with_obj(object shooter, object target){
    return 1;
}

int direct_shoot_obj_at_str(object shooter, string target){
    return 1;
}

int indirect_shoot_obj_with_obj(mixed ob1,mixed ob2){
    return 1;
}

int indirect_shoot_obj_at_obj(mixed ob1,mixed ob2){
    return 1;
}

