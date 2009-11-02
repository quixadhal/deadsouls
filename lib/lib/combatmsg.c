/*   /lib/combatmsg.c
 *   from the Dead Souls LPC Library
 *   combat message handling
 *   created by Blitz@Dead Souls
 */

#include <combat_messages.h>
#include "include/combatmsg.h"

static mixed GetMissData(object targ, int type, string limb){
    string targ_name = targ->GetName(); 

    if(targ->GetDead() || this_object()->GetDead()) return 0;

    if( type == -2 ) switch( random(7) ){
        case 0:
            return ({ "%s completely %s %s.",
                    ({ "You", "miss", targ_name }),
                    ({ GetName(), "misses", "you" }),
                    ({ GetName(), "misses", targ_name }) });
        case 1:
            return ({ "%s %s at %s and %s with thin air.",
                    ({ "You", "swing", targ_name, "connect" }),
                    ({ GetName(), "swings", "you", "connects" }),
                    ({ GetName(), "swings", targ_name, "connects" }) });
        case 2:
            return ({ "%s %s a stunning blow but %s %s by a mile!",
                    ({ "You", "deliver", "miss", targ_name }),
                    ({ GetName(), "delivers", "misses", "you" }),
                    ({ GetName(), "delivers", "misses", targ_name }), });
        case 3:
            return ({ "After a flurry of attacks, %s %s absolutely nothing.",
                    ({ "you", "hit" }),
                    ({ GetName(), "hits" }),
                    ({ GetName(), "hits" }), });
        case 4:
            return ({ "%s %s out at %s %s, but %s it completely.",
                    ({ "You", "lash", possessive_noun(targ), limb, "miss" }),
                    ({ GetName(), "lashes", "your", limb, "misses" }),
                    ({ GetName(), "lashes", possessive_noun(targ), limb, "misses" }) });
        case 5:
            return ({ "%s %s silly as %s %s in the wrong direction.",
                    ({ "You", "look", "you", "swing" }),
                    ({ GetName(), "looks", nominative(this_object()), "swings" }),
                    ({ GetName(), "looks", nominative(this_object()), "swings" }) });
        default:
            return ({ "%s totally %s %s.",
                    ({ "You", "miss", targ_name }),
                    ({ GetName(), "misses", "you" }),
                    ({ GetName(), "misses", targ_name }) });   
    }
    else {
        string pos = possessive_noun(this_object());
        switch( random(7) ){
            case 0:
                return ({ "%s quickly %s out of %s way.",
                        ({ targ_name, "jumps", "your" }),
                        ({ "You", "jump", pos }),
                        ({ targ_name, "jumps", pos }) });
            case 1:
                return ({ "%s deftly %s %s pathetic attack.",
                        ({ targ_name, "blocks", "your" }),
                        ({ "You", "block", pos }),
                        ({ targ_name, "blocks", pos }) });
            case 2:
                return ({ "%s easily %s %s inept attack.",
                        ({ targ_name, "dodges", "your" }),
                        ({ "You", "dodge", pos }),
                        ({ targ_name, "dodges", pos }) });
            case 3:
                return ({ "%s narrowly %s %s quick swing.",
                        ({ targ_name, "avoids", "your" }),
                        ({ "You", "avoid", pos }),
                        ({ targ_name, "avoids", pos }) });
            case 4:
                return ({ "%s attack is thwarted by %s quick defenses.",
                        ({ "Your", possessive_noun(targ_name) }),
                        ({ pos, "your" }),
                        ({ pos, possessive_noun(targ_name) }) });
            case 5:
                return ({ "%s %s beneath %s attack.",
                        ({ targ_name, "ducks", "your" }),
                        ({ "You", "duck", pos }),
                        ({ targ_name, "ducks", pos }) });
            default:
                return ({ "%s %s %s lame attack.",
                        ({ targ_name, "dodges", "your" }),
                        ({ "You", "dodge", pos }),
                        ({ targ_name, "dodges", pos }) });
        }
    }
    return 0;
}

static void eventSendMissMessages(object target, int x, string limb){
    mixed data;
    object env = room_environment();

    if(target->GetDead() || this_object()->GetDead()) return;
    if( !limb ) limb = "body";
    data = GetMissData(target, x, limb);
    if( sizeof(data) != 4 ) return;
    this_object()->eventPrint(sprintf(data[0], data[1]...));
    target->eventPrint(sprintf(data[0], data[2]...), ({ this_object() }));
    env->eventPrint(sprintf(data[0], data[3]...), 
            ({ this_object(), target }) );
}

mixed GetCombatVerbs(string type, int damage){
    mixed ptr;
    switch(type){
        case "knife": case "blade":
            ptr = BLADE_DEGREES;
        break;
        case "projectile":
            ptr = PROJECTILE_DEGREES;
        break;
        default:
        ptr = BLUNT_DEGREES;
        break;
    }
    if( !sizeof(ptr) || !arrayp(ptr))
        return ({ ({ "hit", "hits" }), ({ "solidly" }) });
    damage /= 5;
    if( damage > (sizeof(ptr) - 1) ) damage = (sizeof(ptr) - 1);
    if( sizeof(ptr) == 1 ) return ({ ptr[damage], ({ "solidly" }) });
    else return ptr[damage];
}

mixed GetCombatMove(string type, int skill){
    int i;
    mixed foo;

    if( skill < random(100) ) return 0;
    if( !i = sizeof(MOVE_TYPES) ) return 0;
    foo = MOVE_TYPES[ random(i) ];
    if( sizeof(foo) < 2 ) return 0;
    else return foo;
}

varargs void SendMeleeMessages(object target, int x, string targlimb, string limb){
    int i;
    string adverb;
    mixed verb, ptr, moves;
    object env = room_environment();

    //if(target->GetDead() || this_object()->GetDead()) return;
    if( x < 0 ){
        eventSendMissMessages(target, x, limb);
        return;
    }
    ptr  = GetCombatVerbs("melee", x);
    verb = ptr[0];
    if( sizeof(ptr) > 1 && i = sizeof(ptr[1]) )
        adverb = (ptr[1][ random(i) ] + " in");
    else adverb = "in";
    if( moves = GetCombatMove("melee", 
                this_object()->GetSkillLevel("melee attack")) ){
        verb[0] = moves[0] + " and " + verb[0];
        verb[1] = moves[1] + " and " + verb[1];
    }
    if( !limb ) limb = "attack";
    if( !targlimb ) targlimb = "body";
    this_object()->eventPrint(sprintf("You %s %s %s the %s with your %s.",
                verb[0], target->GetName(), adverb, targlimb, limb) );
    target->eventPrint(sprintf("%s %s you %s your %s with %s %s.",
                GetName(), verb[1], adverb, targlimb, possessive(this_object()), limb), ({ this_object() }));
    env->eventPrint(sprintf("%s %s %s %s the %s with %s %s.",
                GetName(), verb[1], target->GetName(), adverb, targlimb, 
                possessive(this_object()), limb), ({ target, this_object() }) );
    flush_messages();
}

varargs void SendWeaponMessages(object target, int x, object weapon, string limb){
    int i;
    string adverb, type, weap;
    mixed verb, ptr, moves;
    object env = room_environment();

    //if(target->GetDead() || this_object()->GetDead()) return;
    if( x < 0 ){
        eventSendMissMessages(target, x, limb);
        return;
    }
    if( weapon ){
        type = weapon->GetWeaponType();
        weap = weapon->GetKeyName();
    }
    else {
        type = "blunt";
        weap = "weapon";
    }
    ptr = GetCombatVerbs(type, x);
    verb = ptr[0];
    if( sizeof(ptr) > 1 && i = sizeof(ptr[1]) )
        adverb = (ptr[1][ random(i) ] + " in");
    else adverb = "in";
    if( moves = GetCombatMove(type, 
                this_object()->GetSkillLevel(type + " attack")) ){
        verb[0] = moves[0] + " and " + verb[0];
        verb[1] = moves[1] + " and " + verb[1];
    }
    if( !limb ) limb = "body";
    this_object()->eventPrint(sprintf("You %s %s %s the %s with your %s.",
                verb[0], target->GetName(), adverb, limb, weap) );
    target->eventPrint(sprintf("%s %s you %s your %s with %s %s.",
                GetName(), verb[1], adverb, limb, possessive(this_object()), weap), ({ this_object() }) );
    env->eventPrint(sprintf("%s %s %s %s the %s with %s %s.",
                GetName(), verb[1], target->GetName(), adverb, limb,
                possessive(this_object()), weap), ({ target, this_object() }) );
    flush_messages();
}
