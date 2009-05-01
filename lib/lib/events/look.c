/*    /lib/events/show.c
 *    From the Dead Souls LPC Library
 *    Responds to people looking at things on objects
 *    Created by Descartes of Borg 930616
 *    Version: @(#) look.c 1.8@(#)
 *    Last modified: 96/12/31
 */

#include <function.h>
#include <daemons.h>
#include <lib.h>
#include <vision.h>

private mixed   ExternalDesc = 0;
private int     Invisible    = 0;
private static string  look_globalval;
static function f;
mapping Items        = ([]);

// abstract methods
string GetShort();
// end abstract methods

varargs string GetExternalDesc(object who){
    string openstate;

    if(this_object()->CanClose()){
        if(this_object()->GetClosed()) openstate = " It is closed.";
        else openstate = " It is open.";
    }
    else openstate = "";

    if( !ExternalDesc ){
        return "";
    }
    if( functionp(ExternalDesc) ){
        if( functionp(ExternalDesc) & FP_OWNER_DESTED ){
            return "";
        }
        return evaluate(ExternalDesc, who);
    }
    else if( arrayp(ExternalDesc) ){
        return ExternalDesc[query_night()] + openstate;;
    }
    else {
        return ExternalDesc + openstate;
    }
}

string SetExternalDesc(string desc){
    return (ExternalDesc = desc);
}

varargs int GetInvis(object ob){
    if( !ob ){
        ob = (this_player() || previous_object());
    }
    if( functionp(Invisible) ){
        return evaluate(Invisible, ob);
    }
    else return Invisible;
}

mixed SetInvis(mixed val){
    if( !val || intp(val) ){
        if(!val || val == 0){
            Invisible = 0;
            if(sizeof(get_livings(environment(this_object()),2))){
                foreach(object ob in get_livings(environment(this_object()),2)){
                    ob->CheckEncounter();
                }
            }
        }
        else Invisible = val;
    }
    else if( functionp(val) && !Invisible ){
        Invisible = val;
    }
    INSTANCES_D->UpdateInvis( (Invisible ? 1 : 0) );
    return Invisible;
}

varargs mixed AddItem(mixed item, mixed val){
    if( objectp(item) ){
        item->eventMove(this_object());
        return 1;
    }
    if( functionp(val) || stringp(val) || arrayp(val) ){
        if( stringp(item) ){
            Items[item] = val;
        }
        else if( arrayp(item) ){
            map(item, (: AddItem($1, $(val)) :));
            return val;
        }
        else {
            error("Bad argument 1 to AddItem().\n");
        }
    }
    else error("Bad argument 2 to AddItem().\n");
    return (Items[item]);
}

//TMI2 back-compat hack
static mixed AddItem_func(mixed foo){
    foreach(mixed key, mixed val in foo){
        look_globalval = val;
        AddItem(key, (: look_globalval :) );
    }
    return foo;
}

static mixed SetItem_func(mixed foo){
    foreach(mixed key, mixed val in foo){
        look_globalval = val;
        f =  bind( (: call_other, this_object(), look_globalval :), this_object() );
        AddItem(key, (: f :) );
    }
    return foo;
}

varargs mixed GetItem(string item, object who){
    //mixed val = mapping_member(Items, item);
    mixed val = Items[item];

    if( !val ){
        return 0;
    }
    if( stringp(val) || arrayp(val)){
        return val;
    }
    else if( functionp(val) ){
        if( functionp(val) & FP_OWNER_DESTED ){
            return "An error occurred evaulating a function pointer.";
        }
        return evaluate(val, who, item);
    }
    else {
        return 0;
    }
}

string array GetItems(){
    return keys(Items);
}

mapping GetItemsMap(){
    return copy(Items);
}

mapping RemoveItem(mixed item){
    if( !stringp(item) ){
        if( !arrayp(item) ){
            error("Bad argument 1 to RemoveItem().\n");
        }
        map(item, (: RemoveItem($1) :));
        return copy(Items);
    }
    map_delete(Items, item);
    return copy(Items);
}

mapping SetItems(mapping items){
    foreach(mixed key, mixed val in items){
        AddItem(key, val);
    }
    return copy(Items);
}

//TMI2 compat hack
mapping SetItem_desc(mapping items){
    return SetItems(items);
}

varargs string GetLong(string str){
    if( str && Items[str] ){
        return GetItem(str);
    }
    else {
        return this_object()->GetExternalDesc();
    }
}

string SetLong(string str){
    return SetExternalDesc(str);
}

varargs mixed eventShow(object who, string component){
    mixed desc;

    if( component ){
        component = remove_article(lower_case(component));
        desc = GetItem(component, who);
        environment(who)->eventPrint(who->GetName() + " looks at the " +
                component + " on the " + remove_article(GetShort()) + ".",
                ({ who, this_object() }));
    }
    else {
        desc = GetExternalDesc(who);
        environment(who)->eventPrint(who->GetName() + " looks at the " +
                remove_article(GetShort()) + ".",
                ({ who, this_object() }));
    }

    if((!inherits(LIB_SIT,this_object()) && !inherits(LIB_LIE,this_object())) ||
            (!sizeof(this_object()->GetLiers()) && !sizeof(this_object()->GetSitters()))){
        if(inherits(LIB_SURFACE,this_object()) ||
                this_object()->GetOpacity() < 33){
            (functionp(desc) ? evaluate(desc) : who->eventPrint(desc));
            this_object()->eventShowInterior(who);
        }
        else (functionp(desc) ? evaluate(desc) : who->eventPrint(desc));
    }
    else (functionp(desc) ? evaluate(desc) : who->eventPrint(desc));
    return 1;
}

mixed direct_look_obj(){
    object env = environment();
    if(!this_object()->GetInvis()){
        if( env != this_player() && env != environment(this_player()) ){
            return "#Perhaps \"look at "+this_object()->GetKeyName()+
                " on\" something?"; 
        }
    }
    return 1;
}

mixed direct_look_at_obj(){
    return direct_look_obj();
}

mixed direct_look_on_obj(){
    return direct_look_obj();
}

mixed direct_look_at_obj_on_obj(object target, object ob,mixed arg, mixed arg2){
    if(!ob) ob=environment(target);
    if((inherits(LIB_SIT,ob) && sizeof(ob->GetSitters())) ||
            (inherits(LIB_LIE,ob) && sizeof(ob->GetLiers()))){
        write("There appears to be someone blocking your view.");
    }

    if(ob->GetInvis()){
        return "#There is no "+arg+" on "+arg2+" here.";
    }

    if(!target->GetInvis()){
        if((inherits(LIB_SURFACE,ob) || living(ob)) && 
                environment(target) == ob){
            if(this_player()->GetEffectiveVision() == VISION_CLEAR){
                if(living(target)) return target->GetLong();
                else return target->GetExternalDesc();
            }
            else return "#You can't quite make out its details.";
        }
    }
    if( environment(target) != ob || target->GetInvis() ){
        return "#There is no " + arg + " on " + ob->GetShort() + ".";
    }
    return 1;
}

mixed direct_look_at_obj_word_obj(){
    return direct_look_obj();
}

mixed direct_look_at_str_on_obj(string str, object target){
    object dingus;
    str = remove_article(lower_case(str));
    if((inherits(LIB_SIT,target) && sizeof(target->GetSitters())) ||
            (inherits(LIB_LIE,target) && sizeof(target->GetLiers()))){
        write("There appears to be someone blocking your view.");
        return 0;
    }

    if((inherits(LIB_SURFACE,target) || living(target)) && dingus = present(str, target)){
        if(this_player()->GetEffectiveVision() == VISION_CLEAR){
            return dingus->GetExternalDesc();
        }
        else return "#You can't quite make out its details.";
    }
    if( !Items[str] ){
        return "#There is no " + str + " on " + GetShort() + ".";
    }
    return 1;
}
