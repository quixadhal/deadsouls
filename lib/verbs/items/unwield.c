/*    /verbs/items/unwield.c
 *    From the Dead Souls Mud Library
 *    Allows players to stop wielding a weapon
 *    Created by Descartes of Borg 960207
 *    Version: @(#) unwield.c 1.2@(#)
 *    Last modified: 97/01/01
 */

#include <lib.h>

inherit LIB_VERB;

static void create() {
    verb::create();
    SetVerb("unwield");
    SetRules("OBS");
    SetErrorMessage("Unwield what?");
    SetHelp("Syntax: <unwield ARMOR>\n"
            "        <unwield all [of WEAPON]>\n\n"
            "This verb allows you to unwield a weapon which you are "
            "currently wielding.\n\n"
            "See also: get, remove, wear, wield");
}

mixed can_unwield_obj(string verb) {
    if( this_player()->GetParalyzed() ) {
        return "You cannot do anything.";
    }
    return 1;
}

mixed do_unwield_obj(object ob) {
    return ob->eventUnequip(this_player());
}

mixed do_unwield_obs(mixed array targs) {
    object array obs;

    if( !sizeof(targs) ) {
        this_player()->eventPrint("There is no such thing to be unwielded.");
        return 1;
    }
    obs = filter(targs, (: objectp :));
    if( !sizeof(obs) ) {
        mapping messages = unique_mapping(targs, (: $1 :));

        foreach(string msg in keys(messages)) {
            this_player()->eventPrint(msg);
        }
        return 1;
    }
    foreach(object item in obs) {
        do_unwield_obj(item);
    }
    return 1;
}
