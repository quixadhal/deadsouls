/*    /daemon/classes.c
 *    from the Dead Souls  Object Library
 *    handles race configuration and administration
 *    created by Descartes of Borg 960929
 *    Version: @(#) classes.c 1.4@(#)
 *    Last modified: 96/10/17
 */

#include <lib.h>
#include <cfg.h>
#include <save.h>
#include <daemons.h>
#include <privs.h>
#include "include/classes.h"

inherit LIB_DAEMON;

private mapping Classes = ([]);
private int converted = 0;
static string SaveFile;
static int player, foo;

static void create() {
    daemon::create();
    SaveFile = save_file(SAVE_CLASSES);
    if( unguarded((: file_exists(SaveFile) :)) ){
        RestoreObject(SaveFile);
    }
    if(!Classes) Classes = ([]);
    if(!converted || !sizeof(Classes)){
        string *classes = get_dir(CFG_CLASSES+"/");
        Classes = ([]);
        foreach(string element in classes){
            string str = CFG_CLASSES+"/"+element;
            if(file_exists(str)) {
                catch( this_object()->AddClass(str) );
            }
        }
    }
    converted = 1;
    SaveObject(SaveFile);
}

static private void validate() {
    if( !(master()->valid_apply(({ PRIV_ASSIST }))) )
        error("Illegal attempt to modify class data");
}

int ClassMember(string my_class, string query_class) {
    mapping cls;

    if( my_class == query_class ) {
        return 1;
    }
    cls = Classes[query_class];
    if( !cls ) { // query class is a multi-class, thus must be equal to mine
        return 0; // This should change for triple classing
    }
    return (member_array(my_class, values(cls["Multis"])) != -1);
}

void AddClass(string file) {
    mapping cls = ([]);
    string array lines, tmp;
    string class_name;
    player = 1;
    validate();
    if( !file_exists(file) ) error("No such file: " + file);
    lines = explode(read_file(file), "\n");
    lines = filter(lines, function(string str) {
            if( strlen(str) == 0 ) {
            return 0;
            }
            if( str[0] == '#' ) {
            return 0;
            }
            if( str[0] == ' ' || str[0] == '\t' ) {
            return 0;
            }
            if(!strsrch(str, "PLAYER_CLASS")){
            if(sscanf(str, "PLAYER_CLASS %d", foo)){
            if(!foo) player = 0;
            }
            return 0;
            } 
            return 1;
            });
    class_name = lines[0];
    if( Classes[class_name] ) error("Class already exists");
    Classes[class_name] = cls;
    lines = lines[1..];
    cls["Multis"] = ([]);
    cls["Player"] = player;
    while( sizeof(tmp = explode(lines[0], ":")) == 2 ) {
        cls["Multis"][tmp[0]] = tmp[1];
        lines = lines[1..];
    }
    cls["Skills"] = ([]);
    while(sizeof(tmp = explode(lines[0], ":")) == 3) {
        mapping s = ([]);
        s["Average"] = to_int(tmp[2]);
        s["SkillClass"] = to_int(tmp[1]);
        cls["Skills"][tmp[0]] = s;
        SKILLS_D->SetSkill(tmp[0], class_name, tmp[1]);
        if( sizeof(lines) == 1 ) {
            lines = ({});
            break;
        }
        else {
            lines = lines[1..];
        }
    }
    cls["Complete"] = 1;
    SaveObject(SaveFile);
}

void RemoveClass(string class_name) {
    validate();
    map_delete(Classes, class_name);
    SaveObject(SaveFile);
}

void SetClass(string class_name, mixed array args) {
    mapping cls = Classes[class_name];
    mixed array primes, ots;
    if( !cls || !cls["Complete"] || sizeof(args) != 3 ){
        return;
    }
    args[0] = cls["Multis"];
    primes = ({});
    ots = ({});
    foreach(string key, mapping skill in cls["Skills"]) {
        ots = ({ots..., ({ key, skill["Average"], skill["SkillClass"] })});
    }
    args[1] = ots;
}

void SetComplete(string class_name) {
    mapping cls;
    validate();
    if( !Classes[class_name] ) error("No such class");
    else cls = Classes[class_name];
    cls["Complete"] = 1;
    SaveObject(SaveFile);
}

varargs string *GetClasses(int player) {
    string *ret;
    if(player){
        ret = filter(keys(Classes), (: Classes[$1]["Complete"] &&
                    Classes[$1]["Player"] :));
    }
    else ret = filter(keys(Classes), (: Classes[$1]["Complete"] :));
    return ret;
}

mixed GetClass(string str){
    return copy(Classes[str]);
}

int GetPlayerClass(string str){
    if(!Classes[str]) return ;
    return Classes[str]["Player"];
}

string GetHelp(string class_name) {
    mapping cls = Classes[class_name];
    string help = "Class: " + class_name + "\n\n";

    if( !cls ) return 0;
    if( !sizeof(cls["Multis"]) ) {
        help += capitalize(class_name) + " cannot multi-class.\n";
    }
    else {
        help += capitalize(pluralize(class_name)) + " can multi-class with " +
            "the following primary classes:\n";
        foreach(string prime, string other in cls["Multis"]) {
            help += "\t" + capitalize(class_name) + " + " + prime + " = " +
                other +  "\n";
        }
    }
    help += "\n" + capitalize(pluralize(class_name)) + " has the following " +
        "primary skills:\n";
    foreach(string skill, mapping s in cls["Skills"]) {
        if( s["SkillClass"] == 1 ) {
            help += "\t" + skill + "\n";
        }
    }
    return help + "\n";
}
