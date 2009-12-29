#include <lib.h>

inherit LIB_TURN;

int direct_activate_obj(object target){
    return turn::CanTurnOn(this_player());
}

int direct_deactivate_obj(object target){
    return turn::CanTurnOff(this_player());
}

int direct_activate_obj_on_obj(object target, object where){
    return turn::CanTurnOn(this_player(), target);
}

int direct_activate_str_on_obj(string target, object where){
    return turn::CanTurnOn(this_player(), where);
}

int direct_deactivate_obj_on_obj(object target, object where){
    return turn::CanTurnOff(this_player(), target);
}

int direct_deactivate_str_on_obj(string target, object where){
    return turn::CanTurnOff(this_player(), where);
}

