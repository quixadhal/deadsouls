/*    /lib/living.c
 *    from the Dead Souls V Object Library
 *    handles common living code
 *    created by Descartes of Borg 951218
 *    Version: @(#) living.c 1.29@(#)
 *    Last Modified: 96/12/15
 */

#include <lib.h>
#include <daemons.h>
#include "include/living.h"

inherit LIB_CARRY;
inherit LIB_COMBAT;
inherit LIB_CURRENCY;
inherit LIB_FOLLOW;
inherit LIB_MAGIC;
inherit LIB_LEAD;
inherit LIB_SMELL;

private int isPK;

varargs mixed CanReceiveHealing(object who, string limb);

static void create() {
    combat::create();
    currency::create();
    follow::create();
    lead::create();
    isPK = 0;
}

int is_living() { return 1; }

int inventory_accessible() { return 1; }

int inventory_visible() { return 1; }

mixed direct_verb_rule(string verb) {
    return SOUL_D->CanTarget(this_player(), verb, this_object());
}

mixed direct_attack_liv() {
    if( userp(this_player()) && userp(this_object()) ) {
        if( !(int)environment(this_player())->CanAttack(this_object()) ) {
            return "No player killing!";
	}
    }
    else return 1;
}

mixed direct_bite_liv() {
    return 1;
}

mixed direct_capture_liv_word_obj() {
    return 1;
}

mixed direct_pray_for_str_against_str_for_liv() {
    return 1;
}

mixed direct_cast_str_on_obj() {
    return 1;
}

mixed direct_cast_str_against_str() {
    return 1;
}


mixed direct_cast_str_on_str_of_obj() {
    return 1;
}

mixed direct_free_liv_from_obj() {
    return 1;
}

mixed direct_get_obj() {
    return "You can't get a living thing!";
}

mixed direct_get_obj_from_obj() {
    return "You can't get a living thing!";
}

mixed direct_give_liv_obs() {
    return direct_give_liv_obj();
}

mixed direct_give_liv_obj() {
    if( this_player() == this_object() ) return "Are you confused?";
    return 1;
}

mixed indirect_give_obj_to_liv(object item) {
    if( !item ) return 0;
    if( this_player() == this_object() ) return "Are you confused?";
    if( environment(item) != this_player() ) return "You don't have that!";
    return CanCarry((int)item->GetMass());
}

mixed indirect_give_obs_to_liv(object *items) {
    return 1;
}

mixed direct_give_liv_wrd_wrd(object targ, string num, string curr) {
    return direct_give_wrd_wrd_to_liv(num, curr);
}

mixed direct_give_wrd_wrd_to_liv(string num, string curr) {
    mixed tmp;
    int amt;
    
    if( this_object() == this_player() )
      return "Are you feeling a bit confused?";
    if( (amt = to_int(num)) < 1 ) return "What sort of amount is that?";
    tmp = CanCarry(currency_mass(amt, curr));
    if( tmp != 1 ) return GetName() + " cannot carry that much "+ curr+ ".";
}

mixed direct_look_obj() { return 1; }

mixed direct_look_at_obj() { return 1; }

mixed direct_smell_obj() {
    return smell::direct_smell_obj();
}

mixed direct_steal_wrd_from_liv(string wrd) {
    if( wrd != "money" ) return 0;
    if( this_player() == this_object() ) return "Are you fool?";
    if( !GetPK() ) return "You cannot act like a jerk today.";
    if( userp(this_object()) && !((int)this_player()->GetPK()) )
      return "You do not have your player killer flag set.";
    if( this_player()->GetInCombat() )
      return "You are too busy fighting at the moment.";
    return 1;
}

mixed indirect_steal_obj_from_liv(object item, mixed args...) {
    mixed tmp;
    
    if( environment()->GetProperty("no attack") )
        return "Mystical forces prevent your malice.";
    if( !item ) return 1;
    if( environment(item) != this_object() ) return 0;
    if( this_player() == this_object() ) return "Are you a fool?";
    if( !GetPK() ) return "You cannot act like a jerk today.";
    if( userp(this_object()) && !((int)this_player()->GetPK()) )
      return "You do not have your player killer flag set.";    
    if( this_player()->GetInCombat() )
      return "You are too busy fighting at the moment.";
    tmp = (mixed)item->CanDrop(this_object());
    if( tmp != 1 )
      return GetName() + " will not let go of " + (string)item->GetShort()+".";
    return 1;
}

mixed direct_backstab_liv() {
    if( this_object() == this_player() )
      return "That would be messy.";
    if( member_array(this_object(), this_player()->GetEnemies()) != -1 )
      return "%^RED%^You have lost the element of surprise.";
    if( environment()->GetProperty("no attack") ||
        GetProperty("no backstab") )
      return "A mysterious forces stays your hand.";
    return 1;
}

mixed direct_heal_str_of_liv(string limb) {
    string array limbs = GetLimbs();
    mixed tmp;
    
    limb = lower_case(remove_article(limb));
    if( !limbs ) {
        if( this_object() == this_player() ) {
	    return "You have no limbs!";
	}
        else {
	    return GetName() + " has no limbs!";
	}
    }
    else if( member_array(limb, limbs) == -1 ) {
        if( this_object() == this_player() ) {
	    return "You have no " + limb + ".";
	}
        else {
	    return GetName() + " has no " + limb + ".";
	}
    }
    tmp = CanReceiveHealing(this_player(), limb);
    if( tmp != 1 ) {
	return tmp;
    }
    return CanReceiveMagic(0, "heal");
}

 
mixed direct_remedy_str_of_liv(string limb) {
    string *limbs;
    limbs = GetLimbs();
    if( !limbs ) {
        if( this_object() == this_player() ) return "You have no limbs!";
        else return GetName() + " has no limbs!";
    }
    else if( member_array(limb, limbs) == -1 ) {
        if( this_object() == this_player() ) return "You have no " + limb + ".";
        else return GetName() + " has no " + limb + ".";
    }
    return CanReceiveMagic(0, "remedy");
}
 
mixed direct_regen_str_on_liv(string limb) {
    if( !limb ) return 0;
    if( member_array(limb, GetMissingLimbs()) == -1 ) {
         return "That is not a missing limb!";
    }
    return CanReceiveMagic(0, "regen");
}


mixed direct_teleport_to_liv() {
    if( environment()->GetProperty("no teleport") ||
        environment()->GetProperty("no magic") ) {
        return "Mystical forces prevent your magic.";
    }
    else return CanReceiveMagic(0, "teleport");
}

mixed direct_portal_to_liv() {
    return direct_teleport_to_liv();
}

mixed direct_resurrect_liv() {
    if( this_player() == this_object() )
      return "You cannot resurrect yourself.";
    if( !GetUndead() ) 
        return GetName() + " is not dead!";
    return CanReceiveMagic(0, "resurrect");
}

mixed direct_scry_liv() {
    object env = environment();

    if( this_player() == this_object() )
      return "Scry yourself??";
    if( !env ) return GetName() + " is nowhere.";
    if( env->GetProperty("no magic") || env->GetProperty("no scry") )
      return GetName() + " is beyond your reach.";
    return CanReceiveMagic(0, "scry");
}

/* hostile spells */
 
int direct_rockwhip_liv() { return CanReceiveMagic(1, "rockwhip"); }
int direct_acidspray_liv() { return CanReceiveMagic(1, "acidspray"); }
int direct_annihilate_at_liv() { return CanReceiveMagic(1, "annihilate"); }
int direct_annihilate_liv() { return CanReceiveMagic(1, "annihilate"); }
int direct_arrow_liv() { return CanReceiveMagic(1, "arrow"); }
int direct_arrow_at_liv() { return CanReceiveMagic(1, "arrow"); }
int direct_blades_at_liv() { return CanReceiveMagic(1, "blades"); }
int direct_blades_liv() { return CanReceiveMagic(1, "blades"); }
int direct_corrupt_liv() { return CanReceiveMagic(1, "currupt"); }
int direct_demonclaw_liv() { return CanReceiveMagic(1, "demonclaw"); }
int direct_dispel_liv() { return CanReceiveMagic(1, "dispel"); }
int direct_drain_at_liv() { return CanReceiveMagic(1, "drain"); }
int direct_drain_liv() { return CanReceiveMagic(1, "drain"); }
int direct_fireball_at_liv() { return CanReceiveMagic(1, "fireball"); }
int direct_fireball_liv() { return CanReceiveMagic(1, "fireball"); }
int direct_frigidus_at_liv() { return CanReceiveMagic(1, "frigidus"); }
int direct_frigidus_liv() { return CanReceiveMagic(1, "frigidus"); }
int direct_holylight_liv() { return CanReceiveMagic(1, "holylight"); }
int direct_missile_liv() { return CanReceiveMagic(1, "missile"); }
int direct_missile_at_liv() { return CanReceiveMagic(1, "missile"); }
int direct_shock_liv() { return CanReceiveMagic(1, "shock"); }
int direct_palm_liv() { return CanReceiveMagic(1, "palm"); }
int direct_immolate_liv() { return CanReceiveMagic(1, "immolate"); }
int direct_gale_liv() { return CanReceiveMagic(1, "gale"); }
 
/* other spells */
 
int direct_aura_liv() { return CanReceiveMagic(0, "aura"); }
int direct_soulseek_liv() { return CanReceiveMagic(0, "soulseek"); } 
int direct_cloak_wrd() { return CanReceiveMagic(0, "cloak"); }
int direct_stealth_wrd() { return CanReceiveMagic(0, "stealth"); }
int direct_backlash_for_liv() { return CanReceiveMagic(0, "backlash"); }
int direct_backlash_for_liv_against_wrd() { return CanReceiveMagic(0, "backlash"); }
int direct_balance_obj_to_obj() { return CanReceiveMagic(0, "balance"); }
int direct_buffer_liv() { return CanReceiveMagic(0, "buffer"); }
int direct_calm_liv() { return CanReceiveMagic(0, "calm"); }
int direct_cleanse_liv() { return CanReceiveMagic(0, "cleanse"); }
int direct_convert_liv() { return CanReceiveMagic(0, "convert"); }
int direct_shield_liv() { return CanReceiveMagic(0, "shield"); }
int direct_veil_liv_against_wrd_wrd() { return CanReceiveMagic(0, "veil"); }
int direct_ward_liv_against_wrd() { return CanReceiveMagic(0, "ward"); }
int direct_remedy_liv() { return CanReceiveMagic(0, "remedy"); }
int direct_command_str_to_str() { return CanReceiveMagic(0, "command"); }
int direct_gaze() { return CanReceiveMagic(0, "gaze"); }
int direct_send_str_to_str() { return CanReceiveMagic(0, "send"); }
int direct_connect_str() { return CanReceiveMagic(0, "connect"); }
int direct_heal_liv() { return CanReceiveMagic(0, "heal"); }
int direct_mend_liv() { return CanReceiveMagic(0, "mend"); }
int direct_refresh_liv() { return CanReceiveMagic(0, "refresh"); }
int direct_rejuvinate_liv() { return CanReceiveMagic(0, "rejuvinate"); }
int direct_farsight_liv() { return 1; }
int direct_bump_liv() { return 1; }
int direct_evade_liv() { return 1; }
int direct_follow_liv() { return 1; }
int direct_lead_liv() { return 1; }
int direct_marry_liv_to_liv() { return 1; }
int direct_party_wrd_liv() { return 1; }
int direct_challenge_liv() { return 1; }

int indirect_throw_obj_at_obj() { return 1; }
int indirect_toss_obj_at_obj() { return 1; }
int indirect_buy_str_from_liv() { return 1; }
int indirect_sell_obj_to_liv() { return 1; }
int indirect_marry_liv_to_liv() { return 1; }

/*     **********     /lib/living.c modal methods     **********     */
int CanCarry(int amount) { return carry::CanCarry(amount); }

varargs mixed CanReceiveHealing(object who, string limb) {
    int max, hp;

    max = GetMaxHealthPoints(limb);
    hp = GetHealthPoints(limb);
    if( (max-hp) < max/20 ) {
	if( limb ) {
	    return possessive_noun(GetName()) + " " + limb + " needs no help.";
	}
	else {
	    return GetName() + " needs no help.";
	}
    }
    return 1;
}

mixed CanReceiveMagic(int hostile, string spell) {
    if( GetProperty(spell) == "immune" ) {
        this_player()->eventPrint(GetName() + " is immune to such magic.");
        return 0;
    }
    if( !hostile ) return 1;
    if( this_player() == this_object() ) {
        eventPrint("That would be construed as quite foolish.");
        return 0;
    }
    return 1;
}

varargs mixed CanCastMagic(int hostile, string spell) {
    object env = environment();

    if( !env ) "You are nowhere!";
    if( spell && GetProperty("no " + spell) ) {
        eventPrint("A mysterious forces prevents you from doing that.");
        return 0;
    }
    if( env->GetProperty("no magic") ) {
        eventPrint("Mystical forces prevent your magic.");
        return 0;
    }
    if( !hostile ) return 1;
    if( env->GetProperty("no attack" ) ) {
        eventPrint("Mystical forces prevent your hostile intentions.");
        return 0;
    }
    return 1;
}

/*     **********     /lib/living.c event methods     **********     */
mixed eventCure(object who, int amount, string type) {
    object array germs = filter(all_inventory(),
				(: $1->IsGerm() && $1->GetType()== $(type) :));

    if( !sizeof(germs) ) {
	return GetName() + " suffers from no such affliction.";
    }
    return germs[0]->eventCure(who, amount, type);
}

int eventFollow(object dest, int followChance) {
    string dir;

    if( objectp(dest) ) {
	if( !environment() ) {
	    Destruct();
	    return 0;
	}
	dir = environment()->GetDirection(base_name(dest));
    }
    if( !stringp(dir) ) dir = "";
    if( dir != "" && followChance > random(100) ) eventForce(dir);
    if( environment() == dest ) return 1;
    return 0;
}

mixed eventInfect(object germ) {
    return germ->eventInfect(this_object());
}

varargs mixed eventShow(object who, string str, string on_id) {
    who->eventPrint(GetLong(str));
    environment(who)->eventPrint((string)this_player()->GetName() +
				 " looks at " + GetShort() + ".",
				 ({ who, this_object() }));
    return 1;
}

/* when who == this_object(), I am doing the stealing
 * otherwise I am being stolen from
 */
varargs mixed eventSteal(object who, mixed what, object target, int skill) {

 int i, sr;

    if( who == this_object() ) {
        mixed tmp;
        int amt, skill2;

        skill2 = GetSkillLevel("stealing")*2.5;
            skill2 += GetMobility();
            skill2 += GetStatLevel("coordination");
            skill2 += GetStatLevel("charisma");
            skill2 += GetStatLevel("luck")/2;
        if( ClassMember("rogue") ) skill2 += (int)GetLevel();

        if( !stringp(what) ) {
            int x;

            x = sizeof(what);
            if( GetStaminaPoints() < 20.0*x ) {
                eventPrint("You are clumsy in your fatigue.");
                target->SetAttack(this_object());
                 target->eventExecuteAttack(this_object());
                return 1;
            }
            AddStaminaPoints(-20);

           tmp = (mixed)target->eventSteal(who, what, target,skill2);

           /* You can't steal from this target */
           if( !tmp )
              return "You cannot steal from " + (string)target->GetName() +".";

           /* Steal from target was succesful */
           else if( tmp == 1 ) {
                string itstr = "";

                what = filter(what, (: $1 :));
                AddSkillPoints("stealing", implode(map(what,
                               (: $1->GetMass() :)),
                                 (: $1 + $2 :)) +
        GetSkillLevel("stealing") * GetSkillLevel("stealing")*3);
                AddSkillPoints("stealth", random(sizeof(what)) * 20);
                AddStaminaPoints(-2);
		    this_player()->eventPrint(sprintf("You steal %s from %s.",
                    "something", (string)target->GetName()) );
                what->eventMove(this_object());
                return 1;
            }

           /* Steal was unsuccesful */
           else if( tmp == 2 ) {
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
        tmp = (mixed)target->eventSteal(who, what, target, skill2);

        /* You can't steal from this target */
        if( !tmp )
          return "You cannot steal from " + (string)target->GetName() + ".";

        /* Steal from target was succesful */
        else if( tmp == 1 ) {
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
        else if( tmp == 2) {
        AddSkillPoints("stealing", GetSkillLevel("stealing")*2);
        AddStaminaPoints(-5);
        return 1;
        }

        return tmp;
}
/*******************************************************************/
    if( GetInCombat() ) {
 who->eventPrint(GetName() + " is busy in combat.");
        return 0;
    }
    skill -= GetMobility()/2;
    skill -= GetStatLevel("agility");
        skill -= GetStatLevel("wisdom");
    skill -= GetStatLevel("luck")/3;

    if( objectp(what) ) sr = 100 * sizeof(what);
    else sr = 100;
    if( random(sr) > skill ) {
        eventPrint("You notice " + (string)who->GetName() + " trying "
                   "to steal from you!");
         if( !userp(this_object()) ) {
            who->eventPrint("%^RED%^" + (string)GetName() + "%^RED%^ "
                                  "notices your attempt at treachery!",
                                  environment(who) );
            eventForce("attack " + (string)who->GetKeyName());
          this_object()->SetProperty("steal victim", 1);
        }
        return 2;
    }

    if( random(2*sr) > skill ) {
         who->eventPrint("You are unsure if anyone noticed your foolish "
                             "attempt at thievery.",environment(who) );
         return 2;
        }

    if( stringp(what) ) {

foreach(string curr in GetCurrencies()) {
            int x;

            if( !(x = random(GetCurrency(curr)/5)) ) continue;
            if( x > 100 ) x = 100;
            x = (x * skill)/100;
            AddCurrency(curr, -x);
            who->AddCurrency(curr, x);
        }
        return 1;
    }
    for(i=0; i<sizeof(what); i++) {
        if( (mixed)what[i]->eventSteal(who) != 1 ) what[i] = 0;
    }
    return 1;
}
int AddCarriedMass(int x) { return carry::AddCarriedMass(x); }

int GetCarriedMass() {
    return (currency::GetCurrencyMass() + carry::GetCarriedMass());
}

int GetMaxCarry() { return combat::GetMaxCarry(); }

int SetPK(int x) { return (isPK = x); }

int GetPK() { return isPK; }

mixed indirect_look_at_obj_word_obj() {
    return 0;
}
