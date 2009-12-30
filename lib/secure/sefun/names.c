#include <daemons.h>
#include <lib.h>

#ifndef __FLUFFOS__
string base_name(mixed val) {
    string name, base;
    int borg;
    if(!val) return "";
    if(objectp(val)) name = file_name(val);
    else if(val == "") return "";
    else if(stringp(val) && sizeof(val)) name = val;
    else name = file_name(val);
    if(sscanf(name, "%s#%d", base, borg) ==2) return base;
    else return name;
}
#endif

string convert_name(string str) {
    string a, b, ret;
    if(!str || str == "") return "";
    if(!grepp(str,"@")){
        str = replace_string(str, " ", "");
        str = replace_string(str, "'", "");
        return lower_case(replace_string(str, "-", ""));
    }
    if(sscanf(str, "%s@%s", a, b) == 2 &&
            ret = INTERMUD_D->GetMudName(b)) return a+"@"+ret;
    else return str; 
}

string *query_names(object whom) {
    string *name = ({});
    string *names = ({});
    if(!whom || !objectp(whom)) return name;
    if(inherits(LIB_DOOR,whom) || base_name(whom) == LIB_DOOR) 
    {
        name = ({ "door" });
    }
    else {
        if(whom->GetName()) name += ({ lower_case(whom->GetName()) });
        if(whom->GetKeyName()) name += ({ lower_case(whom->GetKeyName()) });
        if(sizeof(whom->GetId())) {
            if(arrayp(whom->GetId()))name += whom->GetId();
            else name += ({ whom->GetId() });
        }
    }
    if(sizeof(name)){
        foreach(mixed subst in name){
            if(!subst) continue;
            if(arrayp(subst)) names += subst;
            else names += ({ lower_case(subst) });
        }
    }
    return names;
}

int answers_to(string name, object what){
    string *adjs, *names, *arg_arr, *tmp_arr;
    int i, j, arrsize;
    if(!name || !what) return 0;
    if(!stringp(name) || !objectp(what)) return 0;
    adjs = filter((what->GetAdjectives() || ({})), (: lower_case(($1||"")) :));
    names = filter(query_names(what), (: lower_case($1) :));
    name = lower_case(name);
    if(member_array(name, names) != -1) return 1;
    if(!sizeof(adjs)) return 0;
    arg_arr = explode(name," ");
    arrsize = sizeof(arg_arr);
    if(arrsize == 1) return 0;
    tmp_arr = arg_arr;
    for(i=0; i < arrsize-1; i++){
        string *elision = ({});
        for(j=i; j < arrsize-1; j++){
            string current_adj = implode(arg_arr[i..j]," ");
            if(member_array(current_adj,adjs) != -1){
                elision = arg_arr[i..j];
                break;
            }
        }
        if(!sizeof(elision)){
            return 0;
        }
        tmp_arr -= elision;
        if(!sizeof(tmp_arr)) return 0;
        if(member_array(implode(tmp_arr," "), names) != -1) return 1;
    }
    return 0;
}

varargs string query_unique_name(mixed ob, int cloned){
    string fn;
    if(objectp(ob)){
        if(!clonep(ob)) cloned = 0;
        if(cloned) fn = file_name(ob);
        else fn = base_name(ob);
    }
    else if(stringp(ob)) fn = ob;
    else return 0;
    fn = replace_string(fn, "/", "0^0");
    fn = replace_string(fn, "#", "9^9");
    return fn;
}
