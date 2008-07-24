// handling of duplicate objects in inventory. 
//
// here we provide the function two objects. If they
// were cloned from the same file AND they have
// different instance numbers, we return "1", because
// they are duplicates, and not just one object. Otherwise,
// we return a "0". Note that 0 can mean the two objects
// were cloned from different files, and it can also mean
// that the objects provided are actually just one object
// in memory.
//
int query_dupes(object ob1,object ob2){
    if(base_name(ob1) == base_name(ob2) && file_name(ob1) != file_name(ob2)) return 1;
    else return 0;
}

// This function is supposed to return an array
// of objects in the inventory of "where" that 
// are cloned from the same file as "ob"
object *get_dupes(object ob, object where){
    object *allstuff,*dupes;
    int i;
    if(!where || !ob) {
        //write("missing something");
        return 0;
    }
    allstuff = all_inventory(where);
    if(!sizeof(allstuff)) return 0;
    if(member_array(ob,allstuff) == -1) return 0;
    for(i=0;i<sizeof(allstuff);i++){
        if( query_dupes(allstuff[i],ob) == 1 && !sizeof(dupes) ) dupes = ({allstuff[i]});
        if( query_dupes(allstuff[i],ob) == 1 && sizeof(dupes) > 0 ) dupes += ({allstuff[i]});
    }
    if(sizeof(dupes) > 0) return dupes;
    else return ({});
}
