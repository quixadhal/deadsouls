/*    /lib/race.c
 *    from the Dead Souls LPC Library
 *    handles all race specific management
 *    created by Descartes of Borg 950122
 *    Version: @(#) race.c 1.8@(#)
 *    Last modified: 96/11/11
 */

#include <lib.h>
#include <daemons.h>
#include <armour_types.h>
#include <damage_types.h>
#include <meal_types.h>
#include "include/race.h"

inherit LIB_BODY;
inherit LIB_GENETICS;
inherit LIB_LANGUAGE;
inherit LIB_TALK;

private string Town, Race, Gender;

// abstract methods
int GetParalyzed();
// end abstract methods

static void create() {
    body::create();
    genetics::create();
    Race = "blob";
    Gender = "neuter";
    Town = "Praxis";
}

mixed CanDrink(object ob) {
    int strength, type;

    if( !ob ) return 0;
    strength = (int)ob->GetStrength();
    type = (int)ob->GetMealType();
    if( (type & MEAL_ALCOHOL) && ((strength + GetAlcohol()) >
                                GetStatLevel("durability")) )
      return "You are way too drunk to coordinate another drink.";
    if( (type & MEAL_CAFFEINE) && ((strength + GetCaffeine()) >
                                   GetStatLevel("durability")) )
      return "Any more caffeine and your heart will burst.";
    if( (type & MEAL_DRINK) && ((strength + GetDrink()) > 100) )
      return "You are too bloated to drink any more!";
    return 1;
}

mixed CanEat(object ob) {
    if( ((int)ob->GetStrength() + GetFood()) > 100 )
      return "One more bite, and you would explode!";
    else return 1;
}

varargs int eventDie(object agent) {
    int x;

    if( (x = body::eventDie(agent)) != 1 ) return x;
    return 1;
}

mixed eventDrink(object ob) {
    int type, strength;

    type = (int)ob->GetMealType();
    strength = (int)ob->GetStrength();
    if( type & MEAL_POISON ) AddPoison(strength);
    if( type & MEAL_DRINK ) AddDrink(strength);
    if( type & MEAL_ALCOHOL ) AddAlcohol(strength);
    if( type & MEAL_CAFFEINE ) AddCaffeine(strength);
    return 1;
}

mixed eventEat(object ob) {
    AddFood((int)ob->GetStrength());
    if( (int)ob->GetMealType() & MEAL_POISON )
      AddPoison((int)ob->GetStrength());
    return 1;
}

varargs string SetRace(string race, mixed extra) {
    mixed array args = ({ ({}), ({}), ({}), ({}) });
    mixed array tmp;
    
    RACES_D->SetCharacterRace(race, args);
    foreach(tmp in args[0]) SetResistance(tmp...);
    foreach(tmp in args[1]) AddStat(tmp...);
    if( stringp(args[2]) ) SetLanguage(args[2], 100, 1);
    if( sizeof(args[3]) == 2 ) SetLightSensitivity(args[3]...);
    if( extra != 1 ) NewBody(race);
    if( stringp(extra) ) return (Race = extra), race; 
    else return (Race = race);
}

string GetRace() { return Race; }

string SetGender(string gender) { return (Gender = gender); }

string GetGender() { return Gender; }

varargs void SetStat(string stat, int level, int classes) {
    int healthPoints;

    genetics::SetStat(stat, level, classes);
    switch(stat) {
        case "durability":
            eventCompleteHeal(healthPoints = GetMaxHealthPoints());
            eventHealDamage(healthPoints);
            break;
        case "intelligence":
            AddMagicPoints(GetMaxMagicPoints());
            break;
        case "agility":
            AddStaminaPoints(GetMaxStaminaPoints());
            break;
    }
}

varargs int GetMaxHealthPoints(string limb) {
    if(!limb) return ( 50 + (GetStatLevel("durability") * 10) );
    else {
        int x;

        x = GetLimbClass(limb);
        if(!x) x = 5;
        return ( (1 + GetStatLevel("durability")/x) * 10 );
    }
}

int GetMaxMagicPoints() {
    return ( 50 + (GetStatLevel("intelligence") * 10) );
}

float GetMaxStaminaPoints() {
    return (50.0 + (GetStatLevel("agility") * 10.0) );
}

void NewBody(string race) {
    mixed array args = allocate(2);
    mixed array tmp;
    
    body::NewBody(race);
    if(!race) return;
    RACES_D->SetCharacterLimbs(race, args);
    foreach(tmp in args[0]) AddLimb(tmp...);
    foreach(tmp in args[1]) AddFingers(tmp...);
}

string SetTown(string str) { return (Town = str); }

string GetTown() { return Town; }

string GetResistance(int type) { return genetics::GetResistance(type); }

int GetLuck() {
    int x;

    x = random(GetStatLevel("luck")) / 20;
    x = ((x > 4) ? 4 : x);
    if( newbiep() ) x += random(7);
   return (x + random(4));
}

int GetMobility() {
   int max = GetMaxCarry();
   int encum, mob;

   if( GetParalyzed() ) {
       return 0;
   }
   if( max < 1 ) {
       max = 1;
   }
   encum = (GetCarriedMass() * 100)/max;
   encum -= (encum * GetStatLevel("agility"))/200;
   mob = 100 - encum;
   if( mob > 100 ) {
       mob = 100;
   }
   else if( mob < 1 ) {
       mob = 0;
   }
   return mob;
}

int GetCarriedMass() { return 0; }

int GetMaxCarry() { return ((2 + GetStatLevel("strength")) * 100); }

int GetHeartRate() {
    int x, y;

    x = body::GetHeartRate();
    y = GetStatLevel("speed");
    if( y > 80 ) x -= 2;
    else if( y > 60 ) x -= 1;
    else if( y > 40 ) x = x;
    else if( y > 20 ) x += 1;
    else x += 2;
    if( x > 6 ) x = 6;
    else if( x < 2 ) x = 2;
    return x;
}

int GetHealRate() {
    int x;

    x = body::GetHealRate() + random((GetStatLevel("durability")/40) + 1);
    if( x > 6 ) x = 6;
    return x;
}

int GetStatLevel(string stat) { return genetics::GetStatLevel(stat); }

int GetAlcohol() { return body::GetAlcohol(); }

static void heart_beat() {
    body::heart_beat();
    language::heart_beat();
    genetics::heart_beat();
}
