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
