#include <lib.h>

inherit LIB_VERB;

mixed can_shoot_obj_with_obj(mixed target,mixed shooter);
mixed can_shoot_obj_at_obj(mixed shooter,mixed target);
varargs mixed do_shoot_obj_at_obj(mixed args...);
varargs mixed do_shoot_obj_with_obj(mixed args...);

static void create() {
    verb::create();
    SetVerb("shoot");
    SetRules("OBJ at OBJ", "OBJ with OBJ",
            "OBJ WRD", "OBJ WRD at WRD");
    SetSynonyms("fire", "gat", "gank");
    SetErrorMessage("Shoot what?");
    SetHelp("Syntax: shoot OBJECT at ENEMY\n\n"
            "Shoots a target.\n\n");
}

varargs mixed eventShoot(mixed device, mixed target, string dir, string whom){
    mixed attackable;
    object tmpob, who = this_player();

    if(!device || !objectp(device) || target == device ){
        write("You can't shoot that with that.");
        return 0;
    }
    if(device->MustCarry() && environment(device) != who) {
        write("You are not holding the weapon.");
        return 1;
    }
    if(device->MustWield() && device->GetWorn() == 0 && !creatorp(who)) {
        write("You are not wielding the weapon.");
        return 1;
    }
    if((!target || !objectp(target)) && !dir){
        write("It seems that is not a valid target.");
        return 0;
    }
    if(target && !present(target,environment(who)) && 
            !present(target,environment(device))){
        write("That target is not here.");
        return 1;
    }

#if 1
    if(living(target) || target->isDummy()){
        attackable = target->GetAttackable();
        if(!attackable || !intp(attackable) || attackable != 1){
            write("You are unable to shoot "+target->GetShort()+".");
            return 1;
        }
    }
#endif
    return device->eventShoot(who, target, dir, whom);
}

mixed can_shoot_liv_with_obj(mixed target,mixed shooter){
    return can_shoot_obj_with_obj(target, shooter);
}

mixed can_shoot_obj_at_liv(mixed shooter,mixed target){
    return can_shoot_obj_at_obj(shooter, target);
}

mixed can_shoot_obj_with_obj(mixed target,mixed shooter) {
    if(intp(check_light())) return this_player()->CanManipulate();
    else return check_light();
}

mixed can_shoot_obj_at_obj(mixed shooter,mixed target) {
    if(intp(check_light())) return this_player()->CanManipulate();
    else return check_light();
}

mixed can_shoot_obj_wrd(mixed shooter,mixed dir) {
    if(intp(check_light())) return this_player()->CanManipulate();
    else return check_light();
}

mixed can_shoot_obj_wrd_at_wrd(mixed shooter, mixed dir, mixed whom) {
    if(intp(check_light())) return this_player()->CanManipulate();
    else return check_light();
}

varargs mixed do_shoot_obj_at_liv(object what, object whom) {
    return do_shoot_obj_at_obj(what, whom);
}

varargs mixed do_shoot_liv_with_obj(object whom, object what) {
    return do_shoot_obj_at_obj(what, whom);
}

varargs mixed do_shoot_obj_at_obj(object what, object whom) {
    return eventShoot(what, whom);
}

varargs mixed do_shoot_obj_with_obj(object whom, object what) {
    return eventShoot(what, whom);
}

varargs mixed do_shoot_obj_wrd(object what, string dir) {
    return eventShoot(what, 0, dir);
}

varargs mixed do_shoot_obj_wrd_at_wrd(object what, string dir, string whom) {
    return eventShoot(what, 0, dir, whom);
}

