/*    /verbs/items/remove.c
 *    From the Dead Souls Mud Library
 *    Allows players to remove armor
 *    Created by Descartes of Borg 960207
 *    Version: @(#) remove.c 1.2@(#)
 *    Last modified: 97/01/01
 */

#include <lib.h>

inherit LIB_VERB;

static void create() {
    verb::create();
    SetVerb("remove");
    SetRules("OBS", "OBS from OBJ", "OBS out of OBJ");
    SetErrorMessage("Remove what?");
    SetHelp("Syntax: <remove ARMOR>\n"
            "        <remove all [of ARMOR]>\n"
            "        <remove ITEM from CONTAINER>\n\n"
            "This verb allows you to remove a piece of armor which you are "
            "currently wearing.\n\n"
            "The second syntax is simply a synonym for the \"get\" "
            "command.\n\n"
            "See also: get, wear, wield, unwield");
    SetSynonyms("take off", "unwear");
}

mixed can_remove_obj() {
    if( this_player()->GetParalyzed() ) {
        return "You cannot do anything.";
    }
    if(intp(check_light())) return this_player()->CanManipulate();
    else return 1;

}

mixed can_remove_obj_out_of_obj() {
    int light;

    if(intp(check_light())) return this_player()->CanManipulate();
    else return check_light();
}

mixed can_remove_obj_from_obj() {
    return can_remove_obj_out_of_obj();
}

mixed do_remove_obj(object ob) {
    return ob->eventUnequip(this_player());
}

mixed do_remove_obs(mixed *res) {
    object *obs;

    obs = filter(res, (: objectp :));
    if( !sizeof(obs) ) {
        mixed *ua;

        ua = unique_array(res, (: $1 :));
        foreach(string *list in ua)
            this_player()->eventPrint(list[0]);
        return 1;
    }
    res = (mixed *)obs->eventUnequip(this_player()) - ({ 1 });
    if( !sizeof(res) ) return 1;
    else return res[0];
}

mixed do_remove_obj_out_of_obj(object targ, object container) {
    return container->eventGetFrom(this_player(), ({ targ }));
}  

mixed do_remove_obj_from_obj(object targ, object container) {
    return do_remove_obj_out_of_obj(targ, container);
}

mixed do_remove_obs_out_of_obj(mixed *res, object container) {
    return container->eventGetFrom(this_player(), res);
}

mixed do_remove_obs_from_obj(mixed *res, object container) {
    return do_remove_obs_out_of_obj(res, container);
}

