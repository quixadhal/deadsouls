#include <lib.h>
#include <dirs.h>

inherit LIB_SHADOW;

object me = this_object();
string *shadowed_id = ({});

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

string *GetId(){
//string *ret = this_object()->GetId();
//ret += ({ "bear", "brown bear" });
return shadowed_id;
}

string GetRace(){
return "bear";
}

int enshadow(object whom){
if(base_name(previous_object()) == "/domains/town/armor/bearsuit"){
shadowed_id = whom->GetId()+ ({ "bear", "brown bear" });
shadow(whom);
return 1;
}
return 0;
}

int unbearshadow(){
destruct(me);
}



