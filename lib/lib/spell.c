/*    /lib/spell.c
 *    From Dead Souls LPMud
 *    An object all spells inherit.
 *    Created by Descartes of Borg 951027
 *    Version: @(#) spell.c 1.22@(#)
 *    Last modified: 96/12/17
 */

#include <lib.h>
#include <magic.h>
#include <damage_types.h>

inherit LIB_DAEMON;
inherit LIB_HELP;

private int          AutoDamage      = -1;            // perform dmg?
private int          AutoHeal        = -1;            // perform healing?
private string       Conjure         = 0;             // file to clone
private int array    Damage          = ({ 0, 0 });    // base, random
private mixed array  Messages        = ({});          // damage/heal messages
private int          DamageType      = MAGIC;         // damage type done
private int          Difficulty      = 0;             // 1-100 scale
private int array    Healing         = ({ 0, 0 });    // base, random
private int array    MagicCost       = ({ 0, 0 });    // base, random
private int          Morality        = 0;             // bad? good?
private string array Religions       = 0;             // limit who can cast
private int          RemoteTargets   = 0;             // can targets be remote
private int          RequiredMagic   = 0;             // min magic pts to cast
private int          RequiredStamina = 0;             // min stam pts to cast
private string array Rules           = ({});          // spell rules
private mapping      Skills          = ([]);          // skill requirements
private string       SpellName       = "";            // name of spell
private int          SpellType       = SPELL_HEALING; // spell type
private int array    StaminaCost     = ({ 0, 0 });    // base, random
private int		   TrainingModifier= 1;			// training factor
private string       Verb            = "cast";        // use what verb?

/* ********************* spell.c attributes ************************ */
int GetAutoDamage() {
    return AutoDamage;
}

static int SetAutoDamage(int x) {
    return (AutoDamage = x);
}

int GetAutoHeal() {
    return AutoHeal;
}

static int SetAutoHeal(int x) {
    return (AutoHeal = x);
}

string GetConjure() {
    return Conjure;
}

static string SetConjure(string str) {
    return (Conjure = str);
}

int GetDamage() {
    int damage = Damage[0];

    if( Damage[1] ) {
	damage += random(Damage[1]);
    }
    return damage;
}

varargs static void SetDamage(int type, mixed array rest...) {
    DamageType = type;
    if( arrayp(rest[0]) ) {
	rest = rest[0];
    }
    Damage[0] = rest[0];
    if( sizeof(rest) == 2 ) {
	Damage[1] = rest[1];
    }
    return;
}

int GetDamageType() {
    return DamageType;
}

int GetDifficulty() {
    return Difficulty;
}

static int SetDifficulty(int x) {
    return (Difficulty = x);
}

string GetErrorMessage() {
    string rule = Rules[0];

    switch(rule) {
    case "":
	if( Verb == "pray" ) {
	    return "Just pray for it.";
	}
	else {
	    return "Simply cast it?";
	}

    case "LIV":
	if( Verb == "pray" ) {
	    return "Pray for it for whom?";
	}
	else {
	    return "Cast it on whom?";
	}

    case "OBJ": case "STR": 
	if( Verb == "pray" ) {
	    return "Pray for it for what?";
	}
	else {
	    return "Cast it on what?";
	}

    case "STR of LIV":
	if( Verb == "pray" ) {
	    return "Pray for it for whom against what?";
	}
	else {
	    return "Cast it on what of whom?";
	}

    case "for LIV": 
	return "Pray for it for whom?";

    case "for OBJ":
	return "Pray for it for what?";

    case "against STR":
	if( Verb == "pray" ) {
	    return "Pray against what?";
	}
	else {
	    return "Cast against what?";
	}
	
    case "against STR for LIV":
	return "Pray against what for whom?";
    }
    if( Verb == "pray" ) {
	return "Pray for it?";
    }
    return "Cast it?";
}

int GetHealing() {
    return Healing[0] + random(Healing[1]);
}

static varargs int array SetHealing(mixed args...) {
    Healing[0] = args[0];
    if( sizeof(args) == 2 ) {
	Healing[1] = args[1];
    }
    return Healing;
}

int GetMagicCost() {
    return MagicCost[0] + random(MagicCost[1]);
}

static varargs int array SetMagicCost(mixed args...) {
    MagicCost[0] = args[0];
    if( sizeof(args) == 2 ) {
	MagicCost[1] = args[1];
    }
    return MagicCost;
}

varargs string array GetMessage(int damage, int healing) {
    int max, div, i;

    if( damage < 1 ) {
	return Messages[0];
    }
    if( sizeof(Messages) == 2 ) {
	return Messages[1];
    }
    if( healing) {
	max = Healing[0] + Healing[1];
    }
    else {
	max = Damage[0] + Damage[1];
    }
    div = max/(sizeof(Messages) - 1);
    i = (damage/div) + 1;
    if( i >= sizeof(Messages) ) {
	i = sizeof(Messages)-1;
    }
    return Messages[i];
}

static mixed array SetMessages(mixed array messages) {
    return (Messages = messages);
}

int GetMorality() {
    return Morality;
}

static int SetMorality(int x) {
    return (Morality = x);
}

string array GetReligions() {
    return copy(Religions);
}

varargs static string array SetReligions(string array religions...) {
    return (Religions = religions);
}

int GetRemoteTargets() {
    return RemoteTargets;
}

static int SetRemoteTargets(int x) {
    return (RemoteTargets = x);
}

int GetRequiredMagic() {
    return RequiredMagic;
}

static int SetRequiredMagic(int x) {
    return (RequiredMagic = x);
}

int GetRequiredStamina() {
    return RequiredStamina;
}

static int SetRequiredStamina(int x) {
    return (RequiredStamina = x);
}

int GetRequiredSkill(string skill) {
    return Skills[skill];
}

string array GetRules() {
    return Rules;
}

varargs static string array SetRules(mixed args...) {
    if( !args ) {
	args = ({ "" });
    }
    else if( arrayp(args[0]) ) {
	args = args[0];
    }
    return (Rules = args);
}

string array GetSkills() {
    return keys(Skills);
}

static mapping SetSkills(mapping mp) {
    return (Skills = mp);
}

string GetSpell() {
    return SpellName;
}

static string SetSpell(string str) {
    return (SpellName = str);
}

int GetSpellType() {
    return SpellType;
}

static int SetSpellType(int x) {
    return (SpellType = x);
}

int GetStaminaCost() {
    return StaminaCost[0] + random(StaminaCost[1]);
}

static varargs int array SetStaminaCost(mixed args...) {
    StaminaCost[0] = args[0];
    if( sizeof(args) == 2 ) {
	StaminaCost[1] = args[1];
    }
    return StaminaCost;
}

varargs object array GetTargets(object who, mixed args...) {
    int count = sizeof(args);
    int attack = (SpellType == SPELL_COMBAT);
    object def;

    if( attack ) {
	def = who->GetCurrentEnemy();
    }
    else {
	def = who;
    }
    if( Verb == "cast" ) {
	if( !count) {
	    if( member_array("", Rules) == -1 ) {
		return 0;
	    }
	    else {
		if( def ) {
		    return ({ def });
		}
		else {
		    return 0;
		}
	    }
	}
	else if( count == 1 ) { // on LIV || on STR
	    if( objectp(args[0]) ) { // on LIV
		if( !living(args[0]) ) {
		    return 0;
		}
		if( member_array("LIV", Rules) == -1 ) {
		    return 0;
		}
		else {
		    return ({ args[0] });
		}
	    }
	    if( stringp(args[0]) ) { // on STR
		int which = member_array("against STR", Rules);
		
		if( which == -1 && member_array("STR", Rules) == -1 ) {
		    return 0;
		}
		else {
		    if( def ) {
			return ({ def });
		    }
		    else {
			return 0;
		    }
		}
	    }
	}
	if( objectp(args[1]) ) { // on STR of LIV
	    if( !living(args[1]) ) {
		return 0;
	    }
	    if( member_array("STR of LIV", Rules) == -1 ) {
		return 0;
	    }
	    else {
		return ({ args[1] });
	    }
	}
    }
    else {
	if( !count) {
	    if( member_array("", Rules) == -1 ) {
		return 0;
	    }
	    else {
		if( def ) {
		    return ({ def });
		}
		else {
		    return 0;
		}
	    }
	}
	else if( count == 1 ) { // against STR
	    if( objectp(args[0]) ) {
		if( !sizeof(({ "for LIV", "for OBJ" }) & Rules) ) {
		    return 0;
		}
		if( !living(args[0]) && member_array("for OBJ",Rules) == -1 ) {
		    return 0;
		}
		return ({ args[0] });
	    }
	    if( member_array("against STR", Rules) == -1 ) {
		return 0;
	    }
	    else {
		if( def ) {
		    return ({ def });
		}
		else {
		    return 0;
		}
	    }
	}
	if( objectp(args[1]) ) { // against STR for LIV
	    if( !living(args[1]) ) {
		return 0;
	    }
	    if( member_array("against STR for LIV", Rules) == -1 ) {
		return 0;
	    }
	    else {
		return ({ args[1] });
	    }
	}
    }
    return 0;
}

static int SetTrainingModifier(int modifier) {
    return TrainingModifier = modifier;
}

int GetTrainingModifier() {
    return TrainingModifier;
}

string GetVerb() {
    return Verb;
}

static string SetVerb(string verb) {
    return (Verb = verb);
}

/* ******************** spell.c modals *************************** */
static int CanAttack(object who, object array enemies, int power) {
    int i, maxi = sizeof(enemies);
    int hits = 0;
    int misses = 0;
    int hit_count = 0;
    int miss_count = 0;
    int hit_con = 0;
    int miss_con = 0;
    int con, bonus;
    
    if( !maxi ) {
	return -1;
    }
    for(i=0; i<maxi; i++) {
	mixed area;
	
	if( !enemies[i] ) {
	    continue;
	}
	if( !enemies[i]->eventPreAttack(who) ) {
	    enemies[i] = 0;
	    continue;
	}
	if( environment(enemies[i]) != environment(who) ) {
	    if( RemoteTargets ) {
		area = 0;
	    }
	    else {
		enemies[i] = 0;
		continue;
	    }
	}
	else {
	    area = environment(who);
	}
	moral_act(who, enemies[i], GetMorality());
	if( !enemies[i]->eventReceiveAttack(power, "magic", who) ) {
	    misses += enemies[i]->GetLevel();
	    miss_count++;
	    miss_con += enemies[i]->GetMagicResistance();
	    send_messages("repel", "$target_name $target_verb "
			  "$agent_possessive_noun magic attack.", who,
			  enemies[i], area);
	    enemies[i] = 0;
	}
	else {
	    hit_count++;
	    hits += enemies[i]->GetLevel();
	    hit_con += enemies[i]->GetMagicResistance();
	}
    }
    if( miss_count > 0 ) {
	bonus = who->GetCombatBonus(misses/miss_count);
      bonus *= GetTrainingModifier();
	foreach(string skill in GetSkills()) {
	    who->eventTrainSkill(skill, power/(hit_count+miss_count),
				 miss_con/miss_count, 0, bonus);
	}			 
    }
    if( hit_count < 1 ) {
	return -1;
    }
    bonus = who->GetCombatBonus(hits/hit_count);
    bonus *= GetTrainingModifier();	
    foreach(string skill in GetSkills()) {
	who->eventTrainSkill(skill, power/(hit_count+miss_count),
			     hit_con/hit_count, 1, bonus);
    }
    return 1;
}

varargs int CanCast(object who, int level, string limb, object array targets) {
    string array skills = GetSkills();
    int count = sizeof(skills);
    int cost = GetMagicCost();
    int x;

    if( Religions ) {
	if( member_array(who->GetReligion(1), Religions) == -1 ) {
	    who->eventPrint("Your deity does not have that kind of power."); 
	    return 0;
	}
    }
    if( cost > 0 ) {
	who->AddMagicPoints(-cost);
    }
    cost = GetStaminaCost();
    if( cost > 0 ) {
	who->AddStaminaPoints(-cost);
    }
    if( AutoHeal != -1 ) {
	int i, maxi = sizeof(targets);

	for(i=0; i<maxi; i++) {
	    int hp, max_hp;
	    
	    if( !targets[i] ) {
		continue;
	    }
	    if( limb ) {
		if( member_array(limb, targets[i]->GetLimbs()) == -1 ) {
		    send_messages("have", "$target_name $target_verb no " +
				  limb + ".", who, targets[i]);
		    targets[i] = 0;
		    continue;
		}
	    }
	    hp = targets[i]->GetHealthPoints(limb);
	    max_hp = targets[i]->GetMaxHealthPoints(limb);
	    if( max_hp - hp < (Healing[0]+Healing[1])/10 + 1 ) {
		if( limb ) {
		    send_messages("", "$target_possessive_noun " + limb +
				  " needs no healing.", who, targets[i]);
		}
		else {
		    send_messages("need", "$target_name $target_verb no "
				  "healing.", who, targets[i]);
		}
		targets[i] = 0;
		continue;
	    }
	}
	if( !sizeof(filter(targets, (: $1 :))) ) {
	    return 0;
	}
    }	
    foreach(string skill in skills) {
	level += who->GetSkillLevel(skill);
    }
    level = level/(count+1);
    x = who->GetMagicChance(level/2 + random(level/2));
    if( x < GetDifficulty() ) { // Can't even cast it...
	foreach(string skill in skills) {
	    who->eventTrainSkill(skill, level, GetDifficulty(),
					 0, GetTrainingModifier());
	}
	who->eventPrint("You must have gotten the words wrong.");
	return 0;
    }
    if( AutoDamage != -1 ) {
	if( CanAttack(who, targets, x) == - 1 ) {
	    who->eventPrint("Your powers fail you.");
	    return 0;
	}
    }
    else {
	if( GetMorality() ) {
	    moral_act(who, 0, GetMorality());
	}
	foreach(string skill in skills) {
	    who->eventTrainSkill(skill, level, GetDifficulty(), 
					 1, GetTrainingModifier());
	}
    }
    return 1;
}

/* ******************** spell.c events *********************** */
/* -1 means this did nothing
 * otherwise return damage amount for SPELL_DAMAGE
 * or heal amount for SPELL_HEAL
 */
varargs int eventCast(object who, int level, mixed limb, object array targets) {
    if( GetConjure() ) {
	object ob = new(GetConjure());

	if( !ob ) {
	    who->eventPrint("An error occurred in conjuring.");
	    return 1;
	}
	send_messages(Messages[0][0], Messages[0][1], who, 0,environment(who));
	if( !ob->eventMove(who) ) {
	    send_messages("drop", "$agent_name could not carry " +
			  ob->GetShort() + " and $agent_verb it!", who, 0,
			  environment(who));
	    ob->eventMove(environment(who));
	}
	return 1;
    }
    if( AutoHeal != -1 ) {
	mapping messages = ([]);
	int total_healing = 0;

	foreach(object target in targets) {
	    string array tmp;
	    int healing;

	    if( !target ) {
		continue;
	    }
	    healing = (GetHealing() * level)/100;
	    healing = target->eventHealDamage(healing, AutoHeal, limb);
	    total_healing += healing;
	    tmp = GetMessage(healing, 1);
	    if( !messages[tmp[1]] ) {
		messages[tmp[1]] = ([ tmp[0] : ({ target }) ]);
	    }
	    else {
		if( !messages[tmp[1]][tmp[0]] ) {
		    messages[tmp[1]][tmp[0]] = ({ target });
		}
		else {
		    messages[tmp[1]][tmp[0]] += ({ target });
		}
	    }
	}
	foreach(string message, mapping tmp in messages) {
	    foreach(string verb, object array obs in tmp) {
		send_messages(verb, message, who, obs,
			      environment(who), ([ "$limb" : limb ]));
	    }
	}
	if( sizeof(targets) ) {
	    return total_healing/sizeof(targets);
	}
	else {
	    return 0;
	}
    }
    if( AutoDamage != -1 ) {
	mapping messages = ([]);
	int total_damage = 0;

	foreach(object target in targets) {
	    string array tmp;
	    int damage;

	    if( !target ) {
		continue;
	    }
	    damage = (GetDamage() * level)/100;
	    damage = target->eventReceiveDamage(who, GetDamageType(), damage,
						AutoDamage, limb);
	    total_damage += damage;
	    tmp = GetMessage(damage);
	    if( !messages[tmp[1]] ) {
		messages[tmp[1]] = ([ tmp[0] : ({ target }) ]);
	    }
	    else {
		if( !messages[tmp[1]][tmp[0]] ) {
		    messages[tmp[1]][tmp[0]] = ({ target });
		}
		else {
		    messages[tmp[1]][tmp[0]] += ({ target });
		}
	    }
	}
	foreach(string message, mapping tmp in messages) {
	    foreach(string verb, object array obs in tmp) {
		send_messages(verb, message, who, obs,
			      environment(who), ([ "$limb" : limb ]));
	    }
	}
	if( sizeof(targets) ) {
	    return total_damage/sizeof(targets);
	}
	else {
	    return 0;
	}
    }
    return -1;
}

varargs mixed eventParse(object who, mixed array args...) {
    int count = sizeof(args);

    if( count < 1 ) {
	if( member_array("", Rules) == -1 ) {
	    return GetErrorMessage();
	}
	return ({});
    }
    if( Verb == "cast" ) {
	if( count == 1 ) {
	    if( objectp(args[0]) ) {
		if( !living(args[0]) ) {
		    if( member_array("OBJ", Rules) == -1 ) {
			return GetErrorMessage();
		    }
		    return ({ args[0] });
		}
		if( sizeof(({ "OBJ", "LIV"}) & Rules) == 0 ) {
		    return GetErrorMessage();
		}
		return ({ args[0] });
	    }
	    if( stringp(args[0]) ) {
		int which = member_array("against STR", Rules);
		
		if( which == -1 && member_array("STR", Rules) == -1 ) {
		    return GetErrorMessage();
		}
		return ({ args[0] });
	    }
	}
	if( member_array("STR of LIV", Rules) == -1 ) {
	    return GetErrorMessage();
	}
	if( stringp(args[0]) && objectp(args[1]) && living(args[1]) ) {
	    return ({ args[0], args[1] });
	}
	return "Cast it on what of whom?";
    }
    else {
	if( count == 1 ) {
	    if( objectp(args[0]) ) {
		if( sizeof(({ "for OBJ", "for LIV" }) & Rules) ) {
		    if( !living(args[0]) ) {
			if( member_array("for OBJ", Rules) == -1 ) {
			    return GetErrorMessage();
			}
		    }
		    return ({ args[0] });
		}
		return GetErrorMessage();
	    }
	    if( member_array("against STR", Rules) == -1 ) {
		return GetErrorMessage();
	    }
	    return ({ args[0] });
	}
	if( member_array("against STR for LIV", Rules) == -1 ) {
	    return GetErrorMessage();
	}
	if( stringp(args[0]) && objectp(args[1]) && living(args[1]) ) {
	    return ({ args[0], args[1] });
	}
	return "Pray for it against what for whom?";
    }
}

/* ***************** spell.c driver applies ******************** */
static void create() {
    daemon::create();
    SetNoClean(1);
}
