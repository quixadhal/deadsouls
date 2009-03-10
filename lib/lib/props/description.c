/*    /lib/props/description.c
 *    From the Dead Souls Mud Library
 *    Handles the describing of objects
 *    Created by Descartes of Borg 961222
 *    Version: @(#) description.c 1.1@(#)
 *    Last modified: 96/12/22
 */

#include <lib.h>
#include <function.h>

inherit LIB_SEARCH;
inherit LIB_DISARM;

private int   ProperNoun = 0;
private int   Modify     = 1;
private mixed Short      = 0;

mixed direct_modify_obj_str(){
    return 1;
}

mixed direct_modify_word_str(){
    return 1;
}

mixed direct_initfix_obj(){
    return 1;
}

mixed direct_initfix(){
    return 1;
}

mixed direct_createfix_obj(){
    return 1;
}

mixed direct_createfix(){
    return 1;
}

mixed direct_reload_obj(){ return 1; }
mixed indirect_reload_obj(){ return 1; }
mixed direct_reload_word_obj(){ return 1; }
mixed indirect_reload_word_obj(){ return 1; }
mixed direct_reload_str_obj(){ return 1; }
mixed indirect_reload_str_obj(){ return 1; }
mixed direct_reload_str_word(){ return 1; }
mixed indirect_reload_str_word(){ return 1; }

mixed direct_reload(){ return 1; }
mixed indirect_reload(){ return 1; }

mixed direct_reload_every_str(){ return 1; }
mixed indirect_reload_every_str(){ return 1; }

mixed direct_copy_obj_to_obj(){ return 1; }
mixed direct_copy_obj_str(){ return 1; }

mixed direct_add_obj_to_obj(){ return 1; }
mixed indirect_add_obj_to_obj(){ return 1; }

mixed indirect_read_obj_at_obj(){ return 1; }

mixed direct_delete_obj_from_obj(){
    return 1;
}

mixed indirect_delete_obj_from_obj(){
    return 1;
}

mixed indirect_delete_obj_from_here(){ return 1; }
mixed direct_delete_obj_from_here(){ return 1; }
mixed direct_delete_obj_from_room(){ return 1; }
mixed indirect_delete_obj_from_room(){ return 1; }

mixed indirect_add_obj_to_here(){ return 1; }
mixed direct_add_obj_to_here(){ return 1; }
mixed indirect_add_obj_to_room(){ return 1; }
mixed direct_add_obj_to_room(){ return 1; }

mixed indirect_add_obj(){ return 1; }
mixed direct_add_obj(){ return 1; }

mixed indirect_delete_obj(){ return 1; }
mixed direct_delete_obj(){ return 1; }

int SetNoModify(int i){
    if(i) Modify = 0;
    else Modify = 1;
}

int GetModify(){
    return Modify;
}

int GetNoModify(){
    if(Modify) return 0;
    else return 1;
}

string GetShort();

string GetDefiniteShort(){
    string tmp = GetShort();

    if( !tmp ){
        return 0;
    }
    if( ProperNoun ){
        return tmp;
    }
    else {
        return add_article(tmp, 1);
    }
}

string GetShort(){
    if( !Short ){
        return 0;
    }
    if( functionp(Short) ){
        if( functionp(Short) & FP_OWNER_DESTED ){
            return "Error in evaluating function pointer.";
        }
        return evaluate(Short);
    }
    else if( arrayp(Short) ){
        return Short[query_night()];
    }
    else {
        return Short;
    }
}

varargs mixed SetShort(mixed val, int proper){
    if( !stringp(val) && !functionp(val) && !arrayp(val) ){
        error("Bad argument 1 to SetShort().\n");
    }
    if( proper ){
        ProperNoun = proper;
    }
    else if( stringp(val) ){ // add a support for most old stuff...
        if( strlen(val) > 2 ){
            string tmp = lower_case(val[0..2]);

            if( tmp[0..1] != "a " && tmp != "an " ){
                ProperNoun = 1;
            }
        }
        else {
            ProperNoun = 1;
        }
    }
    return (Short = val);
}

object array GetDummyItems(){
    object *DummyItems = ({});
    foreach(object item in all_inventory(this_object())){
        if(base_name(item) == LIB_DUMMY){
            DummyItems += ({ item });
        }
    }
    return DummyItems;
}
