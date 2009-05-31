/*    /lib/events/wear.c
 *    From the Dead Souls Mud Library
 *    An object that can be worn
 *    Created by Descartes of Borg 970101
 *    Version: @(#) wear.c 1.1@(#)
 *    Last modified: 97/01/01
 */

// abstract methods
string array GetRestrictLimbs();
mixed CanEquip(object who, string array limbs);
mixed CanUnequip(object who);
// end abstract methods

mixed direct_remove_obj(){

    if( environment(this_object()) != this_player() ){
        return "#You don't have that!";
    }
    return CanUnequip(this_player());
}

mixed direct_wear_obj(){
    if( environment(this_object()) != this_player() ){
        return "#You don't have that!";
    }
    return CanEquip(this_player(), GetRestrictLimbs());
}

mixed direct_wear_obj_on_str(object target, string str){
    if( environment(this_object()) != this_player() ){
        return "#You don't have that!";
    }
    return CanEquip(this_player(), ({ remove_article(lower_case(str)) }));
}

