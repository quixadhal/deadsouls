/*    /daemon/classes.c
 *    from the Dead Souls  Object Library
 *    handles race configuration and administration
 *    created by Descartes of Borg 960929
 *    Version: @(#) classes.c 1.4@(#)
 *    Last modified: 96/10/17
 */

#include <lib.h>
#include <save.h>
#include <privs.h>
#include "include/classes.h"

inherit LIB_DAEMON;

private mapping Classes = ([]);

static void create() {
    daemon::create();
    if( unguarded((: file_size(SAVE_CLASSES __SAVE_EXTENSION__) :)) > 0 )
	unguarded((: restore_object(SAVE_CLASSES) :));
    if( !Classes ) Classes = ([]);
}

static private void validate() {
    if( !((int)master()->valid_apply(({ PRIV_ASSIST }))) )
	error("Illegal attempt to modify class data");
}

int ClassMember(string my_class, string query_class) {
    class Class cls;

    if( my_class == query_class ) {
	return 1;
    }
    cls = Classes[query_class];
    if( !cls ) { // query class is a multi-class, thus must be equal to mine
	return 0; // This should change for triple classing
    }
    return (member_array(my_class, values(cls->Multis)) != -1);
}

void AddClass(string file) {
    class Class cls;
    string array lines, tmp;
    string class_name;
    
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
			      return 1;
                          });
    class_name = lines[0];
    if( Classes[class_name] ) error("Class already exists");
    cls = new(class Class);
    Classes[class_name] = cls;
    lines = lines[1..];
    cls->Multis = ([]);
    while( sizeof(tmp = explode(lines[0], ":")) == 2 ) {
	cls->Multis[tmp[0]] = tmp[1];
	lines = lines[1..];
    }
    cls->Skills = ([]);
    while(sizeof(tmp = explode(lines[0], ":")) == 3) {
	class Skill s = new (class Skill);

	s->Average = to_int(tmp[1]);
	s->SkillClass = to_int(tmp[2]);
	cls->Skills[tmp[0]] = s;
	if( sizeof(lines) == 1 ) {
	    lines = ({});
	    break;
	}
	else {
	    lines = lines[1..];
	}
    }
    cls->Complete = 1;
    save_object(SAVE_CLASSES);
}

void RemoveClass(string class_name) {
    validate();
    map_delete(Classes, class_name);
    save_object(SAVE_CLASSES);
}

void SetClass(string class_name, mixed array args) {
    class Class cls = Classes[class_name];
    mixed array primes, ots;
    
    if( !cls || !cls->Complete || sizeof(args) != 3 ) return;
    args[0] = cls->Multis;
    primes = ({});
    ots = ({});
    foreach(string key, class Skill skill in cls->Skills) {
	if( skill->SkillClass == 1 ) {
	    primes = ({ primes..., ({ key, skill->Average, 1 }) });
	}
	else {
	    ots = ({ ots..., ({ key, skill->Average, skill->SkillClass }) });
	}
    }
    args[1] = primes;
    args[2] = ots;
}
    
void SetComplete(string class_name) {
    class Class cls;
    
    validate();
    if( !Classes[class_name] ) error("No such class");
    else cls = Classes[class_name];
    cls->Complete = 1;
    save_object(SAVE_CLASSES);
}

varargs string array GetClasses() {
    return filter(keys(Classes), (: ((class Class)Classes[$1])->Complete :));
}

string GetHelp(string class_name) {
    class Class cls = Classes[class_name];
    string help = "Class: " + class_name + "\n\n";
    string tmp;
    int x;
    
    if( !cls ) return 0;
    if( !sizeof(cls->Multis) ) {
	help += capitalize(class_name) + " cannot multi-class.\n";
    }
    else {
	help += capitalize(pluralize(class_name)) + " can multi-class with " +
	        "the following primary classes:\n";
	foreach(string prime, string other in cls->Multis) {
	    help += "\t" + capitalize(class_name) + " + " + prime + " = " +
		other +  "\n";
	}
    }
    help += "\n" + capitalize(pluralize(class_name)) + " has the following " +
	"primary skills:\n";
    foreach(string skill, class Skill s in cls->Skills) {
	if( s->SkillClass == 1 ) {
	    help += "\t" + skill + "\n";
	}
    }
    return help + "\n";
}
   
