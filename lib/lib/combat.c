/*    /lib/combat.c
 *    from the Dead Souls LPC Library
 *    combat events and data
 *    created by Descartes of Borg 950124
 *    Version: @(#) combat.c 1.40@(#)
 *    Last modified: 96/11/17
 */

#include <lib.h>
#include <rounds.h>
#include <daemons.h>
#include <position.h>
#include <damage_types.h>
#include <function.h>

inherit LIB_RACE;
inherit LIB_CLASSES;
inherit LIB_COMBATMSG;

private int Wimpy;
private string WimpyCommand;
private static int cParalyzed, tNextRound;
private static string TargetLimb, Party;
private static object CurrentEnemy;
private static function fParalyzed, fNextRound;
private static object *Hostiles, *Enemies, *SpecialTargets;

string GetName();
mixed GetProperty(string key);
string array AddChannel(mixed val);
string array RemoveChannel(mixed val);
int eventForce(mixed args);
int eventExecuteAttack(mixed target);
int eventWeaponRound(mixed target, mixed val);
void eventWeaponAttack(object target, object weapon, int num);
int eventMeleeRound(mixed target, function f);
void eventMeleeAttack(object target, string limb);
int eventMagicRound(mixed target, function f);
int eventWimpy();

static void create() {
    race::create();
    classes::create();
    Hostiles = ({});
    Enemies = ({});
    CurrentEnemy = SpecialTargets = 0;
    Party = 0;
    fParalyzed = 0;
    fNextRound = 0;
    cParalyzed = 0;
    tNextRound = ROUND_UNDEFINED;
    Wimpy = 0.20;
    WimpyCommand = "go out";
}

/*  *****************  /lib/combat.c data functions  *****************  */
object array GetEnemies() {
    return Enemies;
}

int AddEnemy(object ob) {
    if( !ob || (member_array(ob, Enemies) != -1) ) {
	return 0;
    }
    if( !living(ob) ) {
	return 0;
    }
    Enemies += ({ ob });
    return 1;
}

int RemoveEnemy(object ob) {
    if( !ob || (member_array(ob, Enemies) == -1) ) {
	return 0;
    }
    Enemies -= ({ ob });
    return 1;
}

object SetCurrentEnemy(object ob) {
    if( !ob ) {
	return (CurrentEnemy = 0);
    }
    if( !living(ob) ) {
	return CurrentEnemy;
    }
    if( member_array(ob, Enemies) == -1 ) {
	AddEnemy(ob);
    }
    return (CurrentEnemy = ob);
}

static object ResetCurrentEnemy() {
    object array obs;

    obs = filter(GetEnemies(), (: $1 && environment() == environment($1) :));
    if( !sizeof(obs) ) {
	return 0;
    }
    return SetCurrentEnemy(obs[random(sizeof(obs))]);
}

object GetCurrentEnemy() {
    return CurrentEnemy;
}

private static void SortEnemies() {
    if( !sizeof(Enemies = filter(Enemies, (: ($1 && living($1)) :))) ) {
        Hostiles = ({});
        CurrentEnemy = 0;
        return;
    }
    Hostiles = (Hostiles & Enemies);
}

int AddHostile(object ob) {
    if( !ob || (member_array(ob, Hostiles) != -1) ) {
	return 0;
    }
    if( !living(ob) ) {
	return 0;
    }
    Hostiles += ({ ob });
    return 1;
}

int RemoveHostile(object ob) {
    if( !ob || (member_array(ob, Hostiles) == -1) ) {
	return 0;
    }
    Hostiles -= ({ ob });
    return 1;
}

object array GetHostiles() {
    return Hostiles;
}

object array GetSpecialTarget() {
    return SpecialTargets;
}

varargs int SetParalyzed(int count, function f) {
    if(count < 1) {
        count = cParalyzed = 0;
        fParalyzed = 0;
    }
    else {
        fParalyzed = f;
        cParalyzed = count;
    }
    return count;
}

int GetParalyzed() { return cParalyzed; }

string SetParty(string str) {
    if( file_name(previous_object()) != PARTY_D ) return Party;
    if( str ) AddChannel(str);
    else RemoveChannel(Party);
    return (Party = str);
}

string GetParty() {
    return Party;
}

varargs int SetAttack(mixed target, function callback, int type) {
    int i;

    if( objectp(target) ) target = ({ target });
    if( target ) {
        if( member_array(this_object(), target) != -1 ) return 0;
        if( !GetCurrentEnemy() ) call_out((: eventExecuteAttack :), 0, target);
        i = sizeof(target);
        while(i--) if( AddEnemy(target[i]) ) AddHostile(target[i]);
        SpecialTargets = target;
    }
    fNextRound = callback;
    tNextRound = (type || ROUND_UNDEFINED);
    return 1;
}

int GetLevel() {
    return classes::GetLevel();
}

int GetInCombat() {
    return sizeof(filter(GetEnemies(),
                         (: $1 && (environment($1) == environment()) :)));
}

int GetBaseStatLevel(string stat) {
    return race::GetBaseStatLevel(stat);
}

float SetWimpy(float wimpy) {
    return (Wimpy = wimpy);
}

float GetWimpy() {
    return Wimpy;
}

string SetWimpyCommand(string cmd) {
    return (WimpyCommand = cmd);
}

string GetWimpyCommand() {
    return WimpyCommand;
}

int GetMaxCarry() {
    return race::GetMaxCarry();
}

int GetMagicChance(int val) {
    val = GetStatLevel("intelligence")/2 + (3*val)/2;
    return val + GetLuck();
}

int GetMagicResistance() {
    int val = GetStatLevel("wisdom")/2 + (3*GetSkillLevel("magic defense"))/2;

    return val + GetLuck();
}
	
int GetCombatChance(int val) {
    val = val + random((val * GetMobility())/250);
    val = (val/(3- visibility()) + GetLuck());
    if( GetBlind() ) {
	return val/10;
    }
    else {
	return val;
    }
}

int GetDefenseChance(int val) {
    val = (val * GetMobility())/50;
    val = (val/(3- visibility()) + (GetLuck()/2));
    if( GetBlind() ) {
	return val/10;
    }
    else {
	return val;
    }
}

int GetCombatBonus(int level) {
    int diff = level - GetLevel();
    
    if( diff >= 6 && diff < 16 ) {
	return 4;
    }
    else if( diff >= 1 && diff < 6 ) {
	return 3;
    }
    else if( diff >= -9 && diff < 1 ) {
	return 2;
    }
    else if( diff >= -20 && diff < -9 ) {
	return 1;
    }
    else {
	return 0;
    }
}

static int GetDamage(int power, string skill) {
    int x = GetSkillLevel(skill);

    if( power < 1 ) {
	return 0;
    }
    else if( power > 100 ) {
	power = 100;
    }
    if( x < 1 ) {
	x = 1;
    }
    else if( x > 100 ) {
	x = 100;
    }
    x = (x * (power/2 + random(power/2))/10);
    x += (GetLuck()/2) + GetStatLevel("strength")/8;
    if( x < 1 ) { // negative luck or cursed strength
	return 1;
    }
    return x;
}

int CanWeapon(object target, string type, int hands, int num) {
    string limb = target->GetRandomLimb(TargetLimb);
    int chance = (7*GetSkillLevel(type+" attack") + 
			3*GetStatLevel("coordination"))/10;
    int div = 2;
    int x, y;
  
  if(hands > 1) {  
     if(GetSkillLevel("multi-hand")) {
	  chance = (chance/2) + 
	  (GetSkillLevel("multi-hand")/75)*(chance/2);
   	   }
     else { /* If you are really strong you can use multihand a bit */
	  chance *= GetStatLevel("strength")/300;
	  div += (hands-1);
	 }
  }
  if(num > 1) {
     if(GetSkillLevel("multi-weapon")) {
        chance = (chance/2) + 
	 (GetSkillLevel("multi-weapon")/75)*(chance/2);
	  }    
     else { /* If you are really coordinated you can use multiweap a bit */
	  chance *= GetStatLevel("coordination")/300;
	  div += (num-1);
	 }
   }
    chance = GetCombatChance(chance/div);
    x = random(chance);
    y = random(10);
    if( x <= y ) {
        if( x > y/2 ) { 
            TargetLimb = target->GetRandomLimb(0);
        }
        else { 
            TargetLimb = 0;
        }
    }
    else {
	TargetLimb = limb;
    }
    return chance;
}
    

int CanMelee(object target) {
    string limb = target->GetRandomLimb(TargetLimb);
    int chance = ( 7*GetSkillLevel("melee attack") +
			 3*GetStatLevel("coordination") )/10;
    int y = random(10);
    int x;
    
    chance = GetCombatChance(chance/2);
    x = random(chance);
    if( x <= y ) {
	if( x > y/2 ) {
	    TargetLimb = target->GetRandomLimb(0);
	}
	else {
	    TargetLimb = 0;
	}
    }
    else {
	TargetLimb = limb;
    }
    return chance;
}

static int Destruct() {
    if( GetParty() ) PARTY_D->eventLeaveParty(this_object());
    return 1;
}

/*  *****************   /lib/combat.c events  ***************** */

varargs int eventDie(object agent) {
    object ob;
    int x;

    x = race::eventDie(agent);
    if( x != 1 ) {
	return x;
    }
    foreach(ob in GetEnemies()) {
	if( ob ) {
	    ob->eventEnemyDied(this_object());
	}
    }
    environment()->eventLivingDied(this_object(), agent);
    Enemies = ({});
    return 1;
}

int eventExecuteAttack(mixed target) {
    object array weapons;
    function f = fNextRound;
    int type = tNextRound;
    int position = GetPosition();

    fNextRound = 0;
    tNextRound = ROUND_UNDEFINED;
    if( position == POSITION_LYING || position == POSITION_SITTING ) {
	eventPrint("You can't fight unless you are standing up!");
	return 0;
    }
    if( arrayp(target) ) {
	if( !f || (functionp(f) & FP_OWNER_DESTED) ) {
	    return 0; /* built in only handles 1 targ */
	}
        target = filter(target, function(object ob) {
	                           if( !ob ) {
				       return 0;
				   }
				   if( !ob->eventPreAttack(this_object()) ) {
				       return 0;
				   }
				   return 1;
	                       });
        if( !sizeof(target) ) {
	    return 0;
	}
    }
    else if( !target->eventPreAttack(this_object()) ) {
	return 0;
    }
    switch(type) {
        case ROUND_UNDEFINED:
        if( functionp(f) && !(functionp(f) & FP_OWNER_DESTED) ) {
	    return evaluate(f, target);
	}
        if( sizeof(weapons = GetWielded()) ) {
	    return eventWeaponRound(target, weapons);
	}
        else {
	    return eventMeleeRound(target, 0);
	}

        case ROUND_MAGIC:
	return eventMagicRound(target, f);

        case ROUND_MELEE:
        return eventMeleeRound(target, functionp(f) ? f : 0);

        case ROUND_WEAPON:
        return eventWeaponRound(target, functionp(f) ? f : GetWielded());

        case ROUND_OTHER:
        if( functionp(f) && !(functionp(f) & FP_OWNER_DESTED) ) {
	    return evaluate(f);
	}
        else {
	    return 0;
	}

        default: return 0;
    }
    return 0;
}

int eventWeaponRound(mixed target, mixed val) {
    object array weapons = 0;
    function f = 0;

    if( arrayp(val) ) {
	weapons = val;
    }
    else if( functionp(val) && !(functionp(val) & FP_OWNER_DESTED) ) {
	f = val;
    }
    else {
	return 0;
    }
    if( f ) {
	evaluate(f, target);
    }
    else {
	int count = sizeof(weapons);
	
	foreach(object weapon in weapons) {
	    if( !target ) {
		break;
	    }
	    eventWeaponAttack(target, weapon, count);
	}
    }
    return target->GetDying();
}

void eventWeaponAttack(object target, object weapon, int num){
    string weapon_type = weapon->GetWeaponType();
    int hands = weapon->GetHands();
    int level = target->GetLevel();
    int bonus = GetCombatBonus(level);
    int power, pro, con;
    
    if( target->GetDying() ) {
	return;
    }
    pro = CanWeapon(target, weapon_type, hands, num);
    power = random(pro);
    con = target->GetDefenseChance(target->GetSkillLevel(weapon_type +
							 " defense"));
    if( !TargetLimb ) { // If the thing stood still, I still missed
	eventTrainSkill(weapon_type + " attack", pro, 0, 0, bonus);
	if( hands > 1 ) {
	    eventTrainSkill("multi-hand", pro, 0, 0, bonus);
	}
	if( num > 1 ) {
	    eventTrainSkill("multi-weapon", pro, 0, 0, bonus);
	}
	SendWeaponMessages(target, -2, weapon, TargetLimb);
    }
    else if( !target->eventReceiveAttack(power, weapon_type, this_object()) ) {
	// Target avoided the attack
	eventTrainSkill(weapon_type + " attack", pro, con, 0, bonus);
	if( hands > 1 ) {
	    eventTrainSkill("multi-hand", pro, con, 0, bonus);
	}
	if( num > 1 ) {
	    eventTrainSkill("multi-weapon", pro, con, 0, bonus);
	}
        SendWeaponMessages(target, -1, weapon, TargetLimb);
    }
    else { // I hit, but how hard did I hit?
        int damage_type, damage, weapon_damage, actual_damage;
	  eventTrainSkill(weapon_type + " attack", pro*2, con, 1, bonus);
        damage_type = weapon->GetDamageType();
	  damage = (weapon->eventStrike(target) * pro)/100;
        damage = GetDamage(damage, weapon_type + " attack");
        actual_damage = target->eventReceiveDamage(this_object(), damage_type,
						   damage, 0, TargetLimb);
	if( actual_damage < 1 ) {
	    actual_damage = 0;
	}
	weapon_damage = damage - actual_damage;
	if( weapon_damage > 0 ) {
	    weapon->eventReceiveDamage(this_object(), BLUNT, weapon_damage,
				       0, TargetLimb);
	}
        if( !target->GetDying() ) {
	    SendWeaponMessages(target, actual_damage, weapon, TargetLimb);
	}
	else {
            eventPrint(possessive_noun(target) + " death is now on your "
		       "head.");
            target->eventPrint(GetName() + " is your murderer.");
            environment()->eventPrint(possessive_noun(target) +
				      " death is now on " +
				      possessive_noun(this_object())
				      + " head.", ({ this_object(), target }));
        }
    }
}

int eventMeleeRound(mixed target, function f) {
    string array limbs = GetLimbs() - ({ GetTorso() });
    int count = sizeof(limbs);
    int attacks;
    
    if( count < 1 ) {
	return 0;
    }
    if( !f || (functionp(f) & FP_OWNER_DESTED) ) {
        attacks = 1 + random(GetSkillLevel("melee attack"))/50;
	while( attacks-- ) {
	    if( target->GetDying() ) {
		break;
	    }
	    eventMeleeAttack(target, limbs[random(count)]);
	}
    }
    else {
	evaluate(f, target, limbs[random(count)]);
    }
    return target->GetDying();
}

void eventMeleeAttack(object target, string limb) {
    string array limbs;
    int pro, con;
    int chance;
    
    if( target->GetDying() ) {
	return;
    }
    pro = CanMelee(target);
    con = target->GetDefenseChance(target->GetSkillLevel("melee defense"));
    chance = random(pro);
    if( !TargetLimb ) { // I *really* missed
	SendMeleeMessages(target, -2);
	eventTrainSkill("melee attack", pro, 0, 0,
			GetCombatBonus(target->GetLevel()));
    }
    else if( !target->eventReceiveAttack(chance, "melee", this_object()) ) {
	// Enemy dodged my attack
        SendMeleeMessages(target, -1);
	eventTrainSkill("melee attack", pro, con, 0,
			GetCombatBonus(target->GetLevel()));
    }
    else {
	int x;
	
	// I hit, how hard?
	eventTrainSkill("melee attack", pro, con, 1,
			GetCombatBonus(target->GetLevel()));
	  x = GetDamage(3*chance/4, "melee attack");
        x = target->eventReceiveDamage(this_object(), BLUNT, x, 0,
				       TargetLimb);
	if( !target->GetDying() ) {
	    SendMeleeMessages(target, (x > 0) ? x : 0, TargetLimb);
	}
        else {
            eventPrint(possessive_noun(target) + " death is now "
		       "on your head.");
            target->eventPrint(GetName() + " is your murderer.");
            environment()->eventPrint(possessive_noun(target) +
				      " death is now on " +
				      possessive_noun(this_object()) +
				      " head.", ({ this_object(), target }));
        }
    }
}

int eventMagicRound(mixed target, function f) {
    evaluate(f, target);
    return target->GetDying();
}

mixed eventBite(object target) {
    int pro = CanMelee(target);
    int con = target->GetDefenseChance(target->GetSkillLevel("melee defense"));
    int x = random(pro);
    
    if( environment() != environment(target) ) {
	eventPrint(target->GetName() + " has gone away.");
	return 1;
    }
    if( TargetLimb ) {
	if( target->eventReceiveAttack(x, "melee", this_object()) ) { 
	    x = GetDamage(pro/4, "melee attack");
	    x = target->eventReceiveDamage(this_object(), KNIFE, x, 0,
					   TargetLimb);
	    if( x < 1 ) {
		target->eventPrint(possessive_noun(this_object()) + " bite "
				   "is nothing more than a pinch.");
		eventPrint("Your bite is nothing more than a pinch.");
		environment()->eventPrint(possessive_noun(this_object()) +
					  " bite is nothing more than a "
					  "pinch.",
					  ({ target, this_object() }));
	    }
	    else {
		target->eventPrint(GetName() + " bites you in the " +
				   TargetLimb + "!");
		eventPrint("You bite " + target->GetName() + " in the " +
			   TargetLimb + "!");
		environment()->eventPrint(GetName() + " bites " +
					  target->GetName() + "in the " +
					  TargetLimb + "!",
					  ({ target, this_object() }));
	    }
	    eventTrainSkill("melee attack", pro, con, 1,
			    GetCombatBonus(target->GetLevel()));
	}
	else {
	    target->eventPrint("You avoid " + possessive_noun(this_object()) +
			       " bite.");
	    eventPrint(target->GetName() + " avoids your bite.");
	    environment()->eventPrint(target->GetName() + " avoids " +
				      possessive_noun(this_object()) +
				      " bite.",
				      ({ this_object(), target }));
    	    eventTrainSkill("melee attack", pro, con, 0,
			    GetCombatBonus(target->GetLevel()));
	}
    }
    else {
	eventPrint("You flounder about like a buffoon.");
	environment()->eventPrint(GetName() + " flounders about like a "
				  "buffoon.", this_object());
    }
    return 1;
}

int eventPreAttack(object agent) {
    if( agent == this_object() ) {
	return 0;
    }
    if( environment()->GetProperty("no attack") ) {
	return 0;
    }
    if( GetDying() ) {
	return 0;
    }
    if( playerp(this_object()) && playerp(agent) ) { // No PK
        if( !environment()->CanAttack( agent, this_object() ) ) {
            return 0;
	}
    }
    if( AddEnemy(agent) ) {
	AddHostile(agent);
    }
    return 1;
}

varargs int eventReceiveAttack(int speed, string def, object agent) {
    int x, y, pro, level, bonus;

    if( !agent ) {
	agent = previous_object();
    }
    if( !living(agent) ) {
	level = 1;
	bonus = 1;
    }
    else {
	level = agent->GetLevel();
	bonus = GetCombatBonus(level);
    }
    if( AddEnemy(agent) ) {
	AddHostile(agent);
    }
    if( def == "magic" ) {
	pro = GetMagicResistance();
        if( (x = random(pro)) > speed ) {
	    eventTrainSkill("magic defense", pro, speed, 1, bonus);
            return 0;
        }
        else {
	    eventTrainSkill("magic defense", pro, speed, 0, bonus);
            return 1;
        }
    }
    else {
	pro = GetDefenseChance(GetSkillLevel(def + " defense"));
	x = random(pro = pro/2);
        if( x > speed ) {
	    eventTrainSkill(def + " defense", pro, speed, 1, bonus);
            return 0;
        }
        else {
	    eventTrainSkill(def + " defense", pro, speed, 0, bonus);
            return 1;
        }
    }
}

void eventKillEnemy(object ob) {
    int level;
    
    if( !ob ) return;
    level = ob->GetLevel();
    if( member_array(ob, GetHostiles()) == -1 ) {
        int x;

	eventTrainSkill("murder", GetLevel(), level, 1,GetCombatBonus(level)); 
        x = (int)ob->GetMorality();
        if( x > 0 ) x = -x;
        else if( GetMorality() > 200 ) x = 100;
        else x = 0;
        eventMoralAct(x);
    }
}

void eventDestroyEnemy(object ob) {
    int level;
    
    if( !ob ) return;
    level = ob->GetLevel();
    eventTrainSkill("faith", GetLevel(), level, 1, GetCombatBonus(level));
}

void eventEnemyDied(object ob) {
    if( !ob ) return;
    Enemies -= ({ ob });
    Hostiles -= ({ ob });
}

varargs int eventReceiveDamage(object agent, int type, int x, int internal,
			       mixed limbs) {
    int hp;
    
    x = race::eventReceiveDamage(agent, type, x, internal, limbs);
    if( !Wimpy ) return x;
    if( (hp = GetHealthPoints()) < 1 ) return x;
    if( Wimpy < percent(hp, GetMaxHealthPoints()) )
      return x;
    call_out((: eventWimpy :), 0);
    return x;
}

mixed eventTurn(object who) {
    int defense;
    
    if( !GetUndead() ) {
	return 0;
    }
    if( GetProperty("no turn") ) {
	if( !who ) {
	    return 0;
	}
	else {
	    int x = GetProperty("no turn");

	    environment(who)->eventPrint("The power of the undead "
					 "turns on " + who->GetName() +
					 ".", who);
	    who->eventPrint("The power of the undead turns on you.");
	    if( x > random(100) + 1 ) {
		who->eventDie(this_object());
	    }
	    else {
		who->eventReceiveDamage(this_object(), MAGIC, random(50), 1);
	    }
	    return 0;
	}
    }
    if( !who ) {
	race::eventTurn(who);
	return 1;
    }
    defense = GetMagicResistance();
    if( who->GetSkillLevel("faith") < defense ) {
	who->eventPrint("You writhe in pain.");
	environment(who)->eventPrint(who->GetName() + " writhes in pain.",
				     who);
	who->eventReceiveDamage(this_object(), MAGIC, random(defense), 1);
	eventTrainSkill("magic defense", defense, who->GetSkillLevel("faith"),
			1, GetCombatBonus(who->GetLevel()));
	return 0;
    }
    race::eventTurn(who);
    return 1;
}

int eventWimpy() {
    object env = environment();
    string dir, cmd;

    if( !env || !GetInCombat() ) {
	return 0;
    }
    cmd = WimpyCommand || "go out";
    if( (sscanf(cmd, "go %s", dir) && !((string)env->GetExit(dir))) ||
       (sscanf(cmd, "enter %s", dir) && !((string)env->GetEnter(dir))) ) {
	string *tmp;
	
	tmp = filter((string *)environment()->GetExits(),
		     (: !((string)environment()->GetDoor($1)) :));
	if( !sizeof(tmp) ) {
	    tmp = filter((string *)environment()->GetEnters(),
			 (: !((string)environment()->GetDoor($1)) :));
	    if( !sizeof(tmp) ) {
		eventPrint("You need to escape, but you have nowhere to go!");
		return 0;
	    }
	    cmd = "enter " + tmp[random(sizeof(tmp))];
	}
	else cmd = "go " + tmp[random(sizeof(tmp))];
    }
    return eventForce(cmd);
}

static void heart_beat() {
    race::heart_beat();
    if( GetSleeping() || GetDying() ) {
	return;
    }
    if( cParalyzed > 0 ) {
        cParalyzed--;
        if( cParalyzed < 1 ) {
            function f;

            f = fParalyzed;
            fParalyzed = 0;
            if( functionp(f) && !(functionp(f) & FP_OWNER_DESTED) ) {
		evaluate(f);
	    }
            else {
		eventPrint("You can move again.");
	    }
        }
        return;
    }
    if( sizeof(Enemies) ) {
        object ob;

        SortEnemies();
        if( SpecialTargets ) {
	    foreach(object target in SpecialTargets) {
                object tmp;

                if( objectp(SetCurrentEnemy(target)) ) {
		    break;
		}
            }
            eventExecuteAttack(SpecialTargets);
            SpecialTargets = 0;
        }
        else if( ob = ResetCurrentEnemy() ) {
	    eventExecuteAttack(ob);
	}
    }
    else if( tNextRound != ROUND_UNDEFINED && functionp(fNextRound) ) {
        function f;

        f = fNextRound;
        tNextRound = ROUND_UNDEFINED;
        evaluate(f);
    }
}
