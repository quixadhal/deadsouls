#include <lib.h>
#include <position.h>
#include "include/target.h"

inherit LIB_VERB;

static void create() {
    verb::create();
    SetVerb("target");
    SetRules("LVS", "only LVS","LVS only");
    SetErrorMessage("Target whom?");
    SetHelp("Syntax: target <LIVING>\n"
            "        target all of <LIVING>\n"
            "        target <all>\n\n"
            "This command initiates exclusive combat with a living "
            "being or group of living beings using any wielded weapons "
            "or your bare hands.  Anyone else in the room at the time "
            "you issue this command will be ignored by you, even if "
            "they attack you. \n"
            "See also: attack, wimpy, ignore, attack");
}

mixed can_target_liv(object target) {
    int pos = this_player()->GetPosition();

    if( this_player()->GetParalyzed() ) {
        return "You cannot move!";
    }
    if( pos == POSITION_SITTING || pos == POSITION_LYING ) {
        return "You cannot target in that position!";
    }
    if( environment(this_player())->GetProperty("no target") ) {
        message("environment", "A mystical force prevents your malice.",
                this_player());
        return this_player()->CanManipulate();
    }
    return this_player()->CanManipulate();
}

mixed can_target_only_liv(object target){
    return can_target_liv(target);
}

mixed can_target_liv_only(object target){
    return can_target_liv(target);
}

mixed do_target_liv(object target) {
    return do_target_lvs(({ target }), 1);
}

mixed do_target_only_liv(object target){
    return do_target_lvs(({ target }), 1);
}

mixed do_target_liv_only(object target){
    return do_target_lvs(({ target }), 1);
}

varargs mixed do_target_lvs(mixed *targets, int exclusive) {
    object *obs;
    object *notarget;
    string tmp;

    notarget = ({});
    obs = filter(targets, (: objectp :));
    if( !sizeof(obs) ) {
        mixed *ua;

        ua = unique_array(targets, (: $1 :));
        foreach(string *lines in ua) this_player()->eventPrint(lines[0]);
        return 1;
    }
    foreach(object entity in get_livings(environment(this_player()))){
        if(member_array(entity,obs) == -1) notarget += ({ entity });
    }
    if(sizeof(notarget)) this_player()->AddNonTargets(notarget);
    this_player()->SetAttack(obs);
    tmp = item_list(obs);
    obs->eventPrint(this_player()->GetName() + " targets you!");
    environment(this_player())->eventPrint(this_player()->GetName() +
            " targets " + tmp + "!",
            ({ this_player(), obs... }));
    this_player()->eventPrint("You target " + tmp + ".");
    return 1;
}
