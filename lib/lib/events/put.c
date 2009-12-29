/*    /lib/put.c
 *    from the Dead Souls Mud Library
 *    handles things which may be put into other objects
 *    created by Descartes of Borg 960114
 */

#include <lib.h>

private mixed PreventPut;

// abstract methods
string GetShort();
mixed CanDrop(object who);
mixed eventMove(mixed dest);
// end abstract methods

mixed SetPreventPut(mixed val){
    return (PreventPut = val);
}

mixed GetPreventPut(){
    return PreventPut;
}

varargs mixed CanPut(object who, object what){
    mixed tmp;
    object env;

    if(what) env = environment(what);
    if(!env || env != this_player()){
        return "#You don't have that.";
    }
    if( (tmp = CanDrop(who)) != 1 ) return tmp;
    if( !environment() ){ destruct(this_object()); return 1; }
    if( environment() != this_player() &&
            environment() != environment(this_player())) return 0;
    if( !PreventPut ) return 1;
    if( stringp(PreventPut) && PreventPut == "PERMIT" ) return 1;
    if( intp(PreventPut) ) return 0;
    if( stringp(PreventPut) ) return PreventPut;
    if( objectp(PreventPut) ){
        if( PreventPut == who )
            return "You cannot put " + GetShort() + " anywhere.";
        else return 1;
    }
    else if(functionp(PreventPut)) return evaluate(PreventPut, who);
    else {
        return "It seems you're unable to do that right now.";
    }
}

varargs mixed eventPut(object who, object storage, string prep){
    int depth;
    if(!prep || prep == "") prep = " into ";
    if(prep == " onto " && !inherits( LIB_SURFACE, previous_object() ) ){
        who->eventPrint("That isn't a load-bearing surface.");
        return 0;
    }

    if(prep == " into " && inherits( LIB_SURFACE, previous_object() ) ){
        who->eventPrint("That's a surface. Try \"put on\"");
        return 0;
    }

    if((inherits(LIB_SIT,storage) && sizeof(storage->GetSitters())) ||
            (inherits(LIB_LIE,storage) && sizeof(storage->GetLiers()))){
        write("There appears to be someone blocking your access.");
        return 0;
    }


    if( !eventMove(storage) ){
        who->eventPrint("The "+remove_article(this_object()->GetShort())+" stays where it is.");
        return 0;
    }
    who->eventPrint("You put " + GetShort() + prep +
            storage->GetShort() + ".");
    environment(who)->eventPrint(who->GetName() + " puts " +
            GetShort() + prep +
            storage->GetShort() + ".", who);
    if(inherits(LIB_STORAGE, this_object())){
        depth = this_object()->GetRecurseDepth();
        if(depth && inherits(LIB_STORAGE, storage)) storage->AddRecurseDepth(depth); 
    }

    return 1;
}

static void create(){
    PreventPut = 0;
}

mixed direct_put_obj_word_obj(){
    return CanPut(this_player(), this_object());
}

mixed direct_put_wrd_wrd_word_obj(){
    return CanPut(this_player(), this_object());
}

mixed direct_put_obj_obj(){
    return direct_put_obj_word_obj();
}
