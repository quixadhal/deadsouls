int query_carrying(object who, mixed thing){
    if(objectp(thing)) thing = base_name(thing);
    if(last(thing,2) == ".c") truncate(thing,2);

    if(!file_exists(thing+".c")) return 0;

    foreach(object ob in deep_inventory(who)){
        if(base_name(ob) == thing) return 1;
    }
    return 0;
}

object *containers(object ob){
    object *ret =({});
    if(!ob) ob = previous_object();
    while(environment(ob)){
        ret += ({ environment(ob) });
        ob = environment(ob);
    }
    return ret;
}
