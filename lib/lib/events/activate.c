#include <lib.h>

inherit LIB_TURN;

int direct_activate_obj(object target){
    return turn::CanTurnOn(this_player());
}

int direct_deactivate_obj(object target){
    return turn::CanTurnOff(this_player());
}

int direct_activate_obj_on_obj(object target){
    return turn::CanTurnOn(this_player(), target);
}

int direct_deactivate_obj_on_obj(object target){
    return turn::CanTurnOff(this_player(), target);
}
