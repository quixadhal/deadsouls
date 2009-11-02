/*    /adm/simul_efun/domains.c
 *    from Dead Souls
 *    sefuns for managing domains
 *    created by Descartes of Borg 931204
 */

#include <dirs.h>
private string gnom;

object domain_master(mixed val) {
    if(!val) return 0;
    if(stringp(val)) {
        if(!domain_exists(val)) return 0;
        else return load_object(DOMAINS_DIRS+"/"+val+"/adm/master");
    }
    else if(objectp(val)) {
        if(val = domain(val)) 
            return load_object(DOMAINS_DIRS+"/"+val+"/adm/master");
        else return 0;
    }
    error("Bad argument 1 to load_object().\n");
}

int domain_exists(string dmn) {
    if(!stringp(dmn)) return 0;
    return (file_size(DOMAINS_DIRS+"/"+dmn+"/") == -2);
}

varargs string domain(mixed val, int path) {
    string nom, tmp, foo;
    int hits;

    if(objectp(val) && domain_exists(tmp=val->GetDomain())){ 
        if(!path) return tmp;
        if(val->GetDomainPath()) return val->GetDomainPath();
        else return tmp;
    }
    if(stringp(val) && val=load_object(val) &&
            domain_exists(tmp=val->GetDomain())){
        if(!path) return tmp;
        if(val->GetDomainPath()) return val->GetDomainPath();
        else return tmp;
    }
    nom = (objectp(val) ? file_name(val) : val);
    if((hits = (sscanf(nom, DOMAINS_DIRS+"/%s/%*s", tmp)))){
        if(path) return DOMAINS_DIRS+"/"+tmp;
        return tmp;
    }
    gnom = nom;
    if((hits = sscanf(nom, REALMS_DIRS+"/%s/%s/%*s", foo, tmp)) > 1 &&
            unguarded( (: directory_exists(path_prefix(gnom)) :)) ){
        string ret = foo+"/"+tmp;
        if(path) return REALMS_DIRS+"/"+ret;
        return ret;
    }
    if((hits = sscanf(nom, ESTATES_DIRS+"/%s/%s/area/%*s", foo, tmp)) > 1 &&
            unguarded( (: directory_exists(path_prefix(gnom)) :) )){
        if(path) return ESTATES_DIRS+"/"+foo+"/"+tmp+"/area";
        return tmp;
    }
    return 0;
}
