/*    /adm/simul_efun/domains.c
 *    from Dead Souls
 *    sefuns for managing domains
 *    created by Descartes of Borg 931204
 */

#include <dirs.h>

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

string domain(mixed val) {
    string nom, tmp;

    if(objectp(val) && domain_exists(tmp=(string)val->query_domain())) 
      return tmp;
    else if(stringp(val) && val=load_object(val) &&
      domain_exists(tmp=(string)val->query_domain())) return tmp;
    nom = (objectp(val) ? file_name(val) : (string)val);
    if(sscanf(nom, DOMAINS_DIRS+"/%s/%*s", tmp) == 1) return tmp;
    if(sscanf(nom, REALMS_DIRS+"/%*s/%s/%*s", tmp) == 1 &&
      domain_exists(tmp)) return tmp;
    return 0;
}
