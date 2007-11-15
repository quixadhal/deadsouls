#include <lib.h>
private static string source;

mixed *findobs(mixed arg){
    object target;
    object *targets = ({});

    if(objectp(arg)) target = arg;
    if(stringp(arg) ) {
        if(grepp(arg,"/")){
            if(!file_exists(arg)) arg += ".c";
            if(!file_exists(arg)) return ({});
            if(!target = find_object(arg)) return ({});
        }
        else {
            foreach(object ob in objects()){
                if(answers_to(arg,ob)) targets += ({ ob });
            }
        }
    }
    if(target){
        foreach(object ob in objects()){
            if(base_name(ob) == base_name(target)) targets += ({ ob });
        }
    }
    return targets;
}

mixed find_inheritors(mixed arg){
    object *targets = ({});
    if(objectp(arg)) source = base_name(arg);
    else {
        if(!stringp(arg)) return -2;
        if(!file_exists(arg)) arg += ".c";
        if(!file_exists(arg)) return -1;
        source = truncate(arg,2);
    }
    targets = filter(objects(), (: inherits(source, $1) :) );
    if(sizeof(targets)) return targets;
    else return ({});
}

mixed find_deep_inheritors(mixed arg){
    object *targets = ({});
    if(objectp(arg)) source = base_name(arg)+".c";
    else {
        if(!stringp(arg)) return -2;
        if(!file_exists(arg)) arg += ".c";
        if(!file_exists(arg)) return -1;
        source = arg;
    }

    targets = filter(objects(), (: member_array(source, deep_inherit_list($1)) != -1 :) );
    if(sizeof(targets)) return targets;
    else return ({});
}

