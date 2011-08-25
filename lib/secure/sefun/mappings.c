string flat_map(mapping mp) {
    mapping carte = ([]);
    mixed key, val;
    string retkey, retval, retstring;

    retstring = "";

    carte = expand_keys(mp);

    foreach( key, val in carte){
        if(arrayp(key)){
            if(!sizeof(key)) retkey = "BLANK ARRAY";
            else retkey = "ARRAY ("+implode(key,", ")+")";
        }
        else if(objectp(key)) retkey = "OBJECT ("+file_name(key)+")";
        else if(intp(key)) retkey = ""+key;
        else retkey = key;
        if(arrayp(val)){
            if(!sizeof(val)) retval = "BLANK ARRAY";
            else retval = "ARRAY ("+implode(val,", ")+")";
        }
        else if(objectp(val)) retval = "OBJECT ("+file_name(val)+")";
        else if(intp(val)) retval = ""+val;
        else retval = val;

        if(!retstring || retstring == "") retstring = retkey+":"+retval;
        else retstring += ", "+retkey+":"+retval;
    }

    return retstring;

}

varargs mapping add_maps(mapping FirstMapping,mapping  SecondMapping, int preference){
    mapping OutMap = ([]);
    mapping FirstMap = copy(FirstMapping);
    mapping SecondMap = copy(SecondMapping);

    foreach(mixed key, mixed val in (FirstMap || ([]))){
        OutMap[copy(key)] = copy(val);
    }

    foreach(mixed key, mixed val in (SecondMap || ([]))){
        OutMap[copy(key)] = copy(val);
    } 

    return OutMap;
}

int mgrepp(string primary, string *sub){
    foreach(string element in sub){
        if(strsrch(primary,element) != -1) return 0;
    }
    return 1;
}

mixed mapping_member(mapping Map, string sub){
    foreach(mixed key, mixed val in Map){
        if(stringp(key) && key == sub) return key;
        else if(arrayp(key) && member_array(sub, key) != -1) return key;
    }
    return 0;
}




