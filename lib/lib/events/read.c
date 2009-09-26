#include <function.h>
#include <message_class.h>

static private string Language;
static private mixed Read    = 0;
static private mapping Reads = ([]);

private string tmpfile;

// abstract methods
string GetShort();
// end abstract methods

string GetRead(string str){
    if( !str || str == "default" ){
        if(Reads["default"]) return Reads["default"];
        else return Read;
    }
    else {
        return Reads[str];
    }
}

string array GetReads(){
    return keys(Reads);
}

mapping GetReadsMap(){
    mapping ret = (Reads || ([]));
    if(stringp(Read)) ret["default"] = Read;
    else if(mapp(Read) && Read["default"]) ret["default"] = Read["default"];
    return ret;
}

void RemoveRead(string item){
    if( !item || item == "default" ){
        Reads["default"] = 0;
        Read = 0;
    }
    else {
        map_delete(Reads, item);
    }
    return;
}

varargs mixed SetRead(mixed arg1, mixed desc){
    if( mapp(arg1) ){
        Reads = expand_keys(arg1);
        if( Reads["default"] ){
            Read = Reads["default"];
        }
    }
    if( !desc ){
        Read = arg1;
        return Read;
    }
    if( !arg1 || arg1 == "default" ){
        Read = desc;
        return Read;
    }
    if( arrayp(arg1) ){
        foreach(string element in arg1){
            Reads[element] = desc;
        }
    }
    else {
        Reads[arg1] = desc;
    }
    return Reads;
}

void SetReads(mapping ReadMap){
    foreach(mixed key, mixed val in ReadMap){
        SetRead(key, val);
    }
}

varargs mixed SetDefaultRead(mixed arg1, mixed desc){
    if( mapp(arg1) ){
        return 0;
    }
    if( !desc ){
        Read = arg1;
        return Read;
    }
    if( !arg1 || arg1 == "default" ){
        Read = desc;
        return Read;
    }
}

int SetLanguage(string str){
    if(str) Language = str;
    return 1;
}

mixed GetLanguage(){
    if(Language) return Language;
    else return 0;
}

varargs mixed eventRead(object who, mixed str){
    mixed ret;
    mixed val;
    if(str) val = GetRead(str);
    else val = GetRead("default");
    if( arrayp(val) ){
        val = val[query_night()];
    }
    if(mapp(val)) val = val[str];

    if( functionp(val) ){
        if( functionp(val) & FP_OWNER_DESTED ){
            who->eventPrint("There was a problem with the read.");
            return 1;
        }
        ret = evaluate(val, str);
        if(!stringp(ret)) return 1;
    }
    environment(who)->eventPrint(who->GetName() + " reads " + GetShort() + ".",
            who);
    if(ret) val = ret;
    if( !val ){
        who->eventPrint("There is nothing to read.");
        return 1;
    }

    if(Language){
        write("The language appears to be "+capitalize(Language)+".");
    }

    if(!val){
        write("You can't read that.");
        return 0;
    } 

    if(Language && (this_player()->GetLanguageLevel(Language) < 100 &&
                !(this_player()->GetPolyglot()))){
        if(sizeof(val) > 4800){
            val = "It is too long and you are too unfamiliar with the language to make sense of it.";

        }
        else {
            val = translate(val, this_player()->GetLanguageLevel(Language));
        }
    }

    who->eventPage(explode(val,"\n") +({""}));
    return 1;
}

mixed direct_read_obj(){
    if( !Read ){
        return 0;
    }
    else {
        if( environment() != this_player()  && environment(this_player()) !=
                environment()){
            return "#You don't have that!";
        }
        else return 1;

    }
}

mixed direct_read_str_word_obj(string str){
    str = remove_article(lower_case(str));
    if( !Reads[str] ){
        return 0;
    }
    else {
        if( environment() != this_player()  && environment(this_player()) !=
                environment()){
            return "#You don't have that!";
        }
        else return 1;

    }
}

mixed direct_read_obj_at_obj(object reader, object readee){
    if( !Read ){
        return 0;
    }
    else {
        if( environment() != this_player()  && environment(this_player()) !=
                environment()){
            return "#You don't have that!";
        }
        else return 1;

    }
}
