#include <lib.h>

inherit LIB_VERB;

mixed can_shoot_obj_with_obj(mixed target,mixed shooter);
mixed can_shoot_obj_at_obj(mixed shooter,mixed target);
varargs mixed do_shoot_obj_at_obj(mixed args...);
varargs mixed do_shoot_obj_with_obj(mixed args...);

static void create() {
    verb::create();
    SetVerb("shoot");
    SetRules("OBJ at LIV", "LIV with OBJ","OBJ at OBJ", "OBJ with OBJ");
    //SetRules("OBJ at LIV", "LIV with OBJ");
    SetSynonyms("fire", "gat", "gank");
    SetErrorMessage("Shoot what?");
    SetHelp("Syntax: <shoot OBJECT>\n\n"
            "Shoots a target.\n\n");
}

varargs mixed eventShoot(mixed device, mixed target, mixed misc...){
    mixed attackable;
    object tmpob;

    if(!device || !objectp(device) || target == device ){
        write("You can't shoot that with that.");
        return 0;
    }
    if(device->MustCarry() && environment(device) != this_player()) {
        write("You are not holding the weapon.");
        return 1;
    }
    if(device->MustWield() && device->GetWorn() == 0 && !creatorp(this_player())) {
        write("You are not wielding the weapon.");
        return 1;
    }
    if(!target || !objectp(target)){
        write("It seems that is not a valid target.");
        return 0;
    }
    if(!present(target,environment(this_player())) && !present(target,environment(device))){
        write("That target is not here.");
        return 1;
    }

    attackable = target->GetAttackable();

    if(!attackable || !intp(attackable) || attackable != 1){
        write("You are unable to shoot "+target->GetShort()+".");
        return 1;
    }

    return device->eventShoot(device, target);
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

varargs mixed do_shoot_obj_at_liv(mixed args...) {
    return do_shoot_obj_at_obj(args...);
}

varargs mixed do_shoot_liv_with_obj(mixed args...) {
    return do_shoot_obj_at_obj(args[1], args[0]);
}

varargs mixed do_shoot_obj_at_obj(mixed args...) {
    return eventShoot(args[0],args[1],args[2..]);
}

varargs mixed do_shoot_obj_with_obj(mixed args...) {
    return eventShoot(args[1], args[0], args[2..]);
}
