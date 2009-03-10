/*    /secure/sefun/expand_keys.c
 *    from the Dead Souls Mud Library
 *    takes a mapping with array keys and flattens the arrays
 *    created by Descartes of Borg 951013
 */

mapping expand_keys(mapping mp) {
    mapping tmp = ([]);
    mixed key, val;

    foreach(key, val in mp) {
        if( arrayp(key) ) {
            string elem;

            foreach(elem in key) tmp[elem] = val;
        }
        else tmp[key] = val;
    }
    return tmp;
}
