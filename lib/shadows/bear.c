#include <lib.h>
#include <dirs.h>

inherit LIB_SHADOW;
inherit LIB_ID;

object me = this_object();
string *pre_shadowed_id = ({});

varargs string GetExternalDesc(object who){
    return "A large brown bear. Not as huge as a grizzly, but plenty big enough to knock your block off.";
}

varargs string GetLong(object who){
    return "A large brown bear. Not as huge as a grizzly, but plenty big enough to knock your block off.";
}

string GetName(){
    return "Bear";
}

string GetCapName(){
    return "Bear";
}

string GetKeyName(){
    return "bear";
}

string GetShort(){
    return "a bear";
}

string GetRace(){
    return "bear";
}

int eventShadow(object whom){
    object *shadows = keys(whom->GetShadows());
    if(sizeof(shadows)){
	foreach(object shade in shadows){
	    if(base_name(shade) == base_name(me)) return 0;
	}
    }
    if(base_name(previous_object()) == "/domains/town/armor/bearsuit"){
	pre_shadowed_id = whom->GetCanonicalId();
	if(!sizeof(pre_shadowed_id)) pre_shadowed_id = whom->GetId();
	::eventShadow(whom);
	SetId(pre_shadowed_id + ({ "bear", "brown bear" }));
	parse_refresh();
	return 1;
    }
    return 0;
}

string *GetId(){
    return pre_shadowed_id + ({ "bear", "brown bear" });
}

int unbearshadow(){
    SetId(pre_shadowed_id);
    eventUnshadow();
}

string array parse_command_id_list() {
    string array ids = (this_object()->GetId() || ({}));

    return filter(ids, (: stringp($1) && ($1 != "") :));
}

