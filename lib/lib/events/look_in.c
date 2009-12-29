/*    /lib/events/look_in.c
 *    From the Dead Souls Mud Library
 *    Something that can be seen in
 *    Created by Descartes of Borg 961222
 *    Version :@(#) look_in.c 1.2@(#)
 *    Last modified: 96/12/31
 */

#include <function.h>

private string InternalDesc = 0;
int    Opacity = 100;

// abstract methods
string GetShort();
// end abstract methods

string GetInternalDesc(){
    mixed val = InternalDesc;

    if( arrayp(val) ){
        val = val[query_night()];
    }
    if( stringp(val) ){
        return val;
    }
    else if( functionp(val) ){
        if( functionp(val) & FP_OWNER_DESTED ){
            return "Error evaluating internal description.";
        }
        return evaluate(val, this_player());
    }
}

string SetInternalDesc(string str){
    return (InternalDesc = str);
}

int GetOpacity(){
    return Opacity;
}

int SetOpacity(int x){
    Opacity = x;
    parse_refresh();
    return Opacity;
}

varargs mixed CanShowInterior(object who, object target){
    int x;
    if(!who) who = this_player();

    if( environment() != this_player()  && environment(this_player()) !=
            environment()){
        return "#You don't have that!";
    }

    if(living() && !inherits(LIB_VEHICLE,this_object())){
        return "You can't look inside of a living being.";
    }

    if( target ){
        x = 66;
    }
    else {
        x = 33;
    }
    if( this_object()->GetClosed() && this_object()->GetOpacity() > x ){
        return 0;
    }
    return 1;
}

varargs mixed eventShowInterior(object who, object target){
    object here,me,imhere,dabei;
    string this,str;
    here=environment(this_object());
    me=this_object();
    this=me->GetKeyName();
    str=me->GetInternalDesc();
    imhere=present(this,environment(who));
    dabei=present(this,who);

    if( target ){
        return "A strange event has occurred, which you should report.";
    }

    if(!imhere && !dabei){
        who->eventPrint("That is not here.");
        return 0;
    }

    if( !str || str == "" ){
        return 0;
    }
    if(!inherits(LIB_SURFACE, this_object())){
        environment(who)->eventPrint(who->GetName() + " looks inside " +
                GetShort() + ".", who);
    }
    who->eventPrint(str);

}

mixed direct_look_in_obj(){
    if(inherits(LIB_SURFACE, this_object())) return 0;
    return CanShowInterior(this_player());
}

mixed direct_look_inside_obj(){
    if(inherits(LIB_SURFACE, this_object())) return 0;
    return CanShowInterior(this_player());
}

mixed indirect_look_at_obj_word_obj(object target){
    if( environment(target) != this_object() ){
        return "#That is not in there.";
    }
    return CanShowInterior(this_player(), target);
}

mixed inventory_visible(){
    if( GetOpacity() > 33 ){
        return 0;
    }
    else {
        return 1;
    }
}
