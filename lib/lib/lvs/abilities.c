/*    /lib/abilities.c
 *    From the Dead Souls LPC Library
 *    Handles learned traits
 *    Created by Descartes of Borg 950122
 *    Version: @(#) abilities.c 1.22@(#)
 *    Last modified: 97/01/03
 */

#include <daemons.h>

inherit LIB_LEVEL;

private int            Level       = 1;
private mapping        Skills      = ([]);
private static mapping SkillsBonus = ([]);

// abstract methods
varargs void eventPrint(string str, mixed args...);
// end abstract methods

string array GetPrimarySkills();
varargs void SetSkill(string skill, int level, mixed cls);

/* ***************** abilities.c attributes ***************** */
/* GetBaseSkillLevel() returns the unmodified skill level */
int GetBaseSkillLevel(string skill){ 
    if( !Skills[skill] ){
        return 0;
    }
    else {
        return Skills[skill]["level"];
    }
}

int GetLevel(){
    return Level;
}

/* static int ResetLevel()
 *
 * description
 * takes the average skill level of the primary skills and divided by 4
 * and sets the level to that number
 *
 * returns the new level
 */
/*
 * Excuse me, wtf is the point of this?
 */

int ResetLevel(){
    string array skills = GetPrimarySkills();
    int num = sizeof(skills);
    int points = 0;

    if( num < 1 ){
        return (Level = 1);
    }
    foreach(string skill in skills){
        points += Skills[skill]["level"];
    }
    Level = (points/num)/2;
    if( Level < 1 ){
        Level = 1;
    }
    return Level;
}

int SetLevel(int x){
    string array skills = GetPrimarySkills();

    if(!Level){
        foreach(string skill in skills){
            SetSkill(skill, 2*x);
        }
    }
    return (Level = x);
}

int GetMaxSkillLevel(string skill){
    int ret, cls = 4;
#ifdef MAX_SKILL_LEVEL
#if MAX_SKILL_LEVEL
    ret = MAX_SKILL_LEVEL;
    return ret;
#endif
#endif
    cls -= (Skills[skill]["class"] || 4);
    if(cls < 0) cls = 0;
    ret = ((GetLevel()+cls) *2);
    return ret;
}

int GetMaxSkillPoints(string skill, int level){
    if( !Skills[skill] ){
        return 0;
    }
    else if( level == 0 ){
        return 200;
    }
    else {
        int cl, x;

        if( !(cl = Skills[skill]["class"]) ){
            return level * 600;
        }
        if( cl > 4 ){
            cl = 4;
        }
        if( cl < 1 ){
            cl = 4;
        }
        x = level;
        while( cl-- ){
            x *= level;
        }
        return (x * 400);
    }
}

string array GetPrimarySkills(){ 
    return filter(keys(Skills), (: Skills[$1]["class"] == 1 :));
}

/* varargs int AddSkill(string skill, int classes)
 * string skill - the skill being added (required)
 * int classes - the class of the skill (optional)
 *
 * defaults
 * classes defaults to 1, primary class
 *
 * description
 * Used to add a new ability to the list of abilities.  "skill" is the name
 * of the skill being added, and "class" represents the ranking of that skill
 *
 * returns 1 on success, 0 on failure
 */
varargs int AddSkill(string skill, int cls, int level){
    if( !stringp(skill) ){
        error("Bad argument 1 to AddSkill().\n\tExpected: string, Got: " +
                typeof(skill) + "\n");
    }
    if( !nullp(Skills[skill]) ){
        return 0;
    }
    if( !cls ){
        cls = 1;
    }
    if(!level){
        level = 1;
    }
    else if( cls < 0 || cls > 4){
        return 0;
    }
    Skills[skill] = ([ "points" : 0, "level" : level, "class" : cls ]);
    return 1;
}

mapping GetSkill(string skill){
    return copy(Skills[skill]);
}

void RemoveSkill(string skill){
    if( !Skills[skill] ){
        return;
    }
    map_delete(Skills, skill);
}

/* varargs void SetSkill(string skill, int level, int cls)
 * string skill - the name of the skill being set (required)
 * int level - the level to which that skill is being set (required)
 * int classes - the class to which the skill is being set (optional)
 *
 * defaults
 * classes will default to 1, primary class
 *
 * description
 * sets the skill "skill" to the level "level"
 * if a classes is given, its class is set to it, otherwise the class is
 * set to 1
 * useful mostly for monster types, probably should have override
 * protections in the user object (should use AddSkill() for users)
 */
varargs void SetSkill(string skill, int level, int cls){
    int tmp;
    SKILLS_D->SetSkill(skill);
    if(cls && !intp(cls)){
        tmp = 1;
        cls = tmp;
    }
    if( !stringp(skill) ){
        error("Bad argument 1 to SetSkill().\n\tExpected: string, Got: " +
                typeof(skill) + "\n");
    }
    if( !cls ){
        if( Skills[skill] ){
            cls = Skills[skill]["class"];
        }
        else {
            cls = 1;
        }
    }
    else if( cls < 1 || cls > 4){
        return 0;
    }
    Skills[skill] = ([ "points" : 0, "level" : level, "class" : cls ]);
}

string array GetSkills(){
    return keys(Skills);
}

mapping GetSkillsMap(){
    return copy(Skills);
}

void AddSkillBonus(string skill, mixed f){
    if( !SkillsBonus[skill] ){
        SkillsBonus[skill] = ([]);
    }
    SkillsBonus[skill][previous_object()] = f;
}

varargs void RemoveSkillBonus(string skill, object ob){
    if( !SkillsBonus[skill] ){
        return;
    }
    if( !ob ){
        ob = previous_object();
    }
    if( !ob || !SkillsBonus[skill][ob] ){
        return;
    }
    map_delete(SkillsBonus[skill], ob);
}

int GetSkillBonus(string skill){
    object ob;
    int x = 0;

    if( !SkillsBonus[skill] ){
        return 0;
    }

    if(intp(SkillsBonus[skill]))
        return SkillsBonus[skill];

    foreach(ob in keys(SkillsBonus[skill])){
        if( !ob ) continue;
        else if(intp(SkillsBonus[skill][ob])) x += SkillsBonus[skill][ob];
        else x += evaluate(SkillsBonus[skill][ob], skill);
    }
    return x;
}

int GetSkillClass(string skill){ 
    if( !Skills[skill] ){
        return 0;
    }
    else {
        return Skills[skill]["class"];
    }
}

/* GetSkillLevel() returns the base skill level + any bonuses */
int GetSkillLevel(string skill){ 
    return (GetBaseSkillLevel(skill) + GetSkillBonus(skill));
}

/* int AddSkillPoints(string skill, int x)
 * string name - the skill to which points are being added
 * int x - the number of points being added
 *
 * description
 * adds "x" points to the total for skill "skill"
 * if the points go over the max for the current skill level,
 * then the level is raised 1
 * if the points go under 0, then the skill level is lowered
 *
 * returns the skill level after addition
 */
int AddSkillPoints(string name, int x){
    int y;

    if( !Skills[name] ){
        return 0;
    }
    Skills[name]["points"] += x;
    while( Skills[name]["points"] < 0 ){ /* lost skills! */
        if( Skills[name]["level"] == 1 ){
            Skills[name]["points"] = 0;
        }
        else {
            int tmp;
            if(Skills[name]["level"] > 1){
                tmp = --Skills[name]["level"];
                Skills[name]["points"] += GetMaxSkillPoints(name, tmp);
            }
            else break;
        }
    }
    y = GetMaxSkillPoints(name, Skills[name]["level"]);
    while( Skills[name]["points"] > y ){
        if( Skills[name]["level"] >= GetMaxSkillLevel(name) ){
            Skills[name]["points"] = y;
        }
        else {
            eventPrint("%^YELLOW%^You are a bit more adept with your " +
                    name + ".");
            Skills[name]["level"]++;
            Skills[name]["points"] -= y;
        }
        y = GetMaxSkillPoints(name, Skills[name]["level"]);
    }
    return Skills[name]["level"];
}

/* ******************* abilities.c events ********************* */
/**
 * Trains a skill based on a scale of 1-100 in favour of success and 1-100
 * against success.  Any particular thing (like calculating whether a player
 * hits an enemy) determins whether an operation succeeds or fails.  For
 * example, pro = melee attack, con = melee defense.  While the con may
 * be greater than the pro, combat may randomize that.  You still reward
 * based on the absolutes.  Success is flagged to this method.  The value of
 * training looks like this:
 * highest- pro: 0, con: 100, success: 1
 * moderate- pro: 100, con: 0, success: 1
 * moderate- pro: 0, con 100, success: 0
 * low- pro: 100, con: 0, success: 0
 * This means if you have everything going against you and you succeed, you
 * get the highest reward.  If you have everything going for you and you fail,
 * you get nothing.
 * Adjustments are made for your level and any multiplier bonuses.
 */
varargs void  eventTrainSkill(string skill, int pro, int con, int array a...){
    int level = (GetLevel()/8 + 1);
    int val, success, bonus;

    if( sizeof(a) ){
        success = a[0];
        if( sizeof(a) == 2 ){
            bonus = a[1];
        }
        else {
            bonus = 1;
        }
    }
    else {
        success = 1;
        bonus = 1;
    }
    if( con > 100 ){
        con = 100;
    }
    if( con < 0 ){
        con = 0;
    }
    if( pro > 100 ){
        pro = 100;
    }
    if( pro < 0 ){
        pro = 0;
    }
    val = (con - pro + (200*success) + 100)/8;
    AddSkillPoints(skill, (bonus * val * level * level) + 1);
}

/* ****************** abilities.c driver applies **************** */
static void create(){
}

