string *query_names(object whom) {
    string *name = ({});
    string *names = ({});
    if(!whom) return name;
    if(whom->GetName()) name += ({ lower_case(whom->GetName()) });
    if(whom->GetKeyName()) name += ({ lower_case(whom->GetKeyName()) });
    if(sizeof(whom->GetId())) {
	if(arrayp(whom->GetId()))name += whom->GetId();
	else name += ({ whom->GetId() });
    }
    if(sizeof(name)){
	foreach(string subst in name){
	    names += ({ lower_case(subst) });
	}
    }
    return names;
}

int answers_to(string name, object what){
    if(member_array(name,query_names(what)) != -1) return 1;
    else return 0;
}
