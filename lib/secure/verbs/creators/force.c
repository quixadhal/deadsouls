#include <lib.h>

inherit LIB_VERB;

static void create() {
    verb::create();
    SetVerb("force");
    SetRules("LIV STR", "LIV to STR");
    SetErrorMessage("Force whom to do what?");
    SetHelp("Syntax: <force LIVING SOMETHING>\n"
            "        <force LIVING to DO SOMETHING>\n"
            "Allows you to command some living things to do "
            "your bidding.");
}

mixed can_force_liv_to_str(string str) { 
    if(!creatorp(this_player())) return 0;
    return 1;
}

mixed can_force_liv_str(string str) {
    return can_force_liv_to_str(str);
}

mixed do_force_liv_to_str(object target, string cmd) {
    object who = this_player();

    if(!who) return 0;
    if(archp(target) && !securep(who)){
        who->eventPrint(target->GetName()+" shakes "+possessive(target)+
                " head and forces you to dest yourself.");
        tell_room(environment(who), who->GetName()+" dests "+objective(who)+
                "self while trying to pull a foolish joke on "+target->GetName()+".", who);
        tell_player(target,who->GetName()+" tried to force you to "+cmd);
        tell_player(target,who->GetName()+" has been dested, instead.");
        who->eventDestruct();
        return 1;
    }
    target->eventPrint(who->GetName() + " forces you to: " + cmd);
    who->eventPrint("You force " + target->GetShort() + " to: " + cmd);
    target->eventForce(cmd);
    return 1;
}

mixed do_force_liv_str(object ob, string str) {
    return do_force_liv_to_str(ob, str);
}
