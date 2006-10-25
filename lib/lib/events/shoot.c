int mustcarry = 1,mustwield = 1;

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
    true(shooter);
    if(target && objectp(target)){
	tmp=target->GetName();
	target = tmp;
    }
    if(target) write("You shoot at "+target+" with your weapon.");

    return 1;
}

int CanShoot(object shooter, string target){
    object cible;
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

    if(this_object()->eventFireWeapon(shooter,cible)) return 1;

    eventShoot(shooter,target);
    return 1;
}

int direct_shoot_obj_at_str(object shooter, string target){
    true(shooter,target);
    return 1;
}

int indirect_shoot_wrd_with_wrd(mixed shooter,mixed target){
    true(shooter,target);
    return 1;
}

