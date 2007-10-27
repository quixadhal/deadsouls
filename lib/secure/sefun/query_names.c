#include <lib.h>

string *query_names(object whom) {
    string *name = ({});
    string *names = ({});
    if(!whom) return name;
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
            if(arrayp(subst)) names += subst;
            else names += ({ lower_case(subst) });
        }
    }
    return names;
}

int answers_to(string name, object what){
    string *adjs = what->GetAdjectives();
    string *preargs = ({});
    string s1,s2,s3,s4;
    int hits;
    if(member_array(lower_case(name),query_names(what)) != -1) return 1;
    if(!sizeof(adjs)) return 0;
    hits = sscanf(name,"%s %s %s %s",s1, s2, s3, s4);
    if(hits < 4) hits = sscanf(name,"%s %s %s",s1, s2, s3);
    if(hits < 3) hits = sscanf(name,"%s %s",s1, s2);
    if(!hits) return 0;
    hits--;
    if(sizeof(s1)) preargs += ({s1});
    if(sizeof(s2)) preargs += ({s2});
    if(sizeof(s3)) preargs += ({s3});
    if(sizeof(s4)) preargs += ({s4});

    if(member_array(preargs[hits],query_names(what)) == -1) return 0;
    preargs -= ({ preargs[hits] });

    foreach(string prearg in preargs){
        if(member_array(prearg, adjs) == -1) return 0;
    }
    return 1;
}
