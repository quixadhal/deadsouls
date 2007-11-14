#include <lib.h>

inherit LIB_VERB;

static void create() {
    verb::create();
    SetVerb("shoot");
    SetRules("OBJ at OBJ", "OBJ with OBJ");
    SetSynonyms("fire", "gat", "gank");
    SetErrorMessage("Shoot what?");
    SetHelp("Syntax: <shoot OBJECT>\n\n"
      "Shoots a target.\n\n");
}

mixed eventShoot(mixed device, mixed target){
    mixed attackable;
    if(!device || !objectp(device) ) return 0;
    if(!target || !objectp(target)) return 0;
    if(target == device) return 0;

    if(device->MustCarry() && environment(device) != this_player()) {
        write("You are not holding the weapon.");
        return 1;
    }
    if(device->MustWield() && device->GetWorn() == 0 && !creatorp(this_player())) {
        write("You are not wielding the weapon.");
        return 1;
    }

    if(!present(target,environment(this_player())) && !present(target,environment(device))){
        write("That target is not here.");
        return 1;
    }

    attackable = target->GetAttackable();

    if(!attackable || !intp(attackable) || attackable != 1){
        write("You are unable to shoot "+objective(target)+".");
        return 1;
    }

    return device->eventShoot(device, target);
}


mixed can_shoot_obj_with_obj(mixed target,mixed shooter) {
    if(intp(check_light())) return this_player()->CanManipulate();
    else return check_light();
}

mixed can_shoot_obj_at_obj(mixed shooter,mixed target) {
    if(intp(check_light())) return this_player()->CanManipulate();
    else return check_light();
}

varargs mixed do_shoot_obj_at_obj(mixed args...) {
    return eventShoot(args[0],args[1]);
}

varargs mixed do_shoot_obj_with_obj(mixed args...) {
    return eventShoot(args[1], args[0]);
}
