/*    /lib/genetics.c
 *    from the Dead Souls LPC Library
 *    handles genetic characteristics
 *    created by Descartes of Borg 950122
 *    Version: @(#) genetics.c 1.4@(#)
 *    Last Modified: 96/11/11
 */

#include <vision.h>
#include <function.h>
#include <damage_types.h>
#include <daemons.h>
#include "include/genetics.h"

class blindness {
    int count;
    mixed end;
}

private class blindness Blind            = 0;
private mapping         Custom           = ([]);
private int array       LightSensitivity = ({ 25, 75 });
private mapping         Resistance       = ([]);
private mapping         Stats            = ([]);
private static mapping  StatsBonus       = ([]);
private static int      VisionBonus      = 0;

// abstract methods
string GetName();
varargs void eventPrint(string message, mixed args...);
// end abstract methods

static void create(){
    Custom = ([
            "stats" : 15,
            "deviations" : 4,
            "deviating" : 0,
            ]);
    Resistance = ([ "low" : 0, "medium" : 0, "high" : 0, "immune" : 0 ]);
    Resistance["none"] = ALL_DAMAGE;
}

int GetBlind(){
    if( Blind ){
        return 1;
    }
    else {
        return 0;
    }
}

static void RemoveBlindness(){
    mixed val = Blind->end;

    Blind = 0;
    if( arrayp(val) ){
        send_messages(val[0], val[1], this_object());
    }
    else if( functionp(val) && !(functionp(val) & FP_OWNER_DESTED) ){
        evaluate(val, this_object());
    }
    else {
        eventPrint("You can see again.");
    }
}

varargs mixed eventBlind(object who, int amt, mixed end){
    Blind = new(class blindness);
    Blind->count = amt;
    Blind->end = end;
    return 1;
}

mixed eventCustomizeStat(string stat, int amount){
    if( amount < 1 ) return "That is not a valid amount.";
    if( amount > Custom["stats"] )
        return "You do not have enough points to spend on this customization.";
    if( !Stats[stat] ) return "You have no such stat.";
    if( Stats[stat]["level"] + amount > 100 )
        return "You cannot make a stat exceed 100.";
    Stats[stat]["level"] += amount;
    Stats[stat]["points"] = 0;
    Custom["stats"] -= amount;
    return Stats[stat]["level"];
}

mixed eventDeviateStat(string stat, int amount){
    if( amount < 1 ) return "That is not a valid amount.";
    if( amount > Custom["deviations"] )
        return "You do not have enough points to spend on that deviation.";
    if( !Stats[stat] ) return "You have no such stat.";
    if( Stats[stat]["class"] - amount < 1 )
        return "You cannot deviate a stat class below 1.";
    Stats[stat]["class"] -= amount;
    Custom["deviations"] -= amount;
    return Stats[stat]["class"];
}

mixed eventRestoreSight(object who, int amt){
    if( !Blind ){
        return GetName() + " is not blind!";
    }
    Blind->count -= amt;
    if( Blind->count < 1 ){
        RemoveBlindness();
        return 1;
    }
    return 0;
}

varargs void SetStat(string stat, int level, int classes){
    if(!stat) return;
    if(level < 1) level = 1;
    if(!classes) classes = 1;
    Stats[stat] = ([ "points" : 0, "level" : level, "class" : classes ]);
    STATS_D->SetStat(stat);
}

varargs void AddStat(string stat, int base, int cls){
    int level;
    if( userp(this_object()) ) level = 1;
    else level = GetLevel();
    if( !stat || cls < 1 || cls > 5 ) return;
    base += ((5 - cls) * random(10)) + (3 * (level + 1))/(cls * 4);
    if( userp(this_object()) && base > 90 ) base = 90;
    else if( base > 100 ) base = 100;
    STATS_D->SetStat(stat);
    SetStat(stat, base, cls);
}

varargs void RemoveStat(string stat){
    if(!stat) return;
    if(Stats[stat]) map_delete(Stats, stat);
}

mapping GetStat(string stat){
    mapping ret = ([]);
    if(!stat) return 0;
    ret = copy(Stats[stat]);
    if(!sizeof(ret)) return 0;
    if(ret["level"] < 0) ret["level"] = 0;
    return ret;
}

int GetStatClass(string stat){
    if(!Stats[stat]) return 0;
    else return Stats[stat]["class"];
}

int GetBaseStatLevel(string stat){
    if(!Stats[stat]) return 0;
    else return Stats[stat]["level"];
}

int GetStatLevel(string stat){
    int x;

    x = (GetBaseStatLevel(stat) + GetStatBonus(stat));
    switch(stat){
        case "coordination": case "wisdom":
            x -= this_object()->GetAlcohol();break;
        case "strength": case "durability" : case "agility":
            x -= this_object()->GetPoison();break;
        case "intelligence": case "speed": case "coordination":
            x += (this_object()->GetCaffeine() / 10);break;
    }
    return x;
}

int AddStatPoints(string stat, int x){
    int y;

    if( !Stats[stat] ) return 0;
    Stats[stat]["points"] += x;
    while( Stats[stat]["points"] < 0 ){
        if( Stats[stat]["level"] == 1 ) Stats[stat]["points"] = 0;
        else {
            int tmp;

            tmp = --Stats[stat]["level"];
            Stats[stat]["points"] += GetMaxStatPoints(stat, tmp);
        }
    }
    while(Stats[stat]["points"] > (y = GetMaxStatPoints(stat,
                    Stats[stat]["level"]))){
        if(Stats[stat]["level"] >= GetLevel()*4) Stats[stat]["points"] = y;
        else {
            Stats[stat]["level"]++;
            Stats[stat]["points"] -= y;
        }
    }
    return Stats[stat]["level"];
}

string *GetStats(){ return keys(Stats); }
mapping GetStatsMap(){ return copy(Stats); }

int GetMaxStatPoints(string stat, int level){
    if( !Stats[stat] ) return 0;
    else {
        int cl, x;

        if( !(cl = Stats[stat]["class"]) ) return level * 600;
        x = level;
        while( cl-- ) x *= level;
        return x * 600;
    }
}

void AddStatBonus(string stat, mixed f){
    if(!StatsBonus[stat]) StatsBonus[stat] = ([]);
    StatsBonus[stat][previous_object()] = f;
}

varargs void RemoveStatBonus(string stat, object ob){
    if(!StatsBonus[stat]) return;
    if(!ob) ob = previous_object();
    if(!ob || !StatsBonus[stat][ob]) return;
    map_delete(StatsBonus[stat], ob);
}

int GetStatBonus(string stat){
    object *obs;
    int i, x = 0;

    if( !StatsBonus[stat] ) return 0;
    i = sizeof(obs = keys(StatsBonus[stat]));
    while(i--){
        if( !obs[i] ) map_delete(StatsBonus[stat], obs[i]);
        else x += evaluate(StatsBonus[stat][obs[i]], stat);
    }
    return x;
}

/* string SetResistance(int type, string level) 
 * int type - the type being set (you can do a bitwise | on them)
 * string level "none", "low", "medium", "high", and "immune"
 *
 * description
 * you can have a several levels of resistence for given types of damage
 * setting a resistence level here unsets any other levels of resistence
 */

varargs string SetResistance(int type, string level){
    if(level == "none") Resistance["none"] |= type;
    else Resistance["none"] &= ~type;
    if(level == "low") Resistance["low"] |= type;
    else Resistance["low"] &= ~type;
    if(level == "medium") Resistance["medium"] |= type;
    else Resistance["medium"] &= ~type;
    if(level == "high") Resistance["high"] |= type;
    else Resistance["high"] &= ~type;
    if(level == "immune") Resistance["immune"] |= type;
    else Resistance["immune"] &= ~type;
}

string GetResistance(int type){
    if(Resistance["low"] & type) return "low";
    else if(Resistance["medium"] & type) return "medium";
    else if(Resistance["high"] & type) return "high";
    else if(Resistance["immune"] & type) return "immune";
    else return "none";
}

mapping GetResistanceMap(){
    return copy(Resistance);
}

int GetCustomStats(){ return Custom["stats"]; }
int GetCustomDeviations(){ return Custom["deviations"]; }
int GetDeviating(){ return Custom["deviating"]; }
int SetDeviating(int x){ return (Custom["deviating"] = (x ? 1 : 0)); }

varargs mixed GetEffectiveVision(mixed location, int raw_score){
    int array l;
    object env, rider, where;
    int bonus = GetVisionBonus();
    int a, y, x = 0;

    env = environment();

    if(raw_score && !intp(raw_score)){
        location = raw_score;
        raw_score = 0;
    }

    //fixme
    if(!location && sizeof(get_livings(this_object())) && 
            rider = get_random_living(this_object())){
        if(rider->GetProperty("mount") == this_object() && env){
            return rider->GetEffectiveVision(env);
        }
    }

    if(location){
        if(objectp(location)) where = location;
        else if(stringp(location)){
            int err;
            err = catch( where = load_object(location) );
            if(err || !where) return 0;
        }
    }
    if( Blind && !raw_score){
        return VISION_BLIND;
    }
    if( !where && !location ){
        where = env;
    }
    if(!where) return 0;
    x = (env == where ? GetRadiantLight(0) : 0);
    a = where->GetAmbientLight();
    if(x){
        x = x/2;
        x += GetRadiantLight(a) + a;
    }
    else x = a * 2;
    l = GetLightSensitivity();
    l[0] -= bonus;
    l[1] += bonus;

    if(raw_score && !location){
        return "Low: "+l[0]+", High: "+l[1];
    } 
    if( x >= l[0] && x <= l[1] ) return VISION_CLEAR;
    y = l[0]/3;
    if( x < y ) return VISION_TOO_DARK;
    if( x < (2*y) ) return VISION_DARK;
    if( x < l[0] ) return VISION_DIM;
    y = l[1]/3;
    if( x < (l[1] + y) ) return VISION_LIGHT;
    if( x < (l[1] + (2*y)) ) return VISION_BRIGHT;
    return VISION_TOO_BRIGHT;
}

int array GetLightSensitivity(){
    if( !LightSensitivity ) return ({ 25, 75 });
    else return LightSensitivity;
}

varargs static int array SetLightSensitivity(mixed array val...){
    if( !val ) error("Null argument to SetLightSensitivity().\n");
    if( sizeof(val) == 1 ) val = val[0];
    if( sizeof(val) != 2 )
        error(sprintf("Invalid arguments to SetLightSensitivity(): %O\n",  val));
    return (LightSensitivity = val);
}

int AddVisionBonus(int x){
    VisionBonus += x;
    return VisionBonus;
}

int GetVisionBonus(){
    return VisionBonus;
}

static void heart_beat(){
    if( Blind ){
        Blind->count--;
        if( Blind->count < 1 ){
            RemoveBlindness();
        }
    }
}
