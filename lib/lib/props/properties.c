/*    /lib/props/properties.c
 *    From the Dead Souls V Object Library
 *    The properties property
 *    Created by Descartes of Borg 961222
 *    Version: @(#) properties.c 1.1@(#)
 *    Last modified: 96/12/22
 */

private static mapping Properties   = ([]);

mixed AddProperty(string prop, mixed val) {
    if( Properties[prop] ) {
	Properties[prop] += val;
    }
    else {
	Properties[prop] = val;
    }
    return Properties[prop];
}

mixed GetProperty(string prop) {
    return Properties[prop];
}

mapping GetProperties() {
    return Properties;
}

int RemoveProperty(string prop) {
    if( undefinedp(Properties[prop]) ) {
	return 1;
    }
    else {
	map_delete(Properties, prop);
    }
    return !Properties[prop];
}

mixed SetProperty(string prop, mixed val) {
    return (Properties[prop] = val);
}

mapping SetProperties(mapping props) {
    return (Properties += props);
}
