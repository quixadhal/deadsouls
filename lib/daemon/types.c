#include <lib.h>
#include <save.h>
#include <armor_types.h>
#include <body_types.h>
#include <boobytraps.h>
#include <build_types.h>
#include <damage_types.h>
#include <graphics.h>
#include <meal_types.h>
#include <message_class.h>
#include <mouth_types.h>
#include <position.h>
#include <respiration_types.h>
#include <size_types.h>
#include <terrain_types.h>
#include <vendor_types.h>

#include <assessment.h>
#include <climb.h>
#include <jump.h>
#include <magic.h>
#include <medium.h>
#include <motion.h>
#include <rounds.h>
#include <soul.h>
#include <talk_type.h>
#include <vision.h>
#include <parser_error.h>

inherit LIB_DAEMON;

string *types = ({ "vendor_types", "terrain_types", "size_types",
        "respiration_types", "position", "mouth_types", "message_class",
        "meal_types", "graphics", "damage_types", "build_types",
        "boobytraps", "body_types", "armor_types" });

string *configs = ({ "assessment", "climb", "jump",
        "magic", "medium", "motion", "rounds",
        "soul", "talk_type", "vision", "parser_error" });

mapping Types = ([]);

int eventReadTypes();
int eventReadConfigs();

static void create() {
    ::create();
    SetSaveFile(SAVE_TYPES);
    if(!Types) Types = ([]);
    call_out((: eventReadTypes :), 10);
    call_out((: eventReadConfigs :), 10);
}

int eventReadTypes(){
    if(!Types) Types = ([]);
    foreach(string file in types){
        string *tmp_arr;
        string tmp,s1,s2,s3,s4;
        int d1,d2,d3, i, j;
        if(!Types[file]) Types[file] = ([]);
        tmp = read_file("/include/"+file+".h");
        if(!tmp) tmp = read_file("/secure/include/"+file+".h");
        tmp_arr = explode(tmp,"\n");
        foreach(string line in tmp_arr){
            i = sscanf(line,"#define %s (%s<<%s)",s1,s2,s3,s4);
            if( i != 4) j = sscanf(line,"#define %s (%s<<%s)",s1,s2,s3);
            if(i != 4 && j != 3) continue;
            s1 = trim(s1);
            s2 = trim(s2);
            s3 = trim(s3);
            d1 = atoi(s2);
            d2 = atoi(s3);
            d3 = ( d1 << d2 );
            Types[file][d3] = s1;
        }
    }
    eventSave();
    return 1;
}

int eventReadConfigs(){
    if(!Types) Types = ([]);
    foreach(string file in configs){
        string *tmp_arr;
        string tmp,s1,s2,s3;
        int d1, i, j;
        if(!Types[file]) Types[file] = ([]);
        tmp = read_file("/include/"+file+".h");
        if(!tmp) tmp = read_file("/secure/include/"+file+".h");
        tmp_arr = explode(tmp,"\n");
        foreach(string line in tmp_arr){
            line = trim(line);
            i = sscanf(line,"#define %s %s %s)",s1,s2,s3);
            if( i != 3) j = sscanf(line,"#define %s %s",s1,s2);
            if(i != 3 && j != 2) continue;
            s1 = trim(s1);
            s2 = trim(s2);
            d1 = atoi(s2);
            Types[file][d1] = s1;
        }
    }
    eventSave();
    return 1;
}

varargs mixed GetTypes(string str){
    if(!str) return Types + ([]);
    if(Types[str]) return Types[str] + ([]);
    else return 0;
}

mixed eventCalculateTypes(string typename, int type){
    string *ret_arr = ({});
    string ret = "";

    foreach(string canonical_name in keys(Types)){
        if(grepp(canonical_name, typename)){
            typename = canonical_name;
            break;
        }
    }
    if(!Types[typename]) return 0;

    if(member_array(typename,types) != -1){
        foreach(int key, string val in Types[typename]){
            if(type & key) ret_arr += ({ Types[typename][key] });
        }
        return ret_arr;
    }

    foreach(int key, string val in Types[typename]){
        if(type == key) ret = Types[typename][key];
    }
    return ret;
}

