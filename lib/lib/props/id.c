/*    /lib/props/id.c
 *    From the Dead Souls Mud Library
 *    Attributes which identify objects
 *    Created by Descartes of Borg 961222
 *    Version: @(#) id.c 1.2@(#)
 *    Last modified: 96/12/22
 */

#include <lib.h>
#include <daemons.h>

private static string array Adjectives   = ({});
private string              CapName      = 0;
private static string array Id           = ({});
private static string array CanonicalId  = ({});
private static string array ExcludedIds  = ({});
private static string       KeyName      = 0;
private static int          Matching     = 1;
private static object array NotifiedObjects = ({});
private static mixed array  rfn          = ({});

string GetKeyName();

string array GetAdjectives(){
    return Adjectives;
}

varargs string array SetAdjectives(mixed adjs...){
    if( stringp(adjs) ){
        adjs = ({ adjs });
    }
    else if( !arrayp(adjs) ){
        error("Bad argument 1 to SetAdjectives().\n");
    }
    Adjectives = ({});
    foreach(mixed val in adjs){
        if( arrayp(val) ){
            Adjectives += val;
        }
        else {
            Adjectives += ({ val });
        }
    }
    parse_refresh();
    return Adjectives;
}

string GetCapName(){
    return CapName;
}

string SetCapName(string str){
    if( !stringp(str) ){
        error("Bad argument 1 to SetCapName().\n");
    }
    return (CapName = str);
}

string array GetId(){
    string tmp;

    if(Id && sizeof(Id)) return Id;

    tmp = GetKeyName();
    if( tmp ){
        if(!OBJECT_MATCHING || !Matching) 
            return distinct_array(({ CanonicalId..., tmp }));
        else return Id + atomize_string(tmp) - ExcludedIds;
    }
    else return Id;
}

string GetUniqueId(){
    string fn = file_name(this_object());
    rfn = reg_assoc(fn, ({ "[A-Za-z0-9]+" }), ({ 1 }), 0);
    fn = implode(filter(rfn[0], (: rfn[1][member_array($1,rfn[0])] :) ), "*");
    return fn;
}

string array GetCanonicalId(){
    string tmp;
    tmp = GetKeyName();

    return copy(CanonicalId);
}

varargs string array SetId(mixed val...){
    string tmp, fn, gs;
    string *exclude = ({});
    if(!(MASTER_D->GetPerfOK())){
        Matching = 0;
    }
    if( stringp(val) ){
        val = ({ val });
    }
    else if( !arrayp(val) ){
        error("Bad argument 1 to SetId().\n");
    }
    Id = ({});
    foreach(mixed id in val){
        if( stringp(id) ){
            Id = ({ Id..., id });
        }
        else if( arrayp(id) ){
            Id = ({ Id..., id... });
        }
    }
    if(gs = this_object()->GetShort()){
        exclude = ({ lower_case(gs) });
        exclude += ({ lower_case(remove_article(gs)) });
        Id += exclude;
    }
    if(tmp = this_object()->GetRace()){
        Id += ({ tmp });
    }
    if(tmp = this_object()->GetKeyName()){
        Id += ({ tmp });
    }
    if(clonep(this_object())){
        fn = GetUniqueId();
        exclude += ({ fn });
        Id += ({ fn });
    }

    if(COMPAT_MODE) parse_init();
    parse_refresh();

    CanonicalId = Id;

    if(OBJECT_MATCHING && Matching){
        if(!KeyName || !sizeof(KeyName)) KeyName = Id[0];
        Id = distinct_array(atomize_array(Id - exclude) + Id); 
    }
    return Id;
}

string GetKeyName(){
    return KeyName;
}

string SetKeyName(string nom){
    if( !stringp(nom) ){
        error("Bad argument 1 to SetKeyName().\n");
    }
    KeyName = lower_case(nom);
    if( !CapName ){
        CapName = capitalize(nom);
    }
    return KeyName;
}

string GetName(){
    return GetCapName();
}

int id(string str){
    int ret;
    if( !stringp(str) ){
        return 0;
    }
    str = lower_case(str);
    ret = (member_array(str, this_object()->GetId()) != -1);
    if(!ret && OBJECT_MATCHING && answers_to(str,this_object())) ret = 1;
    return ret;
}

string array parse_command_id_list(){
    string array ids = (this_object()->GetId() || ({}));
    return filter(ids, (: stringp($1) && ($1 != "") :));
}

string array parse_command_plural_id_list(){
    string array ids = (this_object()->GetId() || ({}));

    ids = filter(ids, (: stringp($1) && ($1 != "") :));
    return map(ids, (: pluralize :));
}

string array parse_command_adjectiv_id_list(){
    return filter(this_object()->GetAdjectives(), (: $1 && ($1 != "") :));
}

varargs void eventAnnounceCanonicalId(object env){
    object *inv;
    if(!OBJECT_MATCHING || !Matching) return;
    if(!env) env = environment();
    if(!env) return;
    if(environment(env)) env = environment(env);
    inv = deep_inventory(env) - ({ this_object() });
    if(sizeof(inv) > 25) return;
    inv->ReceiveCanonicalId(CanonicalId);
    inv = all_inventory(this_object());
    if(inv && sizeof(inv)) inv->eventAnnounceCanonicalId(env);
}


varargs void ReceiveCanonicalId(mixed foo, int leaving){
    if(!OBJECT_MATCHING || !Matching) return;
    if(!foo || !sizeof(foo)) return;
    if(!leaving){
        foreach(mixed element in foo){
            if(member_array(element, this_object()->GetId()) != -1){
                if(member_array(element, CanonicalId) == -1){
                    if(sizeof(ExcludedIds) < 1024)
                        ExcludedIds += ({ element });
                    parse_init();
                    parse_refresh();
                }
            }
        }
    }
    else {
        foreach(mixed element in foo){
            ExcludedIds -= ({ element });
            parse_init();
            parse_refresh();
        }
    }
    if(previous_object() != this_object()){
        if(member_array(previous_object(), NotifiedObjects) == -1){
            NotifiedObjects += ({ previous_object() });
            previous_object()->ReceiveCanonicalId(CanonicalId);
        }
    }
}

//This is for explicitly enabling or disabling
//object matching. For some items, object matching is
//really inconvenient.

int SetMatching(int i){
    if(!i) Matching = 0;
    else Matching = 1;
    return Matching;
}

int GetMatching(){
    return Matching;
}
