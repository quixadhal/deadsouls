/*    /lib/props/properties.c
 *    From the Dead Souls Mud Library
 *    The properties property
 *    Created by Descartes of Borg 961222
 *    Version: @(#) properties.c 1.1@(#)
 *    Last modified: 96/12/22
 */

mapping Properties   = ([]);

mixed AddProperty(string prop, mixed val){
    if( Properties[prop] ){
        Properties[prop] += val;
    }
    else {
        Properties[prop] = val;
    }
    return Properties[prop];
}

    mixed GetProperty(string prop){
        if(Properties && Properties[prop])
            return Properties[prop];
        else return 0;
    }

mapping GetProperties(){
    if(!Properties) Properties = ([]);
    return Properties;
}

int RemoveProperty(string prop){
    if( undefinedp(Properties[prop]) ){
        return 1;
    }
    else {
        map_delete(Properties, prop);
    }
    return !Properties[prop];
}

mixed SetProperty(string prop, mixed val){
    if(!Properties) Properties = ([]);
    return (Properties[prop] = val);
}

mapping SetProperties(mapping props){
    if(sizeof(Properties)) return (Properties += props);
    else return (Properties = props);
}
