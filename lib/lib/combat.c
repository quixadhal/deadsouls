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

#ifndef MAX_ATTACKS_PER_HB
#define MAX_ATTACKS_PER_HB 40
#endif
#ifndef MAX_POWER
#define MAX_POWER 9001
#endif
#ifndef MAX_SKILL
#define MAX_SKILL 1000
#endif


inherit LIB_RACE;
inherit LIB_CLASSES;
inherit LIB_COMBATMSG;

private int Wimpy, Dead;
private string WimpyCommand;
private static int cParalyzed, tNextRound, AttacksPerHB;
private static string TargetLimb, Party;
private static object CurrentEnemy, genv;
private static function fParalyzed, fNextRound;
private static object *Hostiles, *Enemies, *SpecialTargets, *NonTargets;
private static object *PriorEnemies;

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
int eventWimpy(int i);

static void create(){
    race::create();
    classes::create();
    Hostiles = ({});
    Enemies = ({});
    NonTargets = ({});
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

int GetVisibility(){
    int x = this_object()->GetEffectiveVision();
    if( x > 5 || x < 3 ) return 0;
    if( x == 4 ) return 2;
    else return 1;
}

int GetDead(){
    return Dead;
}

int SetDead(int i){
    if(!i) Dead = 0;
    else Dead = 1;
    return Dead;
}

object array GetEnemies(){
    return Enemies;
}

int AddEnemy(object ob){
    if( !ob || (member_array(ob, Enemies) != -1) ){
        return 0;
    }
    if( !living(ob) ){
        return 0;
    }
    Enemies += ({ ob });
    return 1;
}

int RemoveEnemy(object ob){
    if( !ob || (member_array(ob, Enemies) == -1) ){
        return 0;
    }
    Enemies -= ({ ob });
    return 1;
}

object SetCurrentEnemy(object ob){
    if( !ob ){
        return (CurrentEnemy = 0);
    }
    if( !living(ob) ){
        return CurrentEnemy;
    }
    if( member_array(ob, Enemies) == -1 ){
        AddEnemy(ob);
    }
    return (CurrentEnemy = ob);
}

static object ResetCurrentEnemy(){
    object array obs;
    mixed ret;

    obs = filter(GetEnemies(), (: $1 && ( environment() == environment($1) 
                    || environment() == $1 || environment($1) == this_object()):));
    if( !sizeof(obs) ){
        ret = 0;
    }
    else ret = SetCurrentEnemy(obs[random(sizeof(obs))]);
    return ret;
}

object GetCurrentEnemy(){
    return CurrentEnemy;
}

private static void SortEnemies(){
    if( !sizeof(Enemies = filter(Enemies, (: ($1 && living($1)) :))) ){
        Hostiles = ({});
        CurrentEnemy = 0;
        return;
    }
    Hostiles = (Hostiles & Enemies);
}

mixed *AddNonTargets(mixed val){
    if(!arrayp(val)) val = ({ val });
    foreach(object member in val){
        if(member_array(member,NonTargets) == -1){
            if(base_name(this_object()) != base_name(member)) NonTargets += ({ member });
        }
    }
    return NonTargets;
}

mixed *RemoveNonTargets(mixed val){
    if(!val) val = ({});
    if(!arrayp(val)) val = ({ val });
    if(!sizeof(val)){
        NonTargets = ({});
        return NonTargets;
    }
    foreach(object member in val){
        if(member_array(member,NonTargets) != -1){
            NonTargets -= ({ member });
        }
    }
    return NonTargets;
}

object *GetNonTargets(){
    return NonTargets;
}


int AddHostile(object ob){
    if( !ob || (member_array(ob, Hostiles) != -1) ){
        return 0;
    }
    if( !living(ob) ){
        return 0;
    }
    Hostiles += ({ ob });
    return 1;
}

int RemoveHostile(object ob){
    if( !ob || (member_array(ob, Hostiles) == -1) ){
        return 0;
    }
    Hostiles -= ({ ob });
    return 1;
}

object array GetHostiles(){
    return Hostiles;
}

object array GetSpecialTarget(){
    return SpecialTargets;
}

object array SetSpecialTarget(object *cibles){
    if(cibles) SpecialTargets = cibles;
    return SpecialTargets;
}

varargs int SetParalyzed(int count, function f){
    if(count < 1){
        count = cParalyzed = 0;
        fParalyzed = 0;
    }
    else {
        fParalyzed = f;
        cParalyzed = count;
    }
    return count;
}

int GetParalyzed(){ return cParalyzed; }

int eventQuell(){
    object *truced;
    truced = get_livings(environment(this_object()));
    if(sizeof(Enemies)) PriorEnemies = Enemies;
    Enemies = ({});
    AddNonTargets(truced);
    return 1;
}

int eventUnQuell(){
    object *truced;
    truced = GetNonTargets();
    if(sizeof(PriorEnemies)) Enemies += PriorEnemies;
    if(sizeof(truced)) RemoveNonTargets(truced);
    return 1;
}

string SetParty(string str){
    if( file_name(previous_object()) != PARTY_D ) return Party;
    if( str ) AddChannel(str);
    else RemoveChannel(Party);
    return (Party = str);
}

string GetParty(){
    if(Party){
        if(member_array(Party, PARTY_D->GetParties()) == -1) return 0;
        if(member_array(this_object(), PARTY_D->GetPartyMembers(Party)) == -1){
            return 0;
        }
    }
    return Party;
}

varargs int SetAttack(mixed target, function callback, int type){
    int i;
    object env = room_environment();
    string *voibs = ({ "kill", "target", "smite", "waste", "hit", "attack" });

    /* Don't put stuff in the combat queue if the room is peaced */
    if(env && !target && env->GetProperty("no attack")){
        evaluate( callback );
        return 1;
    }
    if( objectp(target) ) target = ({ target });
    if( target ){
        if( member_array(this_object(), target) != -1 ) return 0;
        if(sizeof(NonTargets) && member_array(query_verb(),voibs) != -1){
            foreach(object individual in target){
                if(member_array(individual,NonTargets) != -1){
                    NonTargets -= ({ individual });
                }
            }
        }
        else if(sizeof(NonTargets)){
            foreach(object individual in target){
                if(member_array(individual,NonTargets) != -1) target -= ({ individual });
            }
            if(!sizeof(target)) return 0;
        }

        if(environment(target[0]) == this_object()){
            type = ROUND_INTERNAL;
        }
        if(environment(this_object()) == target[0]){
            type = ROUND_EXTERNAL;
        }

        if( !GetCurrentEnemy() ) call_out((: eventExecuteAttack :), 0, target);
        i = sizeof(target);
        while(i--) if( AddEnemy(target[i]) ) AddHostile(target[i]);
        if(!sizeof(SpecialTargets) ) SpecialTargets = target;
        else {
            int spec_targ_here;
            foreach(object t in SpecialTargets){
                if(t && present(t->GetKeyName(),environment(this_player()))) spec_targ_here = 1;
            }
            if(!spec_targ_here) SpecialTargets = target;
        }
    }
    fNextRound = callback;
    tNextRound = (type || ROUND_UNDEFINED);
    return 1;
}

int GetLevel(){
    return classes::GetLevel();
}

int GetInCombat(){
    return sizeof(filter(GetEnemies(),
                (: $1 && ( (environment($1) == environment()) 
                           || environment() == $1 || environment($1) ==
                           this_object()) :)));
}

int GetBaseStatLevel(string stat){
    return race::GetBaseStatLevel(stat);
}

float SetWimpy(float wimpy){
    return (Wimpy = wimpy);
}

float GetWimpy(){
    return Wimpy;
}

string SetWimpyCommand(string cmd){
    return (WimpyCommand = cmd);
}

string GetWimpyCommand(){
    return WimpyCommand;
}

int GetMaxCarry(){
    return race::GetMaxCarry();
}

int GetMagicChance(int val){
    val = GetStatLevel("intelligence")/2 + (3*val)/2;
    return val + GetLuck();
}

int GetMagicResistance(){
    int val = GetStatLevel("wisdom")/2 + (3*GetSkillLevel("magic defense"))/2;
    return val + GetLuck();
}

varargs int GetPenalty(object other){
    int other_penalty, ret = 0;
    if(other) other_penalty = other->GetPenalty();
    //Only players get a blindness penalty. 
    if(interactive() && (GetBlind() || GetVisibility() < 1)) ret += 10;
    if(GetParalyzed()) ret += 10;
    switch(GetPosition()){
        case POSITION_LYING :
            if(!(RACES_D->GetLimblessCombatRace(GetRace()))) ret += 10;
            break;
        case POSITION_SITTING : ret += 3;break;
        case POSITION_KNEELING : ret += 3;break;
    }
    if(other_penalty) ret -= other_penalty;
    if(ret < 0) ret = 0;
    return ret;
}

int GetCombatChance(int val){
    val = val + random((val * GetMobility())/50);
    val = (val/(3- GetVisibility()) + GetLuck());
    val = val/(GetPenalty() || 1);
    return val;
}

int GetDefenseChance(int val){
    val = (val * GetMobility())/50;
    val = (val/(3- GetVisibility()) + (GetLuck()/2));
    val = val/(GetPenalty() || 1);
    return val;
}

int GetCombatBonus(int level){
    int diff = level - GetLevel();

    if( diff >= 6 && diff < 16 ){
        return 4;
    }
    else if( diff >= 1 && diff < 6 ){
        return 3;
    }
    else if( diff >= -9 && diff < 1 ){
        return 2;
    }
    else if( diff >= -20 && diff < -9 ){
        return 1;
    }
    else {
        return 0;
    }
}

static int GetDamage(int power, string skill){
    int x = GetSkillLevel(skill);

    if( power < 1 ){
        return 0;
    }
    else if( power > MAX_POWER ){
        power = MAX_POWER;
    }
    if( x < 1 ){
        x = 1;
    }
    else if( x > MAX_SKILL ){
        x = MAX_SKILL;
    }
    x = (x * (power/2 + random(power/2))/10);
    x += (GetLuck()/2) + GetStatLevel("strength")/8;
    if( x < 1 ){ // negative luck or cursed strength
        return 1;
    }
    return x;
}

int CanWeapon(object target, string type, int hands, int num){
    string limb = target->GetRandomLimb(TargetLimb);
    int chance = (7*GetSkillLevel(type+" attack") + 
            3*GetStatLevel("coordination"))/10;
    int div = 2;
    int x, y;

    if(hands > 1){  
        if(GetSkillLevel("multi-hand")){
            chance = (chance/2) + 
                (GetSkillLevel("multi-hand")/25)*(chance/2);
        }
        else { /* If you are really strong you can use multihand a bit */
            chance *= GetStatLevel("strength")/100;
            div += (hands-1);
        }
    }
    if(num > 1){
        if(GetSkillLevel("multi-weapon")){
            chance = (chance/2) + 
                (GetSkillLevel("multi-weapon")/25)*(chance/2);
        }    
        else { /* If you are really coordinated you can use multiweap a bit */
            chance *= GetStatLevel("coordination")/100;
            div += (num-1);
        }
    }
    chance = GetCombatChance(chance/div);
    x = random(chance);
    y = random(10);
    if( x <= y ){
        if( x > y/2 ){ 
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


int CanMelee(object target){
    //if(environment(target) == this_object() ||
    //environment(this_object()) == target) return 100;
    if(!this_object()->GetMelee() && 
            this_object()->GetClass() != "fighter"){
        string limb = target->GetRandomLimb(TargetLimb);
        int chance = ( 6*this_object()->GetSkillLevel("melee attack") +
                2*GetStatLevel("coordination") )/20;
        int y = random(10);
        int x;

        chance = GetCombatChance(chance/3);
        x = random(chance);
        if( x <= y ){
            if( x > y/2 ){
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
    else {
        string limb = target->GetRandomLimb(TargetLimb);
        int chance = ( 7*this_object()->GetSkillLevel("melee attack") +
                3*GetStatLevel("coordination") )/10;
        int y = random(10);
        int x;

        chance = GetCombatChance(chance/2);
        x = random(chance);
        if( x <= y ){
            if( x > y/2 ){
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
}

static int Destruct(){
    if( GetParty() ) PARTY_D->eventLeaveParty(this_object());
    return 1;
}

/*  *****************   /lib/combat.c events  ***************** */

varargs int eventDie(mixed agent){
    object ob, env = room_environment();
    int x;
    if(this_object()->GetGodMode()) return 0;

    if(Dead) return 1;
    Dead = 1;

    x = race::eventDie(agent);
    if( x != 1 ){
        return x;
    }
    foreach(ob in GetEnemies()){
        if( ob ){
            ob->eventEnemyDied(this_object());
        }
    }
    Enemies = ({});
    flush_messages();
    return 1;
}

int eventExecuteAttack(mixed target){
    object array weapons;
    function f = fNextRound;
    int type = tNextRound;
    int position = GetPosition();
    string target_name = "";

    if(AttacksPerHB > MAX_ATTACKS_PER_HB) return 0;
    if(Dead) return 1;
    if(target->GetDead()) return 1;

    if(stringp(target)) target_name = target;
    if(objectp(target)) target_name = target->GetShort();

    AttacksPerHB++;

    fNextRound = 0;
    tNextRound = ROUND_UNDEFINED;

    if(this_object()->GetPacifist()){
        tell_object(this_object(),"As a pacifist, you choose not to fight.");
        return 0;
    }

    if( position == POSITION_LYING || position == POSITION_SITTING &&
            RACES_D->GetLimblessCombatRace(this_object()->GetRace()) != 1){
        if(this_object()->CanFly()){
            this_object()->eventFly();
        }
        else if(objectp(target) && (((this_object()->GetSize(1)) -
                        target->GetSize(1)) < 2) ){
            this_object()->eventPrint("You can't fight "+
                    target_name+" unless you are up!");
            return 0;
        }
    }

    if( arrayp(target) ){
        if( !f || (functionp(f) & FP_OWNER_DESTED) ){
            return 0; /* built in only handles 1 targ */
        }
        target = filter(target, function(object ob){
                if( !ob ){
                return 0;
                }
                if( !ob->eventPreAttack(this_object()) ){
                return 0;
                }
                return 1;
                });
        if( !sizeof(target) ){
            return 0;
        }
    }
    else if( !target->eventPreAttack(this_object()) ){
        return 0;
    }
    if(this_object()->GetClass() != "fighter"){
        this_object()->AddStaminaPoints(-1);
    }  
    switch(type){
        case ROUND_UNDEFINED: case ROUND_EXTERNAL:
            if( functionp(f) && !(functionp(f) & FP_OWNER_DESTED) ){
                return evaluate(f, target);
            }
            if( sizeof(weapons = GetWielded()) ){
                return eventWeaponRound(target, weapons);
            }
            else {
                return eventMeleeRound(target, 0);
            }

        case ROUND_INTERNAL:
            return eventMeleeRound(target, 0);

        case ROUND_MAGIC:
            return eventMagicRound(target, f);

        case ROUND_MELEE:
            return eventMeleeRound(target, functionp(f) ? f : 0);

        case ROUND_WEAPON:
            return eventWeaponRound(target, functionp(f) ? f : GetWielded());

        case ROUND_OTHER:
            if( functionp(f) && !(functionp(f) & FP_OWNER_DESTED) ){
                return evaluate(f);
            }
            else {
                return 0;
            }

        default: return 0;
    }
    return 0;
}

int eventWeaponRound(mixed target, mixed val){
    object array weapons = 0;
    function f = 0;

    if(AttacksPerHB > MAX_ATTACKS_PER_HB) return 0;
    if(Dead) return 1;
    if(target->GetDead()) return 1;
    AttacksPerHB++;
    if( arrayp(val) ){
        weapons = val;
    }
    else if( functionp(val) && !(functionp(val) & FP_OWNER_DESTED) ){
        f = val;
    }
    else {
        return 0;
    }
    if( f ){
        evaluate(f, target);
    }
    else {
        int count = sizeof(weapons);

        foreach(object weapon in weapons){
            if( !target ){
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
    int power, pro, con, fail;

    if(AttacksPerHB > MAX_ATTACKS_PER_HB) return 0;
    if(Dead) return;
    if(target->GetDead()) return;
    AttacksPerHB++;
    if( target->GetDying() ){
        return;
    }

    if(weapon->GetLive() && !interactive()){
        weapon->eventShoot(weapon, target);
        return;
    }
    if(GetPenalty() > random(11)) fail = 1;
    pro = CanWeapon(target, weapon_type, hands, num);
    power = random(pro);
    con = target->GetDefenseChance(target->GetSkillLevel(weapon_type +
                " defense"));
    if( !TargetLimb ){ // If the thing stood still, I still missed
        if(!estatep(target) && !fail) eventTrainSkill(weapon_type + " attack", pro, 0, 0, bonus);
        if( hands > 1 ){
            if(!estatep(target) && !fail) eventTrainSkill("multi-hand", pro, 0, 0, bonus);
        }
        if( num > 1 ){
            if(!estatep(target) && !fail) eventTrainSkill("multi-weapon", pro, 0, 0, bonus);
        }
        SendWeaponMessages(target, -2, weapon, TargetLimb);
    }
    else if( fail || !target->eventReceiveAttack(power, weapon_type, this_object()) ){
        // Target avoided the attack
        if(!estatep(target) && !fail) eventTrainSkill(weapon_type + " attack", pro, con, 0, bonus);
        if( hands > 1 ){
            if(!estatep(target) && !fail) eventTrainSkill("multi-hand", pro, con, 0, bonus);
        }
        if( num > 1 ){
            if(!estatep(target) && !fail) eventTrainSkill("multi-weapon", pro, con, 0, bonus);
        }
        SendWeaponMessages(target, -1, weapon, TargetLimb);
    }
    else { // I hit, but how hard did I hit?
        int damage_type, damage, weapon_damage, actual_damage, encumbrance;
        encumbrance = this_object()->GetEncumbrance();
        if(encumbrance > 20){
            tell_object(this_object(),"You struggle to fight while carrying stuff.");
        }
        if(!estatep(target)) eventTrainSkill(weapon_type + " attack", pro*2, con, 1, bonus);
        damage_type = weapon->GetDamageType();
        damage = (weapon->eventStrike(target) * pro)/(GetLevel()*2);
        damage = GetDamage(damage, weapon_type + " attack");
        damage -= encumbrance;
        if(damage < 0) damage = 0;
        actual_damage = target->eventReceiveDamage(this_object(), damage_type,
                damage, 0, TargetLimb);
        if( actual_damage < 0 ){
            actual_damage = 0;
        }
        weapon_damage = damage - actual_damage;
        if( weapon_damage > 0 ){
            weapon->eventReceiveDamage(this_object(), BLUNT, weapon_damage,
                    0, TargetLimb);
        }
        SendWeaponMessages(target, actual_damage, weapon, TargetLimb);
        if( target->GetDying() ){
        }
    }
}

int eventMeleeRound(mixed target, function f){
    string array limbs = GetLimbs() - ({ GetTorso() });
    int count = sizeof(limbs);
    int attacks;

    if(AttacksPerHB > MAX_ATTACKS_PER_HB) return 0;
    if(Dead) return 1;
    if(target->GetDead()) return 1;
    AttacksPerHB++;
    if( count < 2 ){
        if(RACES_D->GetLimblessCombatRace(this_object()->GetRace())){
            limbs = GetLimbs();
            count = sizeof(limbs);
        }
        else return 0;
    }
    if( !f || (functionp(f) & FP_OWNER_DESTED) ){
        attacks = 1 + random(this_object()->GetSkillLevel("melee attack"))/30;
        while( attacks-- ){
            if( target->GetDying() ){
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

void eventMeleeAttack(object target, string limb){
    int pro, con, autohit;
    int chance, fail;
    int canmelee = (this_object()->GetMelee() ||
            this_object()->GetClass() == "fighter");

    if(AttacksPerHB > MAX_ATTACKS_PER_HB) return;
    if( target->GetDead() || Dead || target->GetDying() ){
        return;
    }
    AttacksPerHB++;
    if(limb == "head" && this_object()->GetCanBite()){
        eventBite(target);
        return;
    }

    if(environment(target) == this_object() ||
            environment(this_object()) == target){
        con = 0;
        fail = 0;
        autohit = 1;
    }
    else if(GetPenalty() > random(11)) fail = 1;

    pro = CanMelee(target);
    if(undefinedp(con)){
        con = target->GetDefenseChance(target->GetSkillLevel("melee defense"));
    }
    chance = random(pro);
    if( !TargetLimb ){ // I *really* missed
        SendMeleeMessages(target, -2);
        if(!estatep(target)) eventTrainSkill("melee attack", pro, 0, 0,
                GetCombatBonus(target->GetLevel()));
    }
    else if( fail || (!autohit && 
                !target->eventReceiveAttack(chance, "melee", this_object())) ){
        // Enemy dodged my attack
        SendMeleeMessages(target, -1);
        if(!estatep(target) && !fail) eventTrainSkill("melee attack", pro, con, 0,
                GetCombatBonus(target->GetLevel()));
    }
    else { // I hit, how hard?
        int x, encumbrance;
        encumbrance = this_object()->GetEncumbrance();
        if(encumbrance > 20){
            tell_object(this_object(),"You struggle to fight while carrying stuff.");
        }
        if(!estatep(target)) eventTrainSkill("melee attack", pro, con, 1,
                GetCombatBonus(target->GetLevel()));
        if(canmelee) x = GetDamage(3*chance/4, "melee attack");
        else x = GetDamage(3*chance/20, "melee attack");
        x -= encumbrance;
        if(x < 0) x = 0;
        x = target->eventReceiveDamage(this_object(), BLUNT, x, 0,
                TargetLimb);
        SendMeleeMessages(target, (x > 0) ? x : 0, TargetLimb);
        if( target->GetDying() ){
        }
    }
}

int eventMagicRound(mixed target, function f){
    if(AttacksPerHB > MAX_ATTACKS_PER_HB) return 0;
    if(target->GetDead()) return 1;
    AttacksPerHB++;
    evaluate(f, target);
    return target->GetDying();
}

mixed eventBite(object target){
    int fail, internal;
    object env = room_environment();
    int pro = CanMelee(target);
    int con = target->GetDefenseChance(target->GetSkillLevel("melee defense"));
    int x = random(pro);
    if(AttacksPerHB > MAX_ATTACKS_PER_HB) return 0;
    if(target->GetDead()) return 1;
    if(environment(target) == this_object() ||
            environment(this_object()) == target) internal = 1;
    else if(GetPenalty() > random(11)) fail = 1;
    AttacksPerHB++;
    if( !internal && environment() != environment(target) ){
        this_object()->eventPrint(target->GetName() + " has gone away.");
        return 1;
    }
    if( !fail && TargetLimb ){
        if( target->eventReceiveAttack(x, "melee", this_object()) ){ 
            x = GetDamage(pro*2, "melee attack");
            x = target->eventReceiveDamage(this_object(), BITE, x, 0,
                    TargetLimb);
            if( x < 1 ){
                target->eventPrint(possessive_noun(this_object()) + " bite "
                        "is nothing more than a pinch.");
                this_object()->eventPrint("Your bite is nothing more than a pinch.");
                env->eventPrint(possessive_noun(this_object()) +
                        " bite is nothing more than a "
                        "pinch.",
                        ({ target, this_object() }));
            }
            else {
                target->eventPrint(GetName() + " bites you in the " +
                        TargetLimb + "!");
                this_object()->eventPrint("You bite " + target->GetName() + " in the " +
                        TargetLimb + "!");
                env->eventPrint(GetName() + " bites " +
                        target->GetName() + " in the " +
                        TargetLimb + "!",
                        ({ target, this_object() }));
            }
            if(!estatep(target)) eventTrainSkill("melee attack", pro, con, 1,
                    GetCombatBonus(target->GetLevel()));
        }
        else {
            target->eventPrint("You avoid " + possessive_noun(this_object()) +
                    " bite.");
            this_object()->eventPrint(target->GetName() + " avoids your bite.");
            env->eventPrint(target->GetName() + " avoids " +
                    possessive_noun(this_object()) +
                    " bite.",
                    ({ this_object(), target }));
            if(!estatep(target)) eventTrainSkill("melee attack", pro, con, 0,
                    GetCombatBonus(target->GetLevel()));
        }
    }
    else {
        this_object()->eventPrint("You flounder about like a buffoon.");
        env->eventPrint(GetName() + " flounders about like a "
                "buffoon.", this_object());
    }
    return 1;
}

int eventPreAttack(object agent){
    object env = room_environment();
    if( agent == this_object() ){
        return 0;
    }
    if( env->GetProperty("no attack") ){
        return 0;
    }
    if( GetDying() ){
        return 0;
    }
    if( playerp(this_object()) && playerp(agent) && !PLAYER_KILL){ 
        if( !env->CanAttack( agent, this_object() ) ){
            return 0;
        }
    }
    if( AddEnemy(agent) ){
        AddHostile(agent);
    }
    return 1;
}

varargs int eventReceiveAttack(int speed, string def, object agent){
    int fail, x, pro, level, bonus, ret;
    if(AttacksPerHB > MAX_ATTACKS_PER_HB) return 0;
    if(Dead) return 0;
    if(GetPenalty() > random(11)) fail = 1;
    if( !agent ){
        agent = previous_object();
    }
    if( !living(agent) ){
        level = 1;
        bonus = 1;
    }
    else {
        level = agent->GetLevel();
        bonus = GetCombatBonus(level);
    }
    if( AddEnemy(agent) ){
        AddHostile(agent);
    }
    if( def == "magic" ){
        pro = GetMagicResistance();
        if( (x = random(pro)) > speed ){
            if(!fail && !estatep(agent)) 
                eventTrainSkill("magic defense", pro, speed, 1, bonus);
            ret = 0;
        }
        else {
            if(!estatep(agent)) eventTrainSkill("magic defense", pro, speed, 0, bonus);
            ret = 1;
        }
    }
    else {
        pro = GetDefenseChance(GetSkillLevel(def + " defense"));
        x = random(pro = pro/2);
        if( !fail && x > speed ){
            if(!estatep(agent)) 
                eventTrainSkill(def + " defense", pro, speed, 1, bonus);
            ret = 0;
        }
        else {
            if(!fail && !estatep(agent)) 
                eventTrainSkill(def + " defense", pro, speed, 0, bonus);
            ret = 1;
        }
    }
    if(fail) ret = 1;
    return ret;
}

void eventKillEnemy(object ob){
    int level;
    int reward;

    if( !ob ) return;
    level = ob->GetLevel();
    if(ob->GetCustomXP()) reward = ob->GetCustomXP();
    else reward = (level * 99);

    if(this_object()->GetParty()){
        int spoils;
        object *loot_sharers = ({ this_object() });
        foreach(object member in PARTY_D->GetPartyMembers(this_object()->GetParty())){
            if(environment(member) == environment(this_object())){
                loot_sharers += ({ member });
            }
        }
        loot_sharers = distinct_array(loot_sharers);
        spoils = reward / sizeof(loot_sharers);
        foreach(object member in loot_sharers){
            member->AddExperiencePoints(spoils);
        }
    } 

    else {
        this_object()->AddExperiencePoints(reward);
    }

    if( member_array(ob, GetHostiles()) == -1 ){
        int x;

        if(!estatep(ob)) eventTrainSkill("murder", GetLevel(), level, 1,GetCombatBonus(level)); 
        x = ob->GetMorality();
        if( x > 0 ) x = -x;
        else if( GetMorality() > 200 ) x = 100;
        else x = 0;
        eventMoralAct(x);
    }
}

void eventDestroyEnemy(object ob){
    int level;

    if( !ob ) return;
    level = ob->GetLevel();
    if(ob->GetCustomXP()) this_object()->AddExperiencePoints(ob->GetCustomXP());
    else this_object()->AddExperiencePoints(level * 222);
    if(!estatep(ob)) eventTrainSkill("faith", GetLevel(), level, 1, GetCombatBonus(level));
}

void eventEnemyDied(object ob){
    if( !ob ) return;
    Enemies -= ({ ob });
    Hostiles -= ({ ob });
    if(!sizeof(SpecialTargets) || (!sizeof(Enemies) || !sizeof(Hostiles))) 
        NonTargets = ({});
}

varargs int eventReceiveDamage(mixed agent, int type, int x, int internal,
        mixed limbs){
    int hp,encumbrance;

    if(objectp(agent)){
        if(estatep(agent) && !estatep(this_object())) return 0;
        if(!estatep(agent) && estatep(this_object())) return 0;
    }

    encumbrance = this_object()->GetEncumbrance();
    //if(AttacksPerHB > MAX_ATTACKS_PER_HB) return 0;
    if(Dead) return 0;
    if(encumbrance > 200){
        if(GetInCombat()) tell_object(this_object(),"You try to dodge while weighed down.");
    }
    x = race::eventReceiveDamage(agent, type, x, internal, limbs);
    if( !Wimpy ) return x;
    if( (hp = GetHealthPoints()) < 1 ) return x;
    if( Wimpy < percent(hp, GetMaxHealthPoints()) )
        return x;
    call_out((: eventWimpy :), 0);
    return x;
}

mixed eventTurn(object who){
    int defense;
    object env = room_environment(who);

    if( !GetUndead() ){
        return 0;
    }
    if( GetProperty("no turn") ){
        if( !who ){
            return 0;
        }
        else {
            int x = GetProperty("no turn");

            env->eventPrint("The power of the undead "
                    "turns on " + who->GetName() +
                    ".", who);
            who->eventPrint("The power of the undead turns on you.");
            if( x > random(100) + 1 ){
                who->eventDie(this_object());
            }
            else {
                who->eventReceiveDamage(this_object(), MAGIC, random(50), 1);
            }
            return 0;
        }
    }
    if( !who ){
        race::eventTurn(who);
        return 1;
    }
    defense = GetMagicResistance();
    if( who->GetSkillLevel("faith") < defense ){
        who->eventPrint("You writhe in pain.");
        env->eventPrint(who->GetName() + " writhes in pain.",
                who);
        who->eventReceiveDamage(this_object(), MAGIC, random(defense), 1);
        if(!estatep(who)) eventTrainSkill("magic defense", defense, who->GetSkillLevel("faith"),
                1, GetCombatBonus(who->GetLevel()));
        return 0;
    }
    race::eventTurn(who);
    return 1;
}

int eventWimpy(int i){
    string dir, cmd;
    genv = room_environment();

    if( !genv || !GetInCombat() ){
        if(!i) return 0;
    }
    cmd = WimpyCommand || "go out";
    if( (sscanf(cmd, "go %s", dir) && !(genv->GetExit(dir))) ||
            (sscanf(cmd, "enter %s", dir) && !(genv->GetEnter(dir))) ){
        string *tmp;

        tmp = filter((genv->GetExits() || ({})),
                (: !(genv->GetDoor($1)) :));
        if( !sizeof(tmp) ){
            tmp = filter(genv->GetEnters(),
                    (: !(genv->GetDoor($1)) :));
            if( !sizeof(tmp) ){
                this_object()->eventPrint("You need to escape, but you have nowhere to go!");
                return 0;
            }
            cmd = "enter " + tmp[random(sizeof(tmp))];
        }
        else cmd = "go " + tmp[random(sizeof(tmp))];
    }
    return eventForce(cmd);
}

static void heart_beat(){
    race::heart_beat();
    AttacksPerHB = 0;
    if( GetSleeping() || GetDying() ){
        return;
    }
    if( cParalyzed > 0 ){
        cParalyzed--;
        if( cParalyzed < 1 ){
            function f;

            f = fParalyzed;
            fParalyzed = 0;
            if( functionp(f) && !(functionp(f) & FP_OWNER_DESTED) ){
                evaluate(f);
            }
            else {
                this_object()->eventPrint("You can move again.");
            }
        }
        return;
    }
    if( sizeof(Enemies) ){
        SortEnemies();
        foreach(object dude in Enemies){
            if(member_array(dude,NonTargets) != -1){
                RemoveEnemy(dude);
                RemoveHostile(dude);
            }
        }
    }
    if( sizeof(Enemies) ){
        object ob;

        SortEnemies();

        if( SpecialTargets ){
            foreach(object target in SpecialTargets){

                if( objectp(SetCurrentEnemy(target)) ){
                    break;
                }
            }
            eventExecuteAttack(SpecialTargets);
            SpecialTargets = 0;
        }
        else if( ob = ResetCurrentEnemy() ){
            eventExecuteAttack(ob);
        }
    }
    else if( tNextRound != ROUND_UNDEFINED && functionp(fNextRound) ){
        function f;

        f = fNextRound;
        tNextRound = ROUND_UNDEFINED;
        evaluate(f);
    }
}
