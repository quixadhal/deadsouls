/*    /lib/body.c
 *    from the Dead Souls LPC Library
 *    handles information regarding bodies as well as events which
 *    affect them
 *    created by Descartes of Borg 950121
 *    Version: @(#) body.c 1.24@(#)
 *    Last Modified: 96/12/21
 */

#include <lib.h>
#include <daemons.h>
#include <function.h>
#include <medium.h>
#include <position.h>
#include <armour_types.h>
#include <damage_types.h>
#include <magic_protection.h>
#include "include/body.h"

inherit LIB_PERSIST;
inherit LIB_POSITION;
inherit LIB_UNDEAD;

#define COLLAPSE_AT            10.0

private int HealthPoints, MagicPoints;
private int Alcohol, Caffeine, Food, Drink, Poison, Sleeping;
private float StaminaPoints;
private string Torso;
private mapping Fingers, Limbs, MissingLimbs;
private static int Dying, LastHeal;
private static function Protect;
private static mapping WornItems;
private static class MagicProtection *Protection;
static private int HeartModifier = 0;

static void create() {
    AddSave( ({ "HealthPoints", "MagicPoints", "StaminaPoints", "Undead",
                "Limbs", "MissingLimbs", "WornItems" }) );
    NewBody(0);
    Protect = 0;
    WornItems = ([]);
    Limbs = ([]);
    Food = Drink = 100;
    Alcohol = Caffeine = 0;
    HealthPoints = MagicPoints = 50;
    StaminaPoints = 50.0;
    Dying = 0;
    LastHeal = time();
    Protection = ({});
}

mixed direct_turn_liv() {
    if( GetUndead() ) {
	return 1;
    }
    else {
	return "You cannot turn the living!";
    }
}

static void heart_beat() {
    object env = environment();
    int i;

    undead::heart_beat();
    if( i = sizeof(Protection) ) {
      while(i--)
        if( Protection[i]->time && (--Protection[i]->time < 1) )
          RemoveMagicProtection(i);
    }
    if( env && (GetResistance(GAS) != "immune") ) {
	if( (i = env->GetPoisonGas()) > 0 ) {
	    eventPrint("You choke on the poisonous gases.");
	    eventReceiveDamage(0, GAS, i);
	}
    }
    eventCheckHealing();
}

void eventReconnect() {
    LastHeal = time();
}

/************      /lib/body.c Modal Methods Section      ************/

mixed CanRemoveItem(object ob) { return 1; }

/************      /lib/body.c Events Section      ************/

private void checkCollapse() {
    float h = percent(GetHealthPoints(), GetMaxHealthPoints());

    if( h < COLLAPSE_AT ) {
	SetParalyzed(3, (: checkCollapse :));
	return;
    }
    eventPrint("You feel some strength returning.");
}

int eventCollapse() {
    int position = GetPosition();
    
    SetParalyzed(3, (: checkCollapse :));
    if( position == POSITION_LYING ) {
	return 1;
    }
    send_messages("collapse", "$agent_name $agent_verb to the ground.",
		  this_object(), 0, environment());
    SetPosition(POSITION_LYING);
    return 1;
}

void eventCheckHealing() {
    int x, y;

    x = GetHeartRate() * 10;
    if( (y = time() - LastHeal)  >= x ) {
        LastHeal = time();
        do {
            eventCompleteHeal(GetHealRate());
        } while( (y = y - x) >= x );
	if( Alcohol > 0 ) {
	    Alcohol--;
	    if( !Alcohol ) {
		message("my_action", "You are left with a pounding headache.",
			this_object());
		AddHealthPoints(-(random(3) + 1));
	    }
	    else if( !GetSleeping() && random(100) < 8 ) {
		string verb, adv;
		
		switch(random(5)) {
		    case 0: verb = "burp"; adv = "rudely"; break;
			    case 1: verb = "look"; adv = "ill"; break;
			    case 2: verb = "hiccup"; adv = "loudly"; break;
			    case 3: verb = "stumble"; adv = "clumsily"; break;
			    case 4: verb = "appear"; adv = "drunk"; break;
			}
		message("my_action", "You " + verb + " " + adv + ".",
			this_object());
		message("other_action", GetName() + " " + pluralize(verb) + " " +
			adv + ".", environment(), ({ this_object() }));
	    }
	}
	if( Sleeping > 0 ) {
	    Sleeping--;
	    if( !Sleeping ) {
		message("my_action", "You wake up!", this_object());
		message("other_action", GetName() + " wakes up from " +
			possessive(this_object()) + " deep sleep.",
			environment(this_object()), ({ this_object() }));
	    }
	    else if( random(100) < 8 ) {
		message("my_action", "You snore.", this_object());
		message("other_action", (string)this_player()->GetName() +
			" snores loudly.", environment(this_object()),
			({ this_object() }));
	    }
	}
	if( Caffeine > 0 ) Caffeine--;
	if( Food > 0 ) Food--;
	if( Drink > 0 ) Drink--;
    }
}

void eventCompleteHeal(int x) {
    eventHealDamage(x, 1, GetLimbs());
    AddMagicPoints(x + 1);
    AddStaminaPoints(x);
}

mixed eventFall() {
    object env = environment();
    string dest;

    if( !env ) {
	return 0;
    }
    if( GetPosition() == POSITION_LYING ) {
	return 0;
    }
    if( env->GetMedium() == MEDIUM_LAND ) {
	return position::eventFall();
    }
    dest = env->GetGround();
    if( !dest ) {
	send_messages(({ "fall", "die" }), "$agent_name $agent_verb into a "
	              "dark abyss and $agent_verb.", this_object(), 0, env);
	SetPosition(POSITION_LYING);
	eventDie();
    }
    else {
	int p = random(100) + 1;
	int was_undead = GetUndead();

	send_messages("fall", "$agent_name $agent_verb through the sky "
		      "towards the world below.", this_object(), 0, env);
	eventMove(dest);
	environment()->eventPrint(GetName() + " comes falling in from above.",
				  this_object());
	SetPosition(POSITION_LYING);
	foreach(string limb in GetLimbs()) {
	    int hp = GetHealthPoints(limb);

	    p = (hp * p)/100;
	    eventReceiveDamage(0, BLUNT, p, 0, ({ limb }));
	    if( Dying || (was_undead != GetUndead()) ) {
		break;
	    }
	}
    }
}

/* varargs int eventHealDamage(int x, int internal, mixed limbs)
 * int x - amount of damage being healed, negatives illegal (required)
 * int internal - internal damage flag (optional)
 * mixed limbs - limb or limbs affected by the heal event (optional)
 *
 * defaults
 * internal defaults to 0
 * limbs defaults to 0
 *
 * description
 * this event is triggered whenever something performs a healing action
 * on the body
 *
 * if the internal flag is set then overall health is healed.
 * if limbs are specified then the specified limbs are healed.
 * if the internal flag is NOT set and NO limbs are specified (default)
 *   then both overall health as well as the health of all limbs are healed.
 *
 * returns the actual amount of healing done or -1 if an error occurs
 */
varargs int eventHealDamage(int x, int internal, mixed limbs) {
    if(!limbs && !internal) {
	limbs = GetLimbs(); internal = 1;
    }
    else if(stringp(limbs)) {
	limbs = ({ limbs });
    }
    if(!limbs) {
	limbs = ({});
    }
    if( !arrayp(limbs)) {
	error("Bad argument 3 to eventHealDamage().\n");
    }
    if(internal) {
	AddHealthPoints(x);
    }
    map(limbs, (: AddHealthPoints($(x), $1) :));
    return x;
}

/* varargs int eventReceiveDamage(object agent, int type, int x,
 *     int internal, mixed limbs)
 * object agent - the living object responsible for this damage (required)
 * int type - the damage type(s) being done (required)
 * int x - the amount of damage being done, negatives illegal (required)
 * int internal - flag for internal or external damage (optional)
 * mixed limbs - limbs to which damage has been done (optional)
 *
 * defaults
 * internal defaults to 0
 * limbs defaults to 0
 *
 * description
 * handles sorting out damage events which happen to the body
 * It assumes some agent is acting as the cause of this event, an
 * agent being some sort of being as the cause of the event
 * the previous_object() is considered to be doing the actual damage
 * what does the damage is different than the agent in the sense
 * that a knife does damage and a living thing is the agent
 * can't get into more detail without getting philosophical, which is
 * beyond the scope of a comment
 * "x" amount of damage gets attempted, modified by varying things like
 * armour and natural resistence to this type of damage
 * the internal flag with no limbs specified means that the damage
 * is taken solely to the overall health
 * if the internal flag is set with limbs, the damage is both internal and
 * done to named limbs
 * if no internal flag is set and no limbs are specified, damage is done to
 * all limbs
 * if no internal flag is set and limbs are specified, then damage is done
 * only to the specified limbs
 * NOTE: internal damage is not modified by armour worn
 *
 * returns the average actual amount of damage done
 */

varargs int eventReceiveDamage(object agent, int type, int x, int internal,
mixed limbs) {
    string tmp = GetResistance(type);
    int fp;

    if( tmp == "immune") {
	return 0;
    }
    switch(tmp) {
        case "low": x = (3*x)/4; break;
        case "medium": x /= 2; break;
        case "high": x /= 4; break;
    }
    if( fp = functionp(Protect) ) {
        if( !(fp & FP_OWNER_DESTED) ) {
            function f;

            f = Protect;
            Protect = 0;
            x -= evaluate(f, this_object(), agent, type, x, limbs);
        }
    }
    x = eventCheckProtection(agent, type, x);
    if( !limbs ) {
        if( internal ) {
            AddHealthPoints(-x, 0, agent);
            return x;
        }
        else {
	    limbs = GetLimbs();
	}
    }
    else if( stringp(limbs) ) {
	limbs = ({ limbs });
    }
    else if( !arrayp(limbs) ) {
	return -1;
    }
    if( internal ) {
        limbs = filter(limbs, (: !AddHealthPoints(-$(x), $1, $(agent)) :));
        map(limbs, (: (Limbs[$1] ? RemoveLimb($1, $(agent)) : 0) :));
        AddHealthPoints(-x, 0, agent);
        return x;
    }
    else {
        int i, y, maxi;

        y = 0;
        for(i=0, maxi = sizeof(limbs); i < maxi; i++) {
            object *obs;
            int j, z;

            z = x;
            if(!Limbs[limbs[i]]) {          /* no limb, no damage */
                y += z;
                continue;
            }
            if(!(j = sizeof(obs = GetWorn(limbs[i])))) { /* no armour */
                y += z;                     /* add to total damage */
                if( !AddHealthPoints(-z, limbs[i], agent) )
                  RemoveLimb(limbs[i], agent);
                continue;
            }
            while(j--) {
                z -= (int)obs[j]->eventReceiveDamage(agent,type, z, 0, limbs[i]);
                if(z < 1) break;
            }
            if(z < 1) continue;
            else {
                y += z;
                if(!AddHealthPoints(-z, limbs[i], agent))
                  RemoveLimb(limbs[i], agent);
            }
        }
        y = y / (maxi ? maxi : 1);
        if( y ) AddHealthPoints(-y, 0, agent);
        return y;
    }
    AddHealthPoints(-x, 0, agent);
    return x;
}

/*  int eventCheckProtection(object agent, int type, int damage)
 *
 *  agent  : object doing the damage
 *  type   : damage type(s)
 *  damage : original amount damage being done
 *
 *  This function cycles through any magic protection found, reducing
 *  damage accordingly.
 *
 *  returns modified damage
 */
int eventCheckProtection(object agent, int type, int damage) {
    int i, y;
    if( !i = sizeof(Protection) ) return damage;
    while(i--) {
        int x;
	
        if( (type & Protection[i]->bits) != type ) continue;
        if( Protection[i]->absorb ) {
            if( (x = (Protection[i]->absorb - damage)) < 1 ) {
                x = Protection[i]->absorb;
                RemoveMagicProtection(i);
                damage -= x;
                if( damage < 1 ) return 0;
                continue;
            }
            Protection[i]->absorb -= damage;
        }
        else if( Protection[i]->protect )
            x = (random(Protection[i]->protect / 2) +
                (Protection[i]->protect / 2));
        else {
            RemoveMagicProtection(i);
            continue;
        }
        if( y = functionp(Protection[i]->hit) ) {
            if( y == FP_OWNER_DESTED ) {
                RemoveMagicProtection(i);
                continue;
             }
            else x = (int)evaluate(Protection[i]->hit, this_object(),
                                    agent, x, Protection[i]);
        }
        damage -= x;
        if( damage < 1 ) return 0;
    }
    return damage;
}

mixed eventReceiveThrow(object who, object what) {
    int x;
    
    if( what->GetClass() > 1 ) {
	int mod = who->GetSkillLevel("projectile attack") +
	          who->GetStatLevel("strength");

	mod = mod/2;
	x = what->eventStrike(this_object()) * 2;
	x = (x*mod)/100;
	if( what->GetWeaponType() != "projectile" ) {
	    x = x/4;
	}
	x = eventReceiveDamage(who, what->GetDamageType(), x, 0, 
			       GetRandomLimb("torso"));
	if( x > 0 ) {
	    who->AddSkillPoints("projectile attack", x);
	}
    }
    else {
	x = 0;
    }
    if( x < 1 ) {
	environment()->eventPrint(GetName() + " catches " +
				  possessive_noun(who->GetName()) + " " +
				  what->GetKeyName() + ".",
				  ({ this_object(), who }));
	eventPrint("You catch " + possessive_noun(who->GetName()) + " " +
		   what->GetKeyName() + ".");
	who->eventPrint(GetName() + " catches your " + what->GetKeyName()
			+ ".");
    }
    else {
	environment()->eventPrint(GetName() + " takes damage from " +
				  possessive_noun(who->GetName()) + " " +
				  what->GetKeyName() + ".",
				  ({ this_object(), who }));
	eventPrint("You take damage from " + possessive_noun(who->GetName()) +
		   " " + what->GetKeyName() + ".");
	who->eventPrint(GetName() + " takes damage from your " +
			what->GetKeyName() + ".");
    }
    what->eventMove(this_object());
    return 1;
}

/* varargs int eventDie(object agent)
 * object agent - the agent responsible for the death (optional)
 *
 * description
 * Kills the owner of this body if not already dying
 *
 * returns true if the thing is dying
 */
varargs int eventDie(object agent) {
    int x;

    SetUndead(!(x = GetUndead()));
    if( agent ) {
        if( x ) agent->eventDestroyEnemy(this_object());
        else agent->eventKillEnemy(this_object());
    }
    if( environment() ) {
        object *obs;
        string *currs;
        object ob;
        string curr;
        int i;

        ob = new(LIB_CORPSE);
        ob->SetCorpse(this_object());
        ob->eventMove(environment());
        obs = filter(all_inventory(), (: !((int)$1->GetRetainOnDeath()) :));
        i = sizeof(obs);
        while(i--) obs[i]->eventMove(ob);
        currs = (string *)this_object()->GetCurrencies() || ({});
        foreach(curr in currs) {
            object pile;
            int amt;

            if( amt = (int)this_object()->GetCurrency(curr) ) {
                pile = new(LIB_PILE);
                pile->SetPile(curr, amt);
                pile->eventMove(ob);
                this_object()->AddCurrency(curr, -amt);
            }
        }
    }
    call_out( function() { Dying = 0; }, 0);
    return 1;
}

/* int eventRemoveItem(object ob)
 * object ob - the item being removed
 *
 * Removes a worn or wielded item from the list
 *
 * returns 1 on success, 0 on failure
 */
int eventRemoveItem(object ob) {
    string limb;

    foreach(limb in keys(WornItems)) {
        if( !WornItems[limb] ) continue;
        if( member_array(ob, WornItems[limb]) != -1) {
	    WornItems[limb] -= ({ ob });
	}
    }
    return 1;
}

/* int eventWear(object ob, mixed limbs)
 * object ob - the item being worn (wielded)
 * mixed limbs - string or array of limbs on which it is being worn
 *
 * description
 * marks the limbs "limbs" as being protected by the armour "ob"
 *
 * returns 1 if successful, 0 if failure
 */
int eventWear(object ob, mixed limbs) {
    string limb;

    if( stringp(limbs) ) {
	limbs = ({ limbs });
    }
    if( CanWear(ob, limbs) != 1 ) {
	return 0;
    }
    foreach(limb in limbs) {
        if( !WornItems[limb] ) WornItems[limb] = ({ ob });
        else WornItems[limb] += ({ ob });
    }
    return 1;
}

/************     /lib/body.c Data manipulation functions      *************/
void NewBody(string race) {
    if(!race)
      Limbs = ([ (Torso = "ooze") : ([ "parent" : 0, "children" : ({}),
      "health" : 50, "class" : 1, "armours" : 0 ]) ]);
    else Limbs = ([]);
    MissingLimbs = ([]);
    Fingers = ([]);
}

/* int CanWear(object armour, string *limbs)
 * object armour - the piece of armour being checked
 * string *limbs - the limbs on which the armour wants to be worn
 *    a nested array indicates that just one of the included limbs
 *    must be satisfied
 *
 * description
 * checks to see if a particular armour can be worn
 *
 * returns 1 if the armour can be worn on those limbs
 * returns 0 if the armour cannot be worn there for any reason
 *
 */
mixed CanWear(object ob, string *limbs) {
    string limb, verb_pr, verb_pt, short;
    int *types;
    int type, bad_types, i, maxi;

    if( !ob ) return 0;
    short = (string)ob->GetShort();
    if( !(type = (int)ob->GetArmourType()) )
      return capitalize(short) + " cannot be worn!";
    if( type & A_WEAPON ) {
        verb_pr = "wield";
        verb_pt = "wielded";
    }
    else {
        verb_pr = "wear";
        verb_pt = "worn";
    }
    if( !limbs || !(maxi = sizeof(limbs)) )
      return "Where should " + short + " be "+ verb_pt + "?";
    if( (string *)ob->GetWorn() )
      return "It is already being " + verb_pt + ".";

    // Verify that the the item can be worn on each limb specified by limbs.
    i = 0;
    foreach(limb in limbs) {
    
        // Nested arrays indicate that only one of the elements in the nested
        // array must be satisfied. Check for one valid limb, and replace the
        // nested array with the valid limb, if found.
        if( arrayp(limb) ) {
          string limb2;
          string validLimb = 0;
          int leastRings = -1;

          if(!sizeof(limb)) error("Bad limb specification to CanWear().\n");
          // Iterate through the nested array.
          foreach(limb2 in limb) {
            string* wornItems;

            // The limb will be valid if:
            // o The body has the named limb capable of wearing the armour type.
            // o There are no armours of the same type on that limb, except
            //   for rings, which can have up to GetFingers() of that type worn.
            if(!Limbs[limb2] || !(Limbs[limb2]["armours"] & type)) continue;
            wornItems = WornItems[limb2];
            
            // If the item is a ring, attempt to distribute the rings evenly
            // across the available hands.
            if(type == A_RING) {
              int currentRings;

              if(!sizeof(wornItems)) currentRings = 0;
              else currentRings = sizeof(filter(wornItems,
                (:(int)$1->GetArmourType() == A_RING:)));
              if(currentRings >= leastRings && leastRings != -1) continue;
              leastRings = currentRings;
              if(currentRings >= GetFingers(limb2)) continue;
            }
            else if(wornItems) {
              object wornItem;
              int tmpType = 0;

              foreach(wornItem in wornItems)
                if(wornItem) tmpType |= (int)wornItem->GetArmourType();
              if(tmpType & type) continue;
            }
            validLimb = limb2;
          }
          if(validLimb) limbs[i] = validLimb;
          else return "You cannot " + verb_pr + " that.";
        }
        else {
          if( !Limbs[limb] ) return "You have no " + limb + ".";
          if( !(Limbs[limb]["armours"] & type) ) {
              if( type & A_WEAPON )
                return "You cannot wield with " + limb + ".";
              else return "You cannot wear " + short + " on your " + limb + ".";
          }
        }
        i++;
    }
    switch(type) {
        case A_RING:
            if(maxi != 1)
              return "You can only wear " + short + " on one limb.";
            if( !WornItems[limbs[0]] ) return 1; /* nothing there, ring ok */
            /* count # worn rings */
            i = sizeof(filter(WornItems[limbs[0]],
              (: (int)$1->GetArmourType() == A_RING :)));
            if(i >= GetFingers(limbs[0]))
              return "You are already wearing too many rings there.";
            else return 1; /* ok */
        case A_GLOVE:
            if(maxi != 1)
              return "You can only wear " + short + " on one limb.";
            if( GetFingers(limbs[0]) > (int)ob->GetFingers() )
              return capitalize(short) + " does not seem to fit well on "
                "your " + limbs[0] + ".";
            bad_types = A_GLOVE | A_LONG_GLOVE | A_SOCK | A_LONG_SOCK |
              A_BOOT | A_LONG_BOOT;
            break;
        case A_LONG_GLOVE:
            if(maxi != 2)
              return capitalize(short) + " should be worn on two limbs.";
            if( limbs[0] == Limbs[limbs[1]]["parent"] ) { /* which is hand? */
                /* more fingers than this armour can stand */
                if(GetFingers(limbs[1]) > (int)ob->GetFingers())
                  return capitalize(short) + " does not seem to fit well on "
                    "your " + limbs[1] + ".";
            }
            else if(limbs[1] == Limbs[limbs[0]]["parent"]) {
                /* ok, first limb is hand, check it */
                if(GetFingers(limbs[0]) > (int)ob->GetFingers())
                  return capitalize(short) + " does not seem to fit well on "
                    "your " + limbs[1] + ".";
            }
            else return "Your " + limbs[0] + " is not connected to your " +
              limbs[1] + ".";
            bad_types = A_GLOVE | A_LONG_GLOVE | A_SOCK | A_LONG_SOCK |
              A_BOOT | A_LONG_BOOT;
            break;
        case A_BOOT: case A_SOCK:
            if(maxi != 1)
              capitalize(short) + " may only be worn on one limb.";
            if(type == A_SOCK) bad_types = A_SOCK | A_LONG_SOCK;
            else bad_types = A_BOOT | A_LONG_BOOT;
            break;
        case A_LONG_BOOT: case A_LONG_SOCK:
            if(maxi != 2)
              return capitalize(short) + " must be worn only on two limbs.";
            if(limbs[0] != Limbs[limbs[1]]["parent"] &&
              limbs[1] != Limbs[limbs[0]]["parent"])
              return "Your " + limbs[0] + " is not connected to your " +
                limbs[1] + ".";
            if(type == A_LONG_SOCK) bad_types = A_LONG_SOCK | A_SOCK;
            else bad_types = A_BOOT | A_LONG_BOOT;
            break;
        case A_HELMET: case A_VEST: case A_AMULET: case A_VISOR: case A_BELT:
            if(maxi != 1)
              return capitalize(short) + " may only be worn on one limb.";
            bad_types = type;
            break;
        case A_PANTS: case A_SHIRT:
            bad_types = type | A_CLOAK;
            break;
        case A_CLOAK:
            bad_types = A_CLOAK | A_SHIRT | A_PANTS;
            break;
        case A_SHIELD:
            foreach(limb in limbs) {
                object worn_item;
                int tmp = 0;

                if( !WornItems[limb] ) continue; /* no armours, no prob */
                if( !Limbs[limb]["parent"] ) continue; /* torso ok for 2 */
                foreach(worn_item in WornItems[limb])  {
                    if( !worn_item ) continue;
                    tmp |= (int)worn_item->GetArmourType();
                }
                /* not gonna allow 2 shields or a shield and weapon here */
                if( tmp & (A_SHIELD | A_WEAPON) )
                  return "You cannot wear " + short + " there right now.";
            }
            return 1; /* ok */
        case A_WEAPON:
            foreach(limb in limbs) {
                object worn_item;
                int tmp = 0;

                if( !WornItems[limb] ) continue; /* nothing there, ok */
                foreach(worn_item in WornItems[limb]) {
                    if( !worn_item ) continue;
                    tmp += (int)worn_item->GetArmourType();
                }
                /* again, not allowing 2 weapons or a shield and weapon */
                if(tmp & (A_SHIELD | A_WEAPON))
                  return "You cannot wield " + short + " there right now.";
            }
            return 1; /* ok */
        case A_ARMOUR: case A_BODY_ARMOUR:
            bad_types = A_ARMOUR | A_BODY_ARMOUR;
            break;
        default: return 0; /* not any illegal stuff */
    }
    foreach(limb in limbs) {
        object worn_item;
        int tmp = 0;
        if( !WornItems[limb] ) continue; /* no preventing types */
        foreach( worn_item in WornItems[limb] ) {
            if( !worn_item ) continue;
            tmp |= (int)worn_item->GetArmourType();
        }
        if(tmp & bad_types)
          return "You cannot " + verb_pr + " " + short + " there right now.";
    }
    return 1; /* ok */
}

/* int AddLimb(string limb, string parent, int classes, int *armours)
 * string limb - the limb being added (required)
 * string parent - the limb to which this one is being attached (required)
 * int classes - rating of the limb's strength (optional)
 * int *armours - the types of armours which can be worn here (optional)
 *
 * defaults
 * classes defaults to 1
 * armours defaults to ({})
 *
 * description
 * adds the named limb to the body, attached at the named point
 * the limb classes starts at 1 for a torso (strongest)
 * to whatever the documentation rates as the weakest
 *
 * returns 1 on success, 0 on failure
 */
varargs int AddLimb(string limb, string parent, int classes, int *armours) {
    int arm = 0;

    if(!limb || Limbs[limb] || (parent && !Limbs[parent])) return 0;
    if(armours) {
        int i;

        i = sizeof(armours);
        while(i--) arm |= armours[i];
    }
    if(MissingLimbs[limb]) map_delete(MissingLimbs, limb);
    if( parent ) Limbs[parent]["children"] += ({ limb });
    else Torso = limb;
    if( !classes ) classes = 5;
    Limbs[limb] = ([ "parent" : parent, "children" : ({}), "class" : classes,
      "armours" : arm ]);
    Limbs[limb]["health"] = GetMaxHealthPoints(limb);
    return 1;
}

int RestoreLimb(string limb) {
    if( !MissingLimbs[limb] ) return 0;
    Limbs[limb] = MissingLimbs[limb];
    Limbs[limb]["health"] = GetMaxHealthPoints(limb);
    map_delete(MissingLimbs, limb);
    return 1;
}

/* int DestLimb(string limb)
 * string limb - the limb being removed
 *
 * description
 * removes a limb from the limbs mapping.  The limb isn't marked as mising,
 * and the monster doesn't die even if you remove a fatal limb.  Useful for
 * removing limbs from standard race types.  Removing the torso isn't allowed.
 *
 * returns -1 on error, 0 on failure, 1 on success
 */
int DestLimb(string limb) {
    string *kiddies;
    int i;

    if(!limb || !Limbs[limb]) return -1;
    if(!Limbs[limb]["parent"]) {
        return -1;
    }
    Limbs[Limbs[limb]["parent"]]["children"] -= ({ limb });
    if( (i = sizeof(kiddies = Limbs[limb]["children"])) )
      while(i--) DestLimb(kiddies[i]);
    map_delete(Limbs, limb);
    return 1;
}

/* int RemoveLimb(string limb, object agent)
 * string limb - the limb being removed
 * object agent - the agent who is responsible for the limb removal
 *
 * description
 * removes a limb from the limbs mapping and stores vital data in the
 * missing limbs mapping
 *
 * returns -1 on error, 0 on failure, 1 on success
 */
int RemoveLimb(string limb, object agent) {
    string *kiddies;
    int i;

    if(!limb || !Limbs[limb]) return -1;
    if(!Limbs[limb]["parent"] || Limbs[limb]["class"] == 1) {
        HealthPoints = 0;
        if( !Dying ) {
            Dying = 1;
            call_out((: eventDie, agent :), 0);
        }
        return 0;
    }
    Limbs[Limbs[limb]["parent"]]["children"] -= ({ limb });
    if( (i = sizeof(kiddies = Limbs[limb]["children"])) )
      while(i--) RemoveLimb(kiddies[i], agent);
    MissingLimbs[limb] = Limbs[limb];
    map_delete(Limbs, limb);
    if( environment() ) {
        object ob;

        message("environment", possessive_noun(GetName()) + " " + limb +
                " is severed!", environment(), ({ this_object() }));
        message("environment", "Your "+ limb + " is severed!", this_object());
        ob = new(LIB_LIMB);
        ob->SetLimb(limb, GetCapName(), GetRace());
        ob->eventMove(environment());
        i = sizeof(WornItems[limb]);
        while(i--) {
            WornItems[limb][i]->SetWorn(0);
            WornItems[limb][i]->eventMove(ob);
        }
        while( i = sizeof(WornItems[limb]) )
          eventRemoveItem(WornItems[limb][i]);
    }
    return 1;
}

mapping GetLimb(string limb) {
    if(!limb || !Limbs[limb]) return 0;
    else return copy(Limbs[limb]);
}

/* string GetRandomLimb(string targ)
 * string targ - the targetted limb
 *
 * description
 * returns a random limb weighted towards the targetted limb
 */

string GetRandomLimb(string targ) {
    string array limbs;

    if( !targ ) {
	targ = GetTorso();
    }
    if( member_array(targ, (limbs=keys(Limbs))) == -1) {
	targ= GetTorso(); /* no target or illegal target, weight torso */
    }
    targ = (limbs + (targ ? ({ targ, targ }) : ({})))[random(sizeof(limbs)+2)];
    return targ;
}

string GetTorso() {
    string *limbs;
    int i;

    i = sizeof(limbs = keys(Limbs));
    while(i--) {
	if(!Limbs[limbs[i]]["parent"]) {
	    return limbs[i];
	}
    }
    return 0;
}

string array GetLimbs() {
    return (Limbs ? keys(Limbs) : 0);
}

int GetLimbClass(string limb) { return Limbs[limb]["class"]; }

string GetLimbParent(string limb) { return Limbs[limb]["parent"]; }

string array GetLimbChildren(string limb) {
    return Limbs[limb]["children"] + ({});
}

mapping GetMissingLimb(string limb) {
    return (limb ? copy(MissingLimbs[limb]) : 0);
}

string array GetMissingLimbs() { return keys(MissingLimbs); }

string GetLong(string nom) {
    string *limbs;
    string str;
    float h;
    
    str = (nom = capitalize(nom));
    h = percent(GetHealthPoints(), GetMaxHealthPoints());
    if( h < 10.0 ) str += " is mortally wounded.\n";
    else if( h < 20.0 ) str += " is near death.\n";
    else if( h < 30.0 ) str += " is severely injured.\n";
    else if( h < 40.0 ) str += " is badly injured.\n";
    else if( h < 50.0 ) str += " is hurt.\n";
    else if( h < 60.0 ) str += " is slightly injured.\n";
    else if( h < 70.0 ) str += " has some cuts and bruises.\n";
    else if( h < 80.0 ) str += " is in decent shape.\n";
    else if( h < 90.0 ) str += " is quite keen.\n";
    else str += " is in top condition.\n";
    limbs = GetMissingLimbs();
    if( sizeof(limbs) ) {
	int i, maxi;

	str += nom + " is missing " + add_article(limbs[0]); 
	for(i=1, maxi = sizeof(limbs); i<maxi; i++) {
	    if( i < maxi-1 ) str += ", " + add_article(limbs[i]);
	    else {
		if( maxi > 2 ) str += ",";
		str += " and " + add_article(limbs[i]);
	    }
	}
	str += ".\n";
    }
    return str;
}

string array GetWieldingLimbs() {
    return filter(keys(Limbs), (: (Limbs[$1]["armours"] & A_WEAPON) :));
}

/* int AddFingers(string limb, int x)
 * string limb - the limb to which fingers will be added
 * int x - the number of fingers being added, can be negative
 *
 * description
 * adds the given number of fingers to the given limb
 *
 * returns the total number of fingers after addition
 */

varargs int AddFingers(string limb, int x) {
    if((Fingers[limb] += x) < 1) Fingers[limb] = 1;
    return Fingers[limb];
}

int GetFingers(string limb) {
    return Fingers[limb];
}

varargs object *GetWorn(string limb) {
    if(!limb) {
        object *ret = ({});
        string *limbs;
        int i;

        i = sizeof(limbs = keys(Limbs));
        while(i--) if(WornItems[limbs[i]]) ret += ({ WornItems[limbs[i]] });
        return distinct_array(ret);
    }
    else if(!WornItems[limb]) return ({});
    else return (WornItems[limb] + ({}));
}

varargs mixed GetWielded(string limb) {
    if(!limb) {
        object *ret = ({});
        string *limbs;
        int i;

        i = sizeof(limbs = keys(Limbs));
        while(i--) {
            if(!WornItems[limbs[i]]) continue;
            else ret += filter(WornItems[limbs[i]],
              (: (int)$1->GetArmourType() == A_WEAPON :));
        }
        return distinct_array(ret);
    }
    else if(!WornItems[limb]) return 0;
    else {
        object *ret;

        ret = filter(WornItems[limb], (: (int)$1->GetArmourType() == A_WEAPON :));
        if(sizeof(ret)) return ret[0];
        else return 0;
    }
}

/* varargs static int AddHealthPoints(int x, string limb, object agent)
 * int x - number of points being added, may be negative (required)
 * string limb - the limb to which health is being added (optional)
 * object agent - the living responsible for this damage
 *
 * defaults
 * limb defaults to 0
 *
 * description
 * if the value of limb is not zero, then "x" number of health points will
 * be added to limb "limb"
 * if he value is 0, then the overall health points will be modified
 *
 * returns the remaining number of health points for the limb in question
 * or for the overall health points
 */

varargs static int AddHealthPoints(int x, string limb, object agent) {
    int y;

    if( limb ) {
        if( !Limbs[limb] ) return -1;
        if((Limbs[limb]["health"] += x) < 1) Limbs[limb]["health"] = 0;
        else if(Limbs[limb]["health"] > (y = GetMaxHealthPoints(limb)))
          Limbs[limb]["health"] = y;
        return Limbs[limb]["health"];
    }
    else {
        if((HealthPoints += x) < 1) HealthPoints = 0;
        else if(HealthPoints > (y = GetMaxHealthPoints())) HealthPoints = y;
        if( HealthPoints < 1 ) {
            if( !Dying ) {
                Dying = 1;
                call_out( (: eventDie, agent :), 0);
            }
        }
	else {
	    float h = percent(GetHealthPoints(), GetMaxHealthPoints());

	    if( h < COLLAPSE_AT ) {
		eventCollapse();
	    }
	}
        return HealthPoints;
    }
}

varargs int GetHealthPoints(string limb) {
    if(limb) {
        if(!Limbs[limb]) return -1;
        else return Limbs[limb]["health"];
    }
    else return HealthPoints;
}

varargs int GetMaxHealthPoints(string limb) { return 0; }

/* int AddMagicPoints(int x)
 * int x - the number of magic points being added, may be negative
 *
 * description
 * adds magic points to the body
 *
 * returns the remaining magic points
 */

int AddMagicPoints(int x) {
    int y;

    if((MagicPoints += x) < 1) MagicPoints = 0;
    else if(MagicPoints > (y = GetMaxMagicPoints())) MagicPoints = y;
    return MagicPoints;
}

int GetMagicPoints() { return MagicPoints; }

int GetMaxMagicPoints() { return 0; }

/* int AddStaminaPoints(int x)
 * int x - number of stamina points being added
 *
 * description
 * adds "x" stamina points, can be negative
 *
 * returns the remaining number of stamina points
 */

float AddStaminaPoints(mixed x) {
    float y;

    if( !intp(x) && !floatp(x) )
      error("Bad argument 1 to AddStaminaPoints().\n");
    if( intp(x) ) x = to_float(x);
    if((StaminaPoints += x) < 0.1) StaminaPoints = 0.0;
    else if(StaminaPoints > (y = GetMaxStaminaPoints())) StaminaPoints = y;
    return StaminaPoints;
}

int GetStaminaPoints() { return to_int(StaminaPoints); }

float GetMaxStaminaPoints() {  return 0; }

int AddMagicProtection(class MagicProtection cl) {
    if( ( !cl->absorb && !(cl->protect && cl->time) ) ||
        ( cl->hit && !functionp(cl->hit) ) ||
        ( cl->end && !functionp(cl->end) ) ||
        ( !cl->bits )
      ) {
        error("Illegal class setting passed to AddMagicProtection.\n");
        return 0;
      }
    cl->timestamp = time();
    Protection += ({ cl });
    return 1;
}

class MagicProtection array GetMagicProtection() { return Protection; }

int RemoveMagicProtection(int i) {
    if( i > sizeof(Protection) - 1 ) return 0;
    if( Protection[i]->end ) {
	if( !(functionp(Protection[i]->end) & FP_OWNER_DESTED) ) {
	    evaluate(Protection[i]->end, this_object());
	}
    }
    Protection -= ({ Protection[i] });
    return 1;
}

int GetDying() { return Dying; }

int SetSleeping(int x) { return (Sleeping = x); }

int GetSleeping() { return Sleeping; }

int AddAlcohol(int x) { return (Alcohol += x); }

int GetAlcohol() { return Alcohol; }

int AddCaffeine(int x) { return (Caffeine += x); }

int GetCaffeine() { return Caffeine; }

int AddDrink(int x) { return (Drink += x); }

int GetDrink() { return Drink; }

int AddFood(int x) { return (Food += x); }

int GetFood() { return Food; }

int AddPoison(int x) {
    Poison += x;
    if( Poison < 1 ) {
	Poison = 0;
    }
    return Poison;
}

int GetPoison() { return Poison; }

string GetResistance(int type) { return "none"; }

string GetRace() { return 0; }

string GetName() { return 0; }

string GetCapName() { return 0; }

int GetHeartRate() {
    int rate;

    rate = (GetAlcohol() - GetCaffeine());
    if( rate > 50 ) rate = 6;
    else if( rate > 25 ) rate = 5;
    else if( rate > 0 ) rate = 4;
    else if( rate > -25 ) rate = 3;
    else rate = 2;
    rate += HeartModifier;
    if( rate < 1 ) {
	rate = 1;
    }
    else if( rate > 10 ) {
	rate = 10;
    }
    return rate;
}

int GetHealRate() {
    int heal;

    heal = 1 - (GetPoison() / 10);
    heal += (GetDrink() + GetFood()) / 25;
    heal *= (1 + (GetSleeping() > 1) + (GetAlcohol() > 50));
    return heal;
}

string GetHealthShort() {
    string cl, sh;
    float h;

    if( !(sh = GetShort()) ) return 0;
    h = percent(GetHealthPoints(), GetMaxHealthPoints());
    if( h > 90.0 ) cl = "%^BOLD%^GREEN%^";
    else if( h > 75.0 ) cl = "%^GREEN%^";
    else if( h > 50.0 ) cl = "%^BOLD%^BLUE%^";
    else if( h > 35.0 ) cl = "%^BLUE%^";
    else if( h > 20.0 ) cl = "%^BOLD%^RED%^";
    else cl = "%^RED%^";
    return cl + capitalize(sh);
}

mixed SetProtect(function f) { return (Protect = f); }

function GetProtect() { return Protect; }

int GetHeartModifier() {
    return HeartModifier;
}

varargs int AddHeartModifier(int x, int t) {
    HeartModifier += x;
    if( t > 0 ) {
	call_out((: AddHeartModifier(-$(x)) :), t);
    }
    return HeartModifier;
}
