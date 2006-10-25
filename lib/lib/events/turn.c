int eventTurnOn(object ob){
    true(ob);
    return 1;
}

int eventTurnOff(object ob){
    true(ob);
    return 1;
}

mixed CanTurn(object who) {
    true(who);
    return 1;
}

mixed CanTurnOn(object who) {
    true(who);
    return 1;
}
mixed CanTurnOff(object who) {
    true(who);
    return 1;
}


int eventTurn(object who) {
    true(who);
    if(!who) who = this_player();
    //    send_messages("turn", "$agent_name $agent_verb $target_name.",
    //                  who, this_object(), environment(who));
    return 1;
}

int direct_turn_obj(object target) {
    true(target);
    return CanTurn(this_player());
}

int direct_turn_on_obj(object target){
    true(target);
    return CanTurnOn(this_player());
}

int direct_turn_off_obj(object target){
    true(target);
    return CanTurnOff(this_player());
}

