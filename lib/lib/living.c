/*    /lib/living.c
 *    from the Dead Souls Object Library
 *    handles common living code
 *    created by Descartes of Borg 951218
 *    Version: @(#) living.c 1.29@(#)
 *    Last Modified: 96/12/15
 */

#include <lib.h>
#include <daemons.h>
#include <position.h>
#include <message_class.h>
#include "include/living.h"

inherit LIB_CARRY;
inherit LIB_DROP;
inherit LIB_GET;
inherit LIB_COMBAT;
inherit LIB_CURRENCY;
inherit LIB_FOLLOW;
inherit LIB_MAGIC;
inherit LIB_LEAD;
inherit LIB_TEACH;
inherit LIB_LEARN;
inherit LIB_QUEST;
inherit LIB_STOP;

private int isPK;
private mixed Attackable = 1;
private int NoCondition = 0;

varargs mixed CanReceiveHealing(object who, string limb);

static void create(){
    combat::create();
    currency::create();
    follow::create();
    lead::create();
    isPK = 0;
}

int is_living(){ return 1; }

int inventory_accessible(){ return 1; }

mixed direct_verb_rule(string verb){
    return SOUL_D->CanTarget(this_player(), verb, this_object());
}

mixed direct_ride_str(){
    return this_object()->GetMount();
}

mixed direct_ride_word_str(){
    return this_object()->GetMount();
}

mixed direct_mount_liv(){
    return this_object()->GetMount();
}

mixed direct_dismount_liv(){
    return this_object()->GetMount();
}

mixed direct_dismount_from_liv(){
    return this_object()->GetMount();
}

mixed direct_attack_liv(){
    if(this_player() == this_object()){
        return "#You can't attack yourself.";
    }
    if(intp(Attackable) && !Attackable){
        return "You are unable to attack "+this_object()->GetShort()+".";
    }
    if(stringp(Attackable)) return Attackable;
    return 1;
}

mixed CanAttack(){
    if(this_player() == this_object()){
        return "You can't attack yourself.";
    }
    if( userp(this_player()) && userp(this_object()) ){
        if(!(environment()->CanAttack(this_player(), this_object()))){
            return "Player killing is not permitted in this area!";
        }
        if(intp(Attackable) && !Attackable){
            return "You are unable to attack "+this_object()->GetShort()+".";
        }
        if(this_player()->GetPK() && this_object()->GetPK()){
            if(!PLAYER_KILL) return "This is not a PK mud.";
        }
        else return "One of you is not a player killer. You cannot fight them.";
    }
    if(functionp(Attackable)) return evaluate(Attackable, this_player());
    else {
        return (Attackable || 0);
    }
}

mixed direct_attack_only_liv(){
    return direct_attack_liv();
}

mixed direct_attack_liv_only(){
    return direct_attack_liv();
}

mixed direct_target_liv(){
    return direct_attack_liv();
}

mixed direct_target_only_liv(){
    return direct_attack_liv();
}

mixed direct_target_liv_only(){
    return direct_attack_liv();
}

mixed direct_bite_liv(){
    return 1;
}

mixed direct_capture_liv_word_obj(){
    return 1;
}

mixed direct_pray_for_str_against_str_for_liv(){
    return 1;
}

mixed direct_cast_str_on_obj(){
    return 1;
}

mixed direct_cast_str_against_str(){
    return 1;
}


mixed direct_cast_str_on_str_of_obj(){
    return 1;
}

mixed direct_free_liv_from_obj(){
    return 1;
}

mixed direct_resurrect_obj(){ return 1; }
mixed indirect_resurrect_obj(){ return 1; }

mixed direct_get_obj(mixed args...){
    int mysize = this_object()->GetSize(1);
    int theirsize = this_player()->GetSize(1);
    if(archp(this_player())) return 1;
    if(creatorp(this_player()) && !creatorp(this_object())){
        return get::direct_get_obj(args...);
    }
    if(interactive(this_player()) && creatorp(this_object())){
        return "NO.";
    }
    if(this_object()->GetBefriended(this_player())) return 1;
    if((theirsize - mysize) > 1) return get::direct_get_obj(args...);
    return "It's too big!";
}

mixed direct_get_obj_from_obj(mixed args...){
    return direct_get_obj(args...);
}

mixed direct_show_liv_obj(){
    if( this_player() == this_object() ) return "Are you confused?";
    return 1;
}

mixed indirect_show_obj_to_liv(object item){
    if( !item ) return 0;
    if( this_player() == this_object() ) return "Are you confused?";
    if( environment(item) != this_player() ) return "You don't have that!";
    else return 1;
}

mixed indirect_show_obj_liv(object item){
    return indirect_show_obj_to_liv(item);
}

mixed direct_give_liv_obs(){
    return direct_give_liv_obj();
}

mixed direct_give_liv_obj(){
    if( this_player() == this_object() ) return "Are you confused?";
    return 1;
}

mixed indirect_give_obj_to_liv(object item){
    if( !item ) return 0;
    if( this_player() == this_object() ) return "Are you confused?";
    if( environment(item) != this_player() ) return "You don't have that!";
    if(!CanCarry(item->GetMass())){
        return this_object()->GetName()+" is carrying too much.";
    }
    else return CanCarry(item->GetMass());
}

mixed indirect_give_obj_liv(object item){
    return indirect_give_obj_to_liv(item);
}

mixed indirect_give_obs_to_liv(object *items){
    return 1;
}

mixed indirect_give_obs_liv(object *items){
    return indirect_give_obs_to_liv(items);
}

mixed direct_give_liv_wrd_wrd(object targ, string num, string curr){
    return direct_give_wrd_wrd_to_liv(num, curr);
}

mixed direct_give_wrd_wrd_to_liv(string num, string curr){
    mixed tmp;
    int amt;

    if( this_object() == this_player() )
        return "Are you feeling a bit confused?";
    if( (amt = to_int(num)) < 1 ) return "What sort of amount is that?";
    tmp = CanCarry(currency_mass(amt, curr));
    if( tmp != 1 ) return GetName() + " cannot carry that much "+ curr+ ".";
    return 1;
}

mixed direct_steal_wrd_from_liv(string wrd){
    if( wrd != "money" ) return 0;
    if( this_player() == this_object() ) return "Are you a fool?";
    if( this_player()->GetInCombat() )
        return "You are too busy fighting at the moment.";
    return 1;
}

mixed indirect_steal_obj_from_liv(object item, mixed args...){
    mixed tmp;

    if( environment()->GetProperty("no attack") )
        return "Mystical forces prevent your malice.";
    if( !item ) return 1;
    if( environment(item) != this_object() ) return 0;
    if( this_player() == this_object() ) return "Are you a fool?";
    if( this_player()->GetInCombat() )
        return "You are too busy fighting at the moment.";
    tmp = item->CanDrop(this_object());
    if( tmp != 1 )
        return GetName() + " will not let go of " + item->GetShort()+".";
    return 1;
}

    mixed direct_backstab_liv(){
        if( this_object() == this_player() )
            return "That would be messy.";
        if( member_array(this_object(), this_player()->GetEnemies()) != -1 )
            return "%^RED%^You have lost the element of surprise.";
        if( environment()->GetProperty("no attack") ||
                GetProperty("no backstab") )
            return "A mysterious forces stays your hand.";
        return 1;
    }

mixed direct_heal_str_of_liv(string limb){
    string array limbs = GetLimbs();
    mixed tmp;

    limb = lower_case(remove_article(limb));
    if( !limbs ){
        if( this_object() == this_player() ){
            return "You have no limbs!";
        }
        else {
            return GetName() + " has no limbs!";
        }
    }
    else if( member_array(limb, limbs) == -1 ){
        if( this_object() == this_player() ){
            return "You have no " + limb + ".";
        }
        else {
            return GetName() + " has no " + limb + ".";
        }
    }
    tmp = CanReceiveHealing(this_player(), limb);
    if( tmp != 1 ){
        return tmp;
    }
    return CanReceiveMagic(0, "heal");
}

mixed direct_remedy_str_of_liv(string limb){
    string *limbs;
    limbs = GetLimbs();
    if( !limbs ){
        if( this_object() == this_player() ) return "You have no limbs!";
        else return GetName() + " has no limbs!";
    }
    else if( member_array(limb, limbs) == -1 ){
        if( this_object() == this_player() ) return "You have no " + limb + ".";
        else return GetName() + " has no " + limb + ".";
    }
    return CanReceiveMagic(0, "remedy");
}

mixed direct_regen_str_on_liv(string limb){
    if( !limb ) return 0;
    if( member_array(limb, GetMissingLimbs()) == -1 ){
        return "That is not a missing limb!";
    }
    return CanReceiveMagic(0, "regen");
}

mixed direct_teleport_to_liv(){
    if( environment()->GetProperty("no teleport") ||
            environment()->GetProperty("no magic") ){
        return "Mystical forces prevent your magic.";
    }
    else return CanReceiveMagic(0, "teleport");
}

mixed direct_portal_to_liv(){
    return direct_teleport_to_liv();
}

    mixed direct_resurrect_liv(){
        if( this_player() == this_object() )
            return "You cannot resurrect yourself.";
        if( !GetUndead() ) 
            return GetName() + " is not dead!";
        return CanReceiveMagic(0, "resurrect");
    }

mixed direct_scry_liv(){
    object env = environment();

    if( this_player() == this_object() )
        return "Scry yourself??";
    if( !env ) return GetName() + " is nowhere.";
    if( env->GetProperty("no magic") || env->GetProperty("no scry") )
        return GetName() + " is beyond your reach.";
    return CanReceiveMagic(0, "scry");
}

mixed indirect_zap_liv(){ return 1; }
mixed direct_zap_liv(){ return 1; }
mixed indirect_pulsecheck_liv(){ return 1; }
mixed direct_pulsecheck_liv(){ return 1; }

/* hostile spells */

int direct_rockwhip_liv(){ return CanReceiveMagic(1, "rockwhip"); }
int direct_acidspray_liv(){ return CanReceiveMagic(1, "acidspray"); }
int direct_annihilate_at_liv(){ return CanReceiveMagic(1, "annihilate"); }
int direct_annihilate_liv(){ return CanReceiveMagic(1, "annihilate"); }
int direct_arrow_liv(){ return CanReceiveMagic(1, "arrow"); }
int direct_arrow_at_liv(){ return CanReceiveMagic(1, "arrow"); }
int direct_blades_at_liv(){ return CanReceiveMagic(1, "blades"); }
int direct_blades_liv(){ return CanReceiveMagic(1, "blades"); }
int direct_corrupt_liv(){ return CanReceiveMagic(1, "currupt"); }
int direct_demonclaw_liv(){ return CanReceiveMagic(1, "demonclaw"); }
int direct_dispel_liv(){ return CanReceiveMagic(1, "dispel"); }
int direct_drain_at_liv(){ return CanReceiveMagic(1, "drain"); }
int direct_drain_liv(){ return CanReceiveMagic(1, "drain"); }
int direct_fireball_at_liv(){ return CanReceiveMagic(1, "fireball"); }
int direct_fireball_liv(){ return CanReceiveMagic(1, "fireball"); }
int direct_frigidus_at_liv(){ return CanReceiveMagic(1, "frigidus"); }
int direct_frigidus_liv(){ return CanReceiveMagic(1, "frigidus"); }
int direct_holylight_liv(){ return CanReceiveMagic(1, "holylight"); }
int direct_missile_liv(){ return CanReceiveMagic(1, "missile"); }
int direct_missile_at_liv(){ return CanReceiveMagic(1, "missile"); }
int direct_shock_liv(){ return CanReceiveMagic(1, "shock"); }
int direct_palm_liv(){ return CanReceiveMagic(1, "palm"); }
int direct_immolate_liv(){ return CanReceiveMagic(1, "immolate"); }
int direct_gale_liv(){ return CanReceiveMagic(1, "gale"); }

/* other spells */

int direct_aura_liv(){ return CanReceiveMagic(0, "aura"); }
int direct_soulseek_liv(){ return CanReceiveMagic(0, "soulseek"); } 
int direct_cloak_wrd(){ return CanReceiveMagic(0, "cloak"); }
int direct_stealth_wrd(){ return CanReceiveMagic(0, "stealth"); }
int direct_backlash_for_liv(){ return CanReceiveMagic(0, "backlash"); }
int direct_backlash_for_liv_against_wrd(){ return CanReceiveMagic(0, "backlash"); }
int direct_balance_obj_to_obj(){ return CanReceiveMagic(0, "balance"); }
int direct_buffer_liv(){ return CanReceiveMagic(0, "buffer"); }
int direct_calm_liv(){ return CanReceiveMagic(0, "calm"); }
int direct_cleanse_liv(){ return CanReceiveMagic(0, "cleanse"); }
int direct_convert_liv(){ return CanReceiveMagic(0, "convert"); }
int direct_shield_liv(){ return CanReceiveMagic(0, "shield"); }
int direct_veil_liv_against_wrd_wrd(){ return CanReceiveMagic(0, "veil"); }
int direct_ward_liv_against_wrd(){ return CanReceiveMagic(0, "ward"); }
int direct_remedy_liv(){ return CanReceiveMagic(0, "remedy"); }
int direct_command_str_to_str(){ return CanReceiveMagic(0, "command"); }
int direct_gaze(){ return CanReceiveMagic(0, "gaze"); }
int direct_send_str_to_str(){ return CanReceiveMagic(0, "send"); }
int direct_connect_str(){ return CanReceiveMagic(0, "connect"); }
int direct_heal_liv(){ return CanReceiveMagic(0, "heal"); }
int direct_mend_liv(){ return CanReceiveMagic(0, "mend"); }
int direct_refresh_liv(){ return CanReceiveMagic(0, "refresh"); }
int direct_rejuvinate_liv(){ return CanReceiveMagic(0, "rejuvinate"); }
int direct_farsight_liv(){ return 1; }
int direct_bump_liv(){ return 1; }
int direct_evade_liv(){ return 1; }
int direct_follow_liv(){ return 1; }
int direct_lead_liv(){ return 1; }
int direct_marry_liv_to_liv(){ return 1; }
int direct_party_wrd_liv(){ return 1; }
int direct_challenge_liv(){ return 1; }
int direct_ignore_liv(){ return 1; }

int indirect_throw_obj_at_obj(){ return 1; }
int indirect_toss_obj_at_obj(){ return 1; }
int indirect_buy_str_from_liv(){ return 1; }
int indirect_sell_obj_to_liv(){ return 1; }
int indirect_marry_liv_to_liv(){ return 1; }


/*     **********     /lib/living.c modal methods     **********     */
int CanCarry(int amount){ return carry::CanCarry(amount); }

varargs mixed CanReceiveHealing(object who, string limb){
    int max, hp;

    max = GetMaxHealthPoints(limb);
    hp = GetHealthPoints(limb);
    if( (max-hp) < max/20 ){
        if( limb ){
            return possessive_noun(GetName()) + " " + limb + " needs no help.";
        }
        else {
            return GetName() + " needs no help.";
        }
    }
    return 1;
}

mixed CanReceiveMagic(int hostile, string spell){
    if( GetProperty(spell) == "immune" ){
        this_player()->eventPrint(GetName() + " is immune to such magic.");
        return 0;
    }
    if( !hostile ) return 1;
    if( this_player() == this_object() ){
        eventPrint("That would be construed as quite foolish.");
        return 0;
    }
    return 1;
}

varargs mixed CanCastMagic(int hostile, string spell){
    object env = environment();

    if( !env ) eventPrint("You are nowhere!");
    if( spell && GetProperty("no " + spell) ){
        eventPrint("A mysterious forces prevents you from doing that.");
        return 0;
    }
    if( env->GetProperty("no magic") ){
        eventPrint("Mystical forces prevent your magic.");
        return 0;
    }
    if( !hostile ) return 1;
    if( env->GetProperty("no attack" ) ){
        eventPrint("Mystical forces prevent your hostile intentions.");
        return 0;
    }
    return 1;
}

/*     **********     /lib/living.c event methods     **********     */

mixed eventCure(object who, int amount, string type){
    object array germs = filter(all_inventory(),
            (: $1->IsGerm() && $1->GetType()== $(type) :));

    if( !sizeof(germs) ){
        return GetName() + " suffers from no such affliction.";
    }
    return germs[0]->eventCure(who, amount, type);
}

int eventFollow(object dest, int followChance){
    string dir;
    int ret;

    if( objectp(dest) ){
        if( !environment() ){
            Destruct();
            return 0;
        }
        dir = environment()->GetDirection(base_name(dest));
    }
    if( !stringp(dir) ) dir = "";
    if( dir != "" && followChance > random(100) ){
        eventForce(dir);
    }
    if( environment() == dest ) return 1;
    else {
        string newdir = replace_string(dir,"go ","");
        if(newdir) ret = environment(this_object())->eventGo(this_object(), newdir);
        if(!ret){
            newdir = replace_string(dir,"enter ","");
            ret = environment(this_object())->eventEnter(this_object(), newdir);
        }
    }
    return ret;
}

mixed eventInfect(object germ){
    return germ->eventInfect(this_object());
}

varargs mixed eventShow(object who, string str){
    string desc = this_object()->GetItem(str);
    if(!desc){
        who->eventPrint(this_object()->GetLong(str));
    }
    else {
        who->eventPrint(desc);
    }
    environment(who)->eventPrint(this_player()->GetName() +
            " looks at " + this_object()->GetShort() + ".",
            ({ who, this_object() }));
    return 1;
}

/* when who == this_object(), I am doing the stealing
 * otherwise I am being stolen from
 */
varargs mixed eventSteal(object who, mixed what, object target, int skill){

    int i, sr;

    if( who == this_object() ){
        mixed tmp;
        int amt, skill2;

        skill2 = to_int(to_float(GetSkillLevel("stealing"))*2.5);
        skill2 += GetMobility();
        skill2 += GetStatLevel("coordination");
        skill2 += GetStatLevel("charisma");
        skill2 += to_int(GetStatLevel("luck")/2);
        if( ClassMember("rogue") ) skill2 += GetLevel();
        if( ClassMember("thief") ) skill2 += GetLevel();

        if( !stringp(what) ){
            int x;

            x = sizeof(what);
            if( GetStaminaPoints() < 20.0*x ){
                eventPrint("You are clumsy in your fatigue.");
                if(target->GetRace() != "kender"){
                    target->SetAttack(this_object());
                    target->eventExecuteAttack(this_object());
                }
                return 1;
            }
            AddStaminaPoints(-20);

            tmp = target->eventSteal(who, what, target,skill2);

            /* You can't steal from this target */
            if( !tmp )
                return "You cannot steal from " + target->GetName() +".";

            /* Steal from target was succesful */
            else if( tmp == 1 ){

                what = filter(what, (: $1 :));
                AddSkillPoints("stealing", implode(map(what,
                                (: $1->GetMass() :)),
                            (: $1 + $2 :)) +
                        GetSkillLevel("stealing") * GetSkillLevel("stealing")*3);
                AddSkillPoints("stealth", random(sizeof(what)) * 20);
                AddStaminaPoints(-2);
                this_player()->eventPrint(sprintf("You steal %s from %s.",
                            "something", target->GetName()) );
                what->eventMove(this_object());
                return 1;
            }

            /* Steal was unsuccesful */
            else if( tmp == 2 ){
                AddSkillPoints("stealing", GetSkillLevel("stealing")*2);
                AddStaminaPoints(-5);
                return 1;
            }

            else return tmp;
        }
        /********************************************/
        /* This part deals with stealing money */

        amt = GetNetWorth();
        eventPrint("You reach for " + possessive_noun(target) + " money.");
        tmp = target->eventSteal(who, what, target, skill2);

        /* You can't steal from this target */
        if( !tmp )
            return "You cannot steal from " + target->GetName() + ".";

        /* Steal from target was succesful */
        else if( tmp == 1 ){
            amt = GetNetWorth() - amt;
            if( amt < 1 ) return tmp;
            AddSkillPoints("stealing", random(7*amt) +
                    GetSkillLevel("stealing") * GetSkillLevel("stealing") * 2);
            AddSkillPoints("stealth", random(amt));
            AddStatPoints("coordination", random(amt));
            AddStaminaPoints(-3);
            eventPrint("You come away with some money!");
            return tmp;
        }

        /* Steal was unsuccesful */
        else if( tmp == 2){
            AddSkillPoints("stealing", GetSkillLevel("stealing")*2);
            AddStaminaPoints(-5);
            return 1;
        }

        return tmp;
    }
    /*******************************************************************/
    if( GetInCombat() ){
        who->eventPrint(GetName() + " is busy in combat.");
        return 0;
    }
    skill -= to_int(GetMobility()/2);
    skill -= GetStatLevel("agility");
    skill -= GetStatLevel("wisdom");
    skill -= to_int(GetStatLevel("luck")/3);

    if( objectp(what) ) sr = 100 * sizeof(what);
    else sr = 100;
    if( random(sr) > skill ){
        target->eventPrint("You notice " + who->GetName() + " trying "
                "to steal from you!");
        if( !userp(this_object()) ){
            who->eventPrint("%^RED%^" + GetName() + "%^RED%^ "
                    "notices your attempt at treachery!",
                    environment(who) );
            eventForce("attack " + who->GetKeyName());
            this_object()->SetProperty("steal victim", 1);
        }
        return 2;
    }

    if( random(2*sr) > skill ){
        who->eventPrint("You are unsure if anyone noticed your foolish "
                "attempt at thievery.",environment(who) );
        return 2;
    }

    if( stringp(what) ){

        foreach(string curr in GetCurrencies()){
            int x;

            if( !(x = random(GetCurrency(curr)/5)) ) continue;
            if( x > 100 ) x = 100;
            x = (x * skill)/100;
            AddCurrency(curr, -x);
            who->AddCurrency(curr, x);
        }
        return 1;
    }
    for(i=0; i<sizeof(what); i++){
        if( what[i]->eventSteal(who) != 1 ) what[i] = 0;
    }
    return 1;
}

int AddCarriedMass(int x){ return carry::AddCarriedMass(x); }

int GetCarriedMass(){
    return (currency::GetCurrencyMass() + carry::GetCarriedMass());
}

int GetNonCurrencyMass(){
    return (carry::GetCarriedMass());
}

int GetMaxCarry(){ return combat::GetMaxCarry(); }

int SetPK(int x){ return (isPK = x); }

int GetPK(){ return isPK; }

int SetDead(int i){
    return combat::SetDead(i);
}

mixed SetAttackable(mixed foo){
    Attackable = foo;
    return Attackable;
}

mixed GetAttackable(){
    return Attackable;
}

int SetNoCondition(int foo){
    NoCondition = foo;
    return NoCondition;
}

int GetNoCondition(){
    return NoCondition;
}

varargs int eventMoveLiving(mixed dest, string omsg, string imsg, mixed dir){
    object *inv;
    object prev;
    string prevclim, newclim;
    int check = GUARD_D->CheckMove(this_object(), dest, dir);

    if(!check){
        eventPrint("You remain where you are.", MSG_SYSTEM);
        return 0;
    }
    if(omsg && stringp(omsg)){
        omsg = replace_string(omsg, "$N", this_object()->GetName());
    }
    if(imsg && stringp(imsg)){
        imsg = replace_string(imsg, "$N", this_object()->GetName());
    }

    if( prev = environment() ){
        prevclim = prev->GetClimate();
        if( stringp(dest) ){
            if(dest[0] != '/'){
                string *arr;

                arr = explode(file_name(prev), "/");
                dest = "/"+implode(arr[0..sizeof(arr)-2], "/")+"/"+dest;
            }
        }
        if( !eventMove(dest) ){
            eventPrint("You remain where you are.", MSG_SYSTEM);
            return 0;
        }
        if(prev){
            inv = filter(all_inventory(prev), (: (!this_object()->GetInvis($1) 
                            && living($1) && !GetProperty("stealthy") && ($1 != this_object())) :));
        }
        if(!dir) dir = "away";
        if(query_verb() == "home" ){
            if(!omsg || omsg == "") omsg = GetMessage("telout");
            if(!imsg || imsg == "") imsg = GetMessage("telin");
        }
        else if(GetPosition() == POSITION_SITTING ||
                GetPosition() == POSITION_LYING ){
            if(!omsg || omsg == "") omsg = GetName()+" crawls "+dir+".";
            if(!imsg || imsg == "") imsg = GetName()+" crawls in.";
        }
        else if(GetPosition() == POSITION_FLYING ){
            if(!omsg || omsg == "") omsg = GetName()+" flies "+dir+".";
            if(!imsg || imsg == "") imsg = GetName()+" flies in.";
        }
        else {
            if(!omsg || omsg == "") omsg = GetMessage("leave",dir);
            if(!imsg || imsg == "") imsg = GetMessage("come");
        }
        if(sizeof(inv)) inv->eventPrint(omsg, MSG_ENV);
    }
    else if( !eventMove(dest) ){
        eventPrint("You remain where you are.", MSG_SYSTEM);
        return 0;
    }
    inv = filter(all_inventory(environment()),
            (: (!this_object()->GetInvis($1) && !GetProperty("stealthy") &&
                living($1) && ($1 != this_object())) :));

    inv->eventPrint(imsg, MSG_ENV);
    if(this_object()->GetInvis()){
        if(!creatorp(this_object())) AddStaminaPoints(-(15-(GetSkillLevel("stealth")/10)));
        AddSkillPoints("stealth", 30 + GetSkillLevel("stealth")*2);
        eventPrint("%^RED%^You move along quietly....%^RESET%^\n");
    }
    if(GetProperty("stealthy") && interactive(this_object())){
        if(!creatorp(this_object())) AddStaminaPoints(-3 - random(3));
        AddSkillPoints("stealth", 10 + GetSkillLevel("stealth")*2);
    }
    this_object()->eventDescribeEnvironment(this_object()->GetBriefMode());
    newclim = environment()->GetClimate();
    if( !GetUndead() ) switch( newclim ){
        case "arid":
            if(!creatorp(this_object())) AddStaminaPoints(-0.3);
        break;
        case "tropical":
            if(!creatorp(this_object())) AddStaminaPoints(-0.3);
        break;
        case "sub-tropical":
            if(!creatorp(this_object())) AddStaminaPoints(-0.2);
        break;
        case "sub-arctic":
            if(!creatorp(this_object())) AddStaminaPoints(-0.2);
        break;
        case "arctic":
            if(!creatorp(this_object())) AddStaminaPoints(-0.3);	  
        break;
        default:
        if(!creatorp(this_object())) AddStaminaPoints(-0.1);	  
        break;	    
    }
    if( prevclim != newclim && prevclim != "indoors" && newclim != "indoors" ){
        switch(prevclim){
            case "arid":
                if( newclim == "tropical" || newclim == "sub-tropical" )
                    message("environment", "The air is much more humid.",
                            this_object());
                else message("environment", "The air is getting a bit cooler.",
                        this_object());
            break;
            case "tropical":
                if( newclim != "arid" )
                    message("environment", "The air is not quite as humid.",
                            this_object());
                else message("environment", "The air has become suddenly dry.",
                        this_object());
            break;
            case "sub-tropical":
                if( newclim == "arid" )
                    message("environment", "The air has become suddenly dry.",
                            this_object());
                else if( newclim == "tropical" )
                    message("environment","The air has gotten a bit more humid.",
                            this_object());
                else message("environment", "The air is not quite as humid.",
                        this_object());
            break;
            case "temperate":
                if( newclim == "arid" )
                    message("environment", "The air is a bit drier and warmer.",
                            this_object());
                else if( newclim == "tropical" )
                    message("environment", "The air is much more humid.",
                            this_object());
                else if( newclim == "sub-tropical" )
                    message("environment", "The air is a bit more humid.",
                            this_object());
                else message("environment", "The air is a bit colder now.",
                        this_object());
            break;
            case "sub-arctic":
                if( newclim == "arid" || newclim == "tropical" ||
                        newclim == "sub-tropical" )
                    message("environment", "It has suddenly grown very hot.",
                            this_object());
                else if( newclim == "arctic" )
                    message("environment", "It is a bit cooler than before.",
                            this_object());
                else message("environment", "It is not quite as cold as "
                        "before.", this_object());
            break;
            case "arctic":
                if( newclim == "sub-arctic" )
                    message("environment", "It is not quite as cold now.",
                            this_object());
                else message("environment", "It is suddenly much warmer than "
                        "before.", this_object());
        }
    }
    eventMoveFollowers(environment(this_object()));
    TRACKER_D->TrackLiving(this_object());
    return 1;
}

string GetEquippedShort(){
    return this_object()->GetHealthShort();
}

int GeteventPrints(){
    return 1;
}
