mixed eventTurnOn(object ob){
    return "#Please be more specific";;
}

mixed eventTurnOff(object ob){
    return "#Please be more specific";
}

varargs mixed CanTurn(object who, object what){
    return 1;
}

varargs mixed CanTurnOn(object who, object what){
    return 1;
}

varargs mixed CanTurnOff(object who, object what){
    return 1;
}

int eventTurn(object who){
    if(!who) who = this_player();
    return 1;
}

int direct_turn_obj(object target){
    return CanTurn(this_player());
}

int direct_turn_on_obj(object target){
    return CanTurnOn(this_player());
}

int direct_turn_on_obj_on_obj(object target, object thinger){
    return CanTurnOn(this_player(), target );
}

int indirect_turn_on_obj_on_obj(object thinger, object target){
    return CanTurnOn(this_player(), target );
}

int direct_turn_off_obj(object target){
    return CanTurnOff(this_player());
}

int direct_turn_off_obj_on_obj(object target, object thinger){
    return CanTurnOff(this_player(), target);
}

int indirect_turn_off_obj_on_obj(object thinger, object target){
    return CanTurnOff(this_player(), target);
}

int direct_turn_obj_on(object target){
    return CanTurnOn(this_player());
}

int direct_turn_obj_on_obj_on(object target, object thinger){
    return CanTurnOn(this_player(), target );
}

int indirect_turn_obj_on_obj_on(object thinger, object target){
    return CanTurnOn(this_player(), target );
}

int direct_turn_obj_off(object target){
    return CanTurnOff(this_player());
}

int direct_turn_obj_on_obj_off(object target, object thinger){
    return CanTurnOff(this_player(), target );
}

int indirect_turn_obj_on_obj_off(object thinger, object target){
    return CanTurnOff(this_player(), target );
}
