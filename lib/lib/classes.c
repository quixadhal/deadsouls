/*    /lib/classes.c
 *    from the Dead Souls LPC Library
 *    classes and guild handling object
 *    created by Descartes of Borg 950123
 *    Version: @(#) classes.c 1.4@(#)
 *    Last modified: 96/12/13
 */

#include <lib.h>
#include <daemons.h>
#include "include/classes.h"

inherit LIB_ABILITIES;

private int Morality;
private string Class, Guild;
private mapping SkillModifiers;
private string *Religion;

static void create() {
    abilities::create();
    SkillModifiers = ([]);
    Religion = allocate(2);
    Class = 0;
    Guild = 0;
    Morality = 0;
}

int eventMoralAct(int degree) {
    if( degree > 10 ) {
	degree = 10;
    }
    else if( degree < -10 ) {
	degree = -10;
    }
    Morality += degree;
    if( Morality > 2500 ) Morality = 2500;
    else if( Morality < -2500 ) Morality = -2500;
    return Morality;
}

int AddSkillPoints(string skill, int x) {
    if( SkillModifiers[skill] ) {
	int stat_level; 
	stat_level = GetBaseStatLevel(SkillModifiers[skill]);
	if( stat_level < 20 ) x = x - x/2;
	else if( stat_level < 40 ) x = x - x/3;
	else if( stat_level > 70 && x < 100 ) x = x + x/3;
	else if( stat_level > 99 ) x = x + x/2;
    }
    return abilities::AddSkillPoints(skill, x);
}

static string SetSkillModifier(string skill, string stat) {
    if(!GetSkill(skill)) return 0;
    else return (SkillModifiers[skill] = stat);
}

string GetSkillModifier(string skill) { return SkillModifiers[skill]; }

string SetClass(string class_name) {
    mixed array args = allocate(3);
    mixed array tmp;
    
    CLASSES_D->SetClass(class_name, args);
    if( Class ) {
	string multi;

	if( !high_mortalp() ) { // Not high mortal
	    return Class;
	}
	if( !args[0] ) { // No such secondary class
	    return Class;
        }
        multi = args[0][Class];
	if( !multi ) { // Can't multi-class in this combo
	    return Class;
	}
	class_name = multi;
    }
    else {
	if( !args[0] ) { // No such class
	    return Class;
	}
	foreach(tmp in args[2]) {
	    SetSkill(tmp...);
	}
    }
    foreach(tmp in args[1]) {
	SetSkill(tmp...);
    }
    return (Class = class_name);
}

string ChangeClass(string class_name)  {
    mixed array args = allocate(3);
    mixed array tmp;
    string cl;
    foreach(cl in GetSkills())  {
        RemoveSkill(cl);
    }
    Class = 0;
    return SetClass(class_name);
}

string GetClass() { return Class; }

int ClassMember(string class_name) {
    return (int)CLASSES_D->ClassMember(Class, class_name);
}

string SetGuild(string guild) { return (Guild = guild); }

string GetGuild() { return Guild; }

int GetBaseStatLevel(string stat) { return 0; }

int SetMorality(int x) { return (Morality = x); }

int GetMorality() { return Morality; }
 
string GetMoralityDescription() {
    string str;
    int x;

    x = GetMorality();
    if( x > 2000 ) str = "saintly";
    else if( x > 1500 ) str = "pious";
    else if( x > 1000 ) str = "benevolent";
    else if( x > 700 ) str = "good";
    else if( x > 450 ) str = "friendly";
    else if( x >= 200 ) str = "nice";
    else if( x > -200 ) str = "neutral";
    else if( x > -450 ) str = "mean";
    else if( x > -700 ) str = "cruel";
    else if( x > -1000 ) str = "wicked";
    else if( x > -1500 ) str = "malevolent";
    else if( x > -2000 ) str = "dastardly";
    else str = "demonic";
    return str;
}

string *SetReligion(string adj, string noun) {
    Religion[0] = adj;
    Religion[1] = noun;
    return Religion;
}

varargs string GetReligion(int flag) { return Religion[flag]; } 
