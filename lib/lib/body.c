/*    /lib/body.c
 *    from the Dead Souls LPC Library
 *    handles information regarding bodies as well as events which
 *    affect them
 *    created by Descartes of Borg 950121
 *    Version: @(#) body.c 1.24@(#)
 */

#include <lib.h>
#include ROOMS_H
#include <daemons.h>
#include <function.h>
#include <medium.h>
#include <position.h>
#include <armor_types.h>
#include <respiration_types.h>
#include <terrain_types.h>
#include <damage_types.h>
#include <magic_protection.h>
#include "include/body.h"
#ifndef AUTO_ADVANCE
#define AUTO_ADVANCE 0
#endif

inherit LIB_POSITION;
inherit LIB_UNDEAD;
inherit LIB_CRAWL;
inherit LIB_FLY;
inherit LIB_SWIM;
inherit LIB_MOUNT;
inherit LIB_BODY_MASS;
inherit LIB_PERSIST;

#define COLLAPSE_AT            5.0
#ifndef SEVERABLE_LIMBS
#define SEVERABLE_LIMBS 1
#endif

private int HealthPoints, MagicPoints, ExperiencePoints, ExperienceDebt;
private int melee, godmode;
private int Alcohol, Caffeine, Food, Drink, Poison, Sleeping, DeathEvents;
private float StaminaPoints;
private string Torso, Biter, keepalive;
private mapping Fingers, Limbs, MissingLimbs;
private static int Dying, LastHeal, Encumbrance;
private static function Protect;
private static mapping WornItems;
private static class MagicProtection *Protection;
static private int HeartModifier = 0;
private static string PoliticalParty, BodyComposition;
private static int Pacifist, globalint1;
private static mapping Dimensions = ([]);
private int firearms_wounds;
string *ExtraChannels;
mixed Agent;

string GetRace();

static void create(){
    PoliticalParty = "UNDECIDED";
    firearms_wounds = 0;
    DeathEvents = 0;
    NewBody(0);
    Protect = 0;
    WornItems = ([]);
    Limbs = ([]);
    Food = Drink = 100;
    Alcohol = Caffeine = 0;
    HealthPoints = MagicPoints = 50;
    StaminaPoints = 50.0;
    ExperiencePoints = 50;
    ExperienceDebt = 0;
    Dying = 0;
    LastHeal = time();
    Protection = ({});
    ExtraChannels = ({});
    Dimensions = ([]);
}

varargs mixed eventBuy(mixed arg1, mixed arg2, mixed arg3){
    //This function will hopefully get overridden where appropriate.
    write(capitalize(this_object()->GetShort())+" isn't buying anything from you.");
    return 1;
}

int GetMass(){
    return body_mass::GetBodyMass();
}

int GetSize(int decimal){
    int i;
    if(!decimal && !undefinedp(Dimensions["Size"])){
        return Dimensions["Size"];
    }
    if(decimal && !undefinedp(Dimensions["SizeDecimal"])){
        return Dimensions["SizeDecimal"];
    }
    Dimensions["Size"] = RACES_D->GetRaceSize(GetRace());
    if(!decimal){
        return Dimensions["Size"];
    }
    i = 32;
    while(i){
        i--;
        if((1 << i) & Dimensions["Size"]){
            return (Dimensions["SizeDecimal"] = i);
        }
    }
    return (Dimensions["SizeDecimal"] = i);
}

int GetBodyType(){
    if(!undefinedp(Dimensions["BodyType"])){
        return Dimensions["BodyType"];
    }
    return (Dimensions["BodyType"] = RACES_D->GetRaceBodyType(GetRace()));
}

int SetMass(int i){
    return body_mass::SetBodyMass(i);
}

int SetSize(int i){
    return Dimensions["Size"] = i;
}

int SetBodyType(int i){
    return Dimensions["BodyType"] = i;
}


int GetEncumbrance(){
    int encumbrance = 0;
    object *stuff = filter(all_inventory(this_object()), (: !($1->GetWorn()) :) );

    if(!(ENABLE_ENCUMBRANCE) || inherits(LIB_NPC,this_object()) ) return encumbrance;
    if(sizeof(stuff)) foreach(object item in stuff) 
        encumbrance += (item->GetMass())/2;
    if(sizeof(stuff)) encumbrance += sizeof(stuff);
    return encumbrance;
}

string SetBodyComposition(string str){
    if(!str) return "";
    else BodyComposition = str;
    return BodyComposition;
}

string GetBodyComposition(){
    return BodyComposition;
}

int GetPacifist(){
    return Pacifist;
}

int SetPacifist(int i){
    if(!i) Pacifist = 0;
    else Pacifist = 1;
    return Pacifist;
}

int SetCanBite(int i){
    if(i) Biter = "yes";
    else Biter = "no";
    return i;
}

int GetCanBite(){
    if(sizeof(Biter)){
        if(Biter == "yes") return 1;
        else return 0;
    }
    else return RACES_D->GetBitingRace(this_object()->GetRace());
}

string *AddExtraChannels(string *chans){
    foreach(string chan in chans){
        if(member_array(chan,this_player()->GetExtraChannels()) == -1){
            ExtraChannels += ({ chan });
        }
    }
    return ExtraChannels;
}

string *RemoveExtraChannels(string *chans){
    foreach(string chan in chans){
        if(member_array(chan,this_player()->GetExtraChannels()) != -1){
            ExtraChannels -= ({ chan });
        }
    }
    return ExtraChannels;
}

string *GetExtraChannels(){
    return ExtraChannels;
}

string *SetExtraChannels(string *chans){
    ExtraChannels = chans;
    return ExtraChannels;
}

mixed direct_body_liv(){
    return 1;
}

mixed direct_turn_liv(){
    if( GetUndead() ){
        return 1;
    }
    else {
        return "You cannot turn the living!";
    }
}

void eventCheckEnvironment(){
    object env = environment();
    int i, roomres, breathdam;
    int restype;
    float j,k;

    if(!userp(this_object()) && !clonep(this_object())) return;

    restype = this_object()->GetRespiration();
    j = percent(GetHealthPoints(), GetMaxHealthPoints());
    k = percent(GetStaminaPoints(), GetMaxStaminaPoints());

    if( j < COLLAPSE_AT  || k < COLLAPSE_AT ){
        this_object()->eventCollapse();
    }

    if(RACES_D->GetLimblessRace(this_object()->GetRace()) &&
            this_object()->GetPosition() == POSITION_STANDING){
        this_object()->SetPosition(POSITION_FLOATING);
    }

    if(!env) return;
    if(env->GetMedium() == MEDIUM_AIR &&
            this_object()->GetPosition() != POSITION_FLYING){
        if(!(this_object()->CanFly())){
            this_object()->SetPosition(POSITION_FLOATING);
            call_out("eventFall", 1);
        }
        else this_object()->eventFly();
    }
    else if((env->GetTerrainType() & (T_SEAFLOOR | T_SPACE | T_UNDERWATER)) && 
            this_object()->GetPosition() == POSITION_FLYING){
        this_object()->SetPosition(POSITION_FLOATING);
    }
    else if((env->GetMedium() == MEDIUM_WATER ||
                env->GetMedium() == MEDIUM_SURFACE) &&
            this_object()->GetPosition() != POSITION_SWIMMING){
        if(!(this_object()->GetPosition() == POSITION_FLYING &&
                    environment()->GetMedium() == MEDIUM_SURFACE)){
            if(!(environment()->GetTerrainType() & (T_SEAFLOOR)) && this_object()->CanSwim()){
                if(this_object()->GetPosition() != POSITION_FLOATING){
                    this_object()->eventSwim();
                }
            }
            else if(!(environment()->GetTerrainType() & (T_SEAFLOOR))){
                this_object()->SetPosition(POSITION_FLOATING);
                call_out("eventSink", 1);
            }
        }
    }

    i = this_object()->CanBreathe();
    if(undefinedp(i)) i = 1;

    if(env->GetMedium() == MEDIUM_SPACE){
        this_object()->SetPosition(POSITION_FLOATING);
        if(restype != R_VACUUM){
            if(!i){
                breathdam = 1;
                j=this_object()->eventReceiveDamage("Outer space",ANOXIA,200,1);
                if(j) eventPrint("You are asphyxiating.");
            }
        }
    }
    else if(env->GetMedium() == MEDIUM_WATER){
        if(restype != R_VACUUM && restype != R_WATER){
            if(!i){
                breathdam = 1;
                j=this_object()->eventReceiveDamage("Water", ANOXIA, 100, 1);
                if(j) eventPrint("You are drowning.");
            }
        }
    }
    else if(restype == R_WATER && env->GetMedium() != MEDIUM_WATER){
        if(!i){
            breathdam = 1;
            j=this_object()->eventReceiveDamage("Air", ANOXIA, 100, 1);
            if(j) eventPrint("You are asphyxiating.");
        }
    }
    if(!breathdam){
        if(!i){
            j=this_object()->eventReceiveDamage("asphyxia", ANOXIA, 200, 1);
            if(j) eventPrint("You cannot breathe!");
        }
    }
    if(i && restype == R_AIR && (i = env->GetPoisonGas()) > 0 ){
        if( this_object()->GetResistance(GAS) != "immune" ){
            eventPrint("You choke on toxic gases.");
            this_object()->eventReceiveDamage("Poison gas", GAS, i, 1);
        }
    }
}

static void heart_beat(){
    int i;
    undead::heart_beat();
    if(!GetDying()){
        if( i = sizeof(Protection) ){
            while(i--)
                if( Protection[i]->time && (--Protection[i]->time < 1) )
                    RemoveMagicProtection(i);
        }
        eventCheckEnvironment();
        eventCheckHealing();
        if(!stringp(hobbled(this_player()))){
            this_object()->eventCollapse();
        }
    }
}

void eventReconnect(){
    LastHeal = time();
}

void ParseHook(string str){
    parse_sentence(str);
}


/************      /lib/body.c Modal Methods Section      ************/

mixed CanRemoveItem(object ob){ return 1; }

/************      /lib/body.c Events Section      ************/

private void checkCollapse(){
    float h = percent(GetHealthPoints(), GetMaxHealthPoints());
    float i = percent(GetStaminaPoints(), GetMaxStaminaPoints());

    if( h < COLLAPSE_AT  || i < COLLAPSE_AT ){
        SetParalyzed(3, (: checkCollapse :));
        return;
    }
    this_object()->eventPrint("You feel some strength returning.");
}

varargs int eventCollapse(int noparalyze){
    int position = GetPosition();
    int medium;
    if(environment()) medium = environment()->GetMedium();

    if(!this_object() || !environment()) return 0;

    foreach(object ob in all_inventory(environment())){
        if(inherits(LIB_CHAIR,ob) || inherits(LIB_BED,ob) ){
            ob->eventReleaseStand(this_object());
        }
    }

    if(medium == MEDIUM_LAND){
        if( position == POSITION_LYING ){
            return 1;
        }
        send_messages("collapse", "$agent_name $agent_verb to the ground.",
                this_object(), 0, environment());
        SetPosition(POSITION_LYING);
        if(!noparalyze) SetParalyzed(3, (: checkCollapse :));
        return 1;
    }

    if( position == POSITION_FLOATING ){
        return 1;
    }
    send_messages("go", "$agent_name $agent_verb limp.",
            this_object(), 0, environment());
    SetPosition(POSITION_FLOATING);
    if(!noparalyze) SetParalyzed(3, (: checkCollapse :));
    return 1;
}

void eventCheckHealing(){
    int x, y, lead;
    object dude;
    dude = this_object();
    lead = dude->GetLead();

    if(lead && !present("firearms_wound", dude)){
        object wound = new(LIB_WOUND);
        if(wound) wound->eventMove(dude);
    }

    if(interactive() && !environment()){
        this_object()->eventMove(ROOM_START); 
    }

    //This resets the parser counter.
    this_object()->DoneTrying();

    if(HealthPoints < 1 && !this_object()->GetDying()){
        this_object()->SetDying(1);
        call_out("eventDie", 0, "misfortune");
        return;
    }

    if(AUTO_ADVANCE && interactive() && !this_object()->GetDying()){
        PLAYERS_D->CheckAdvance(this_object());
    }

    x = GetHeartRate() * 10;

    if(dude->GetSleeping() > 0 && dude->GetPosition() != POSITION_LYING
            && dude->GetPosition() != POSITION_SITTING){
        dude->eventCollapse();
    }

    if(dude->GetInCombat()){
        if(dude->GetInvis()) dude->SetInvis(0);
        if(!interactive(dude) && !RACES_D->GetLimblessRace(dude->GetRace())){
            dude->SetAutoStand(1);
        }
    }

    if(!inherits(LIB_VEHICLE,this_object())){
        if( (y = time() - LastHeal)  >= x ){
            LastHeal = time();
            do {
                eventCompleteHeal(GetHealRate());
            } while( (y = y - x) >= x );
            if( Alcohol > 0 ){
                Alcohol--;
                if( !Alcohol ){
                    message("my_action", "You are left with a pounding headache.",
                            this_object());
                    AddHealthPoints(-(random(3) + 1));
                }
                else if( !GetSleeping() && random(100) < 8 ){
                    string verb, adv;

                    switch(random(5)){
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
            if( Sleeping > 0 ){
                Sleeping--;
                if( !Sleeping || dude->GetInCombat() ){
                    Sleeping = 0;
                    message("my_action", "You wake up!", this_object());
                    message("other_action", GetName() + " wakes up from " +
                            possessive(this_object()) + " deep sleep.",
                            environment(this_object()), ({ this_object() }));
                }
                else if( random(100) < 8 ){
                    message("my_action", "You snore.", this_object());
                    message("other_action", this_player()->GetName() +
                            " snores loudly.", environment(this_object()),
                            ({ this_object() }));
                }
            }
            if( Caffeine > 0 ) Caffeine--;
            if( Food > 0 ) Food--;
            if( Drink > 0 ) Drink--;
        }
    }
}

void eventCompleteHeal(int x){
    eventHealDamage(x, 1, GetLimbs());
    AddMagicPoints(x + 1);
    AddStaminaPoints(x);
}

mixed eventFall(){
    object env = environment();
    object *riders;
    string dest;

    if( !env ){
        return 0;
    }
    if( GetPosition() == POSITION_LYING ){
        return 0;
    }
    if( env->GetMedium() == MEDIUM_AIR ){
        return position::eventFall();
    }
    dest = env->GetGround();
    if(!dest){
        int p;
        int was_undead = GetUndead();

        eventMove(dest);
        this_object()->eventCollapse();
        foreach(string limb in GetLimbs()){
            int hp = GetHealthPoints(limb);

            p = random(hp);
            this_object()->eventReceiveDamage("Deceleration sickness", BLUNT, p, 0, ({ limb }));
            if( Dying || (was_undead != GetUndead()) ){
                break;
            }
        }
        riders = get_livings(this_object());
        if(sizeof(riders)){
            foreach(object passenger in riders){
                passenger->eventReceiveDamage("Deceleration sickness", BLUNT, p, 0,
                        ({ passenger->GetTorso() }));
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
 * if limbs are specified then the specified limbs are healed. * if the internal flag is NOT set and NO limbs are specified (default)
 *   then both overall health as well as the health of all limbs are healed.
 *
 * returns the actual amount of healing done or -1 if an error occurs
 */
varargs int eventHealDamage(int x, int internal, mixed limbs){
    if(!limbs && !internal){
        limbs = GetLimbs(); internal = 1;
    }
    else if(stringp(limbs)){
        limbs = ({ limbs });
    }
    if(!limbs){
        limbs = ({});
    }
    if( !arrayp(limbs)){
        error("Bad argument 3 to eventHealDamage().\n");
    }
    if(internal){
        AddHealthPoints(x);
    }
    map(limbs, (: AddHealthPoints($(x), $1) :));
    return x;
}

/* varargs int eventReceiveDamage(mixed agent, int type, int x,
 *     int internal, mixed limbs)
 * object agent - the thing responsible for this damage (required)
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
 * armor and natural resistence to this type of damage
 * the internal flag with no limbs specified means that the damage
 * is taken solely to the overall health
 * if the internal flag is set with limbs, the damage is both internal and
 * done to named limbs
 * if no internal flag is set and no limbs are specified, damage is done to
 * all limbs
 * if no internal flag is set and limbs are specified, then damage is done
 * only to the specified limbs
 * NOTE: internal damage is not modified by armor worn
 *
 * returns the average actual amount of damage done
 */

varargs int eventReceiveDamage(mixed agent, int type, int x, int internal,
        mixed limbs){
    string tmp = GetResistance(type);
    string agentname;
    int fp, basedam, tmpdam, dmgred;

    if(agent && stringp(agent)){
        agentname = agent;
        agent = 0;
    }

    if(objectp(agent)){
        if(estatep(agent) && !estatep(this_object())) return 0;
        if(!estatep(agent) && estatep(this_object())) return 0;
    }

    basedam = x;

    if(godmode) x = 0;

    switch(tmp){
        case "low": x = (3*x)/4; break;
        case "medium": x /= 2; break;
        case "high": x /= 4; break;
        case "immune": x = 0; break;
    }
    if(!limbs && !internal){
        limbs = ({ scramble_array(GetLimbs())[0] });
    }
    if( fp = functionp(Protect) ){
        if( !(fp & FP_OWNER_DESTED) ){
            function f;
            f = Protect;
            Protect = 0;
            x -= evaluate(f, this_object(), agent, type, x, limbs);
        }
    }
    x = eventCheckProtection(agent, type, x);
    if( internal ){
        AddHealthPoints(-x, 0, (agent || agentname));
        return x;
    }
    if( stringp(limbs) ){
        limbs = ({ limbs });
    }
    else if( !arrayp(limbs) ){
        return -1;
    }
    if(true()){
        int i, y, maxi;

        y = 0;
        for(i=0, maxi = sizeof(limbs); i < maxi; i++){
            object *obs;
            int j, z;

            z = x;
            if(!Limbs[limbs[i]]){          /* no limb */
                y += z;
                continue;
            }
            if(!(j = sizeof(obs = GetWorn(limbs[i])))){ /* no armor */
                y += z;                     /* add to total damage */
                if( !AddHealthPoints(-z, limbs[i], (agent || agentname)) ){
                    call_out("RemoveLimb",0,limbs[i], (agent || agentname));
                }
                AddStaminaPoints(-z/2);
            }
            else {
                while(j--){
                    dmgred = obs[j]->eventReceiveDamage(
                            (agent || agentname), type, z, 0, limbs[i]);
                    z -= dmgred;
                    if(z < 1){
                    }
                    else {
                    }
                }
            }
            y = z;
            if( y > 0 ){
                if(!AddHealthPoints(-y, 0, (agent || agentname))){
                    call_out("RemoveLimb",0,limbs[i], (agent || agentname));
                }
                AddStaminaPoints(-y/2);
            }
            return y;
        }
        return x;
    }
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
int eventCheckProtection(object agent, int type, int damage){
    int i, y;
    if( !i = sizeof(Protection) ) return damage;
    while(i--){
        int x;

        if( (type & Protection[i]->bits) != type ) continue;
        if( Protection[i]->absorb ){
            if( (x = (Protection[i]->absorb - damage)) < 1 ){
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
        if( y = functionp(Protection[i]->hit) ){
            if( y == FP_OWNER_DESTED ){
                RemoveMagicProtection(i);
                continue;
            }
            else x = evaluate(Protection[i]->hit, this_object(),
                    agent, x, Protection[i]);
        }
        damage -= x;
        if( damage < 1 ) return 0;
    }
    return damage;
}

mixed eventReceiveThrow(object who, object what){
    int x;

    if( what->GetClass() > 1 ){
        int mod = who->GetSkillLevel("projectile attack") +
            who->GetStatLevel("strength");

        x = what->eventStrike(this_object()) * 3;
        x = (x*mod)/100;
        if( what->GetWeaponType() != "projectile" ){
            x = x/4;
        }
        x = this_object()->eventReceiveDamage(who, what->GetDamageType(), x, 0, 
                GetRandomLimb("torso"));
        if( x > 0 ){
            who->AddSkillPoints("projectile attack", x);
        }
    }
    else {
        x = 0;
    }
    if( x < 1 ){
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

/* varargs int eventDie(mixed agent)
 * mixed agent - the agent responsible for the death (optional)
 *
 * description
 * Kills the owner of this body if not already dying
 *
 * returns true if the thing is dying
 */
varargs int eventDie(mixed agent){
    int x;
    string killer, death_annc;
    object crime_scene;

    if(DeathEvents) return 1;
    DeathEvents = 1;

    this_object()->SetDying(0);

    if(agent && stringp(agent)){
        killer = agent;
        agent = 0;
    }
    else {
        if(!agent || !objectp(agent)){
            killer = "UNKNOWN";
        }
        else killer = agent->GetName();
    }

    if(RACES_D->GetNonMeatRace(GetRace()))
        death_annc = killer + " has destroyed "+ this_object()->GetName()+".";
    else if(!this_object()->GetUndead())
        death_annc = killer + " has slain "+ this_object()->GetName()+".";
    else death_annc = killer + " has destroyed "+ this_object()->GetName()+".";

    if(killer == "asphyxia"){
        tell_room(ROOM_ARCH, this_object()->GetName() + " asphyxiated "+
                "in "+base_name(environment(this_object())));
    }

    CHAT_D->eventSendChannel("SYSTEM","death",death_annc,0);

    if( Sleeping > 0 ) Sleeping = 0;

    if( agent && objectp(agent) ){
        if( x ) agent->eventDestroyEnemy(this_object());
        else agent->eventKillEnemy(this_object());
    }
    crime_scene = environment();
    if( crime_scene ){
        object *obs;
        string *currs;
        object ob;
        string curr;
        int i;
        object *riders = filter( deep_inventory(), (: living($1) :) );

        //I'd like to move the living body out first, but for now this
        //misfeature stays.

        if(riders && sizeof(riders)){
            foreach(object rider in riders) eventBuck(rider);
        }
        if(GetRace() == "android" || GetRace() == "bot" ||
                inherits(LIB_VEHICLE,this_object())) ob = new(LIB_BOT_CORPSE);
        else if(member_array(GetRace(), RACES_D->GetNonMeatRaces()) != -1){
            ob = crime_scene;
            if(GetBodyComposition()){
                ob = new(LIB_CLAY);
                ob->SetComposition(GetBodyComposition());
            }
        }
        else {
            ob = new(LIB_CORPSE);
            ob->SetCorpse(this_object());
        }
        if(ob != crime_scene) ob->eventMove(crime_scene);
        obs = filter(all_inventory(), (: !($1->GetRetainOnDeath()) :));
        i = sizeof(obs);
        obs->eventMove(ob);
        currs = this_object()->GetCurrencies() || ({});
        foreach(curr in currs){
            object pile;
            int amt;

            if( amt = this_object()->GetCurrency(curr) ){
                pile = new(LIB_PILE);
                pile->SetPile(curr, amt);
                pile->eventMove(ob);
                this_object()->AddCurrency(curr, -amt);
            }
        }
    }

    SetUndead(!(x = GetUndead()));

    evaluate( function(){ Dying = 0; });
    flush_messages();
    return 1;
}

/* int eventRemoveItem(object ob)
 * object ob - the item being removed
 *
 * Removes a worn or wielded item from the list
 *
 * returns 1 on success, 0 on failure
 */
int eventRemoveItem(object ob){
    string limb;

    foreach(limb in keys(WornItems)){
        if( !WornItems[limb] ) continue;
        if( member_array(ob, WornItems[limb]) != -1){
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
 * marks the limbs "limbs" as being protected by the armor "ob"
 *
 * returns 1 if successful, 0 if failure
 */
int eventWear(object ob, mixed limbs){
    string limb,s1;
    string *target_limb;
    target_limb = ({});

    if( stringp(limbs) ){
        limbs = ({ limbs });
    }
    if( CanWear(ob, limbs) != 1 ){
        return 0;
    }
#if 0
    if(!inherits(LIB_WEAPON, ob)){
        foreach(limb in limbs){
            if(sscanf(limb,"%s foot",s1) || sscanf(limb,"%s hand",s1) ){
                if(!sizeof(GetWorn(limb))) target_limb = ({ limb });
            }
        }
    }
#endif
    if(sizeof(target_limb) && sizeof(limbs) == 1) limbs = target_limb;
    foreach(limb in limbs){
        if( !WornItems[limb] ) WornItems[limb] = ({ ob });
        else WornItems[limb] += ({ ob });
    }
    return 1;
}

string *GetEquippedLimbs(){
    string *equipped_limbs = ({});
    object *wornstuff = filter(all_inventory(this_object()), (: $1->GetWorn() :) );

    foreach(object item in wornstuff){
        equipped_limbs += item->GetWorn();
    }
    return equipped_limbs;
}
/************     /lib/body.c Data manipulation functions      *************/
    void NewBody(string race){
        if(!race)
            Limbs = ([ (Torso = "ooze") : ([ "parent" : 0, "children" : ({}),
                        "health" : 50, "class" : 1, "armors" : 0 ]) ]);
        else Limbs = ([]);
        MissingLimbs = ([]);
        Fingers = ([]);
    }

/* int CanWear(object armor, string *limbs)
 * object armor - the piece of armor being checked
 * string *limbs - the limbs on which the armor wants to be worn
 *    a nested array indicates that just one of the included limbs
 *    must be satisfied
 *
 * description
 * checks to see if a particular armor can be worn
 *
 * returns 1 if the armor can be worn on those limbs
 * returns 0 if the armor cannot be worn there for any reason
 *
 */
mixed CanWear(object ob, string *limbs){
    string limb, verb_pr, verb_pt, short;
    int type, bad_types, i, maxi;

    if( !ob ) return 0;
    short = ob->GetShort();
    if( !(type = ob->GetArmorType()) )
        return capitalize(short) + " cannot be worn!";
    if( type & A_WEAPON ){
        verb_pr = "wield";
        verb_pt = "wielded";
    }
    else {
        verb_pr = "wear";
        verb_pt = "worn";
    }
    if( !limbs || !(maxi = sizeof(limbs)) )
        return "Where should " + short + " be "+ verb_pt + "?";
    if( ob->GetWorn() )
        return "It is already being " + verb_pt + ".";

    // Verify that the the item can be worn on each limb specified by limbs.
    i = 0;
    foreach(limb in limbs){

        // Nested arrays indicate that only one of the elements in the nested
        // array must be satisfied. Check for one valid limb, and replace the
        // nested array with the valid limb, if found.
        if( arrayp(limb) ){
            string limb2;
            string validLimb = 0;
            int leastRings = -1;

            if(!sizeof(limb)) error("Bad limb specification to CanWear().\n");
            // Iterate through the nested array.
            foreach(limb2 in limb){
                string* wornItems;

                // The limb will be valid if:
                // o The body has the named limb capable of wearing the armor type.
                // o There are no armors of the same type on that limb, except
                //   for rings, which can have up to GetFingers() of that type worn.
                if(!Limbs[limb2] || !(Limbs[limb2]["armors"] & type)) continue;
                wornItems = WornItems[limb2];

                // If the item is a ring, attempt to distribute the rings evenly
                // across the available hands.
                if(type == A_RING){
                    int currentRings;

                    if(!sizeof(wornItems)) currentRings = 0;
                    else currentRings = sizeof(filter(wornItems,
                                (:$1->GetArmorType() == A_RING:)));
                    if(currentRings >= leastRings && leastRings != -1) continue;
                    leastRings = currentRings;
                    if(currentRings >= GetFingers(limb2)) continue;
                }
                else if(wornItems){
                    object wornItem;
                    int tmpType = 0;

                    foreach(wornItem in wornItems)
                        if(wornItem) tmpType |= wornItem->GetArmorType();
                    if(tmpType & type) continue;
                }
                validLimb = limb2;
            }
            if(validLimb) limbs[i] = validLimb;
            else return "You cannot " + verb_pr + " that.";
        }
        else {
            if(!GetLimb(limb)){
                string ret = "Try a different body part.";
                string *hands = GetWieldingLimbs();
                if(type & A_RING && sizeof(hands)){
                    ret = "Try: wear " + ob->GetKeyName() + " on "+
                        hands[0];
                }
                return ret;
            }
            globalint1 = Limbs[limb]["armors"];
            if( !Limbs[limb] ) return "You have no " + limb + ".";
            if( !(globalint1 & type) ){
                if( type & A_WEAPON )
                    return "You cannot wield with " + limb + ".";
                else return "You cannot wear " + short + " on your " + limb + ".";
            }
        }
        i++;
    }
    switch(type){
        case A_RING:
            if(maxi != 1)
                return "You can only wear " + short + " on one limb.";
            if( !WornItems[limbs[0]] ) return 1; /* nothing there, ring ok */
            /* count # worn rings */
            i = sizeof(filter(WornItems[limbs[0]],
                        (: $1->GetArmorType() == A_RING :)));
            if(i >= GetFingers(limbs[0]))
                return "You are already wearing too many rings there.";
            else return 1; /* ok */
        case A_GLOVE:
            if(maxi != 1)
                if( GetFingers(limbs[0]) > ob->GetFingers() ){
                    return capitalize(short) + " does not seem to fit well on "
                        "your " + limbs[0] + ".";
                }
            bad_types = A_GLOVE | A_LONG_GLOVE | A_SOCK | A_LONG_SOCK; 
            break;
        case A_LONG_GLOVE:
            if(maxi != 2)
                return capitalize(short) + " should be worn on two limbs.";
            if( limbs[0] == Limbs[limbs[1]]["parent"] ){ /* which is hand? */
                /* more fingers than this armor can stand */
                if(GetFingers(limbs[1]) > ob->GetFingers())
                    return capitalize(short) + " does not seem to fit well on "
                        "your " + limbs[1] + ".";
            }
            else if(limbs[1] == Limbs[limbs[0]]["parent"]){
                /* ok, first limb is hand, check it */
                if(GetFingers(limbs[0]) > ob->GetFingers())
                    return capitalize(short) + " does not seem to fit well on "
                        "your " + limbs[1] + ".";
            }
            else return "Your " + limbs[0] + " is not connected to your " +
                limbs[1] + ".";
            bad_types = A_GLOVE | A_LONG_GLOVE; 
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
        case A_HELMET: case A_VEST: case A_AMULET: case A_VISOR: case A_BELT: case A_COLLAR:
            if(maxi != 1)
                return capitalize(short) + " may only be worn on one limb.";
            bad_types = type;
            break;
        case A_PANTS: case A_SHIRT:
            bad_types = type;
            break;
        case A_CLOAK:
            bad_types = type;
            break;
        case A_SHIELD:
            bad_types = A_SHIELD | A_WEAPON;
            break;
#if 0
            foreach(limb in limbs){
                object worn_item;
                int tmp = 0;

                if( !WornItems[limb] ) continue; /* no armors, no prob */
                if( !Limbs[limb]["parent"] ) continue; /* torso ok for 2 */
                foreach(worn_item in WornItems[limb])  {
                    if( !worn_item ) continue;
                    tmp |= worn_item->GetArmorType();
                }
                /* not gonna allow 2 shields or a shield and weapon here */
                if( tmp & (A_SHIELD | A_WEAPON) )
                    return "You cannot wear " + short + " there right now.";
            }
            return 1; /* ok */
#endif
        case A_WEAPON:
            foreach(limb in limbs){
                object worn_item;
                int tmp = 0;

                if( !WornItems[limb] ) continue; /* nothing there, ok */
                foreach(worn_item in WornItems[limb]){
                    if( !worn_item ) continue;
                    tmp += worn_item->GetArmorType();
                }
                /* again, not allowing 2 weapons or a shield and weapon */
                if(tmp & (A_SHIELD | A_WEAPON))
                    return "You cannot wield " + short + " there right now.";
            }
            return 1; /* ok */
        case A_ARMOR: case A_BODY_ARMOR:
            bad_types = A_ARMOR | A_BODY_ARMOR;
            break;
        case A_COLLAR:
            bad_types = A_COLLAR | A_AMULET;
            break;
        case A_CUSTOM: 
            bad_types = A_CUSTOM;
            break;
        case A_EXO:
            bad_types = A_EXO;
            break;

        default: return 0; /* not any illegal stuff */
    }
    foreach(limb in limbs){
        object worn_item;
        int tmp = 0;
        if( !WornItems[limb] ) continue; /* no preventing types */
        foreach( worn_item in WornItems[limb] ){
            if( !worn_item ) continue;
            tmp |= worn_item->GetArmorType();
        }
        if(tmp & bad_types){
            return "You cannot " + verb_pr + " " + short + " there right now.";
        }
    }
    return 1; /* ok */
}

mixed CanManipulate(){
    string *prehensile_limbs = this_object()->GetWieldingLimbs();
    if(!sizeof(prehensile_limbs)){
        say(this_object()->GetName()+" looks helpless without prehensile appendages.");
        return "You lack prehensile limbs with which to do that.";
    }
    return 1;
}

int CanFly(){
    float j = percent(GetHealthPoints(), GetMaxHealthPoints());
    float k = percent(GetStaminaPoints(), GetMaxStaminaPoints());
    string clipped = identify(this_object()->GetMissingLimbs());

    if( j < COLLAPSE_AT  || k < COLLAPSE_AT ){
        return 0;
    }

    if(creatorp(this_object())) return 1;

    if(!RACES_D->CanFly(this_object()->GetRace())){
        return 0;
    }

    if(!clipped || !sizeof(clipped)) return 1;
    if(!grepp(lower_case(clipped),"wing")) return 1;

    if(this_object()->GetStaminaPoints() < 5) return 0;
    return 0;
}

int CanSwim(){
    float j = percent(GetHealthPoints(), GetMaxHealthPoints());
    float k = percent(GetStaminaPoints(), GetMaxStaminaPoints());

    if( j < COLLAPSE_AT  || k < COLLAPSE_AT ){
        return 0;
    }

    if(creatorp(this_object())) return 1;
    if(sizeof(this_object()->GetMissingLimbs())) return 0;

    if(!RACES_D->CanSwim(this_object()->GetRace())){
        return 0;
    }

    if(this_object()->GetStaminaPoints() < 5) return 0;

    return 1;
}


/* int AddLimb(string limb, string parent, int classes, int *armors)
 * string limb - the limb being added (required)
 * string parent - the limb to which this one is being attached (required)
 * int classes - rating of the limb's strength (optional)
 * int *armors - the types of armors which can be worn here (optional)
 *
 * defaults
 * classes defaults to 1
 * armors defaults to ({})
 *
 * description
 * adds the named limb to the body, attached at the named point
 * the limb classes starts at 1 for a torso (strongest)
 * to whatever the documentation rates as the weakest
 *
 * returns 1 on success, 0 on failure
 */
varargs int AddLimb(string limb, string parent, int classes, int *armors){
    int arm = 0;

    if(!limb || Limbs[limb] || (parent && !Limbs[parent])){
        return 0;
    }
    if(armors){
        int i;

        i = sizeof(armors);
        while(i--) arm |= armors[i];
    }
    if(MissingLimbs[limb]) map_delete(MissingLimbs, limb);
    if( parent ) Limbs[parent]["children"] += ({ limb });
    else Torso = limb;
    if( !classes ) classes = 5;
    Limbs[limb] = ([ "parent" : parent, "children" : ({}), "class" : classes,
            "armors" : arm ]);
    Limbs[limb]["health"] = GetMaxHealthPoints(limb);
    return 1;
}

int HealLimb(string limb){
    Limbs[limb]["health"] = GetMaxHealthPoints(limb);
    return Limbs[limb]["health"];
}

// Restore Limb has been bugfixed and modified to 
// handle missing parents and missing children. To restore
// a limb, RestoreLimb("right arm") will restore the arm only.
// To restore the arm plus its children (in this case, a
// hand) use RestoreLimb("right arm",1) and this will enable
// the recursive restore of the limb. Trying to restore 
// a limb will fail if the parent is missing.

varargs int RestoreLimb(string limb, int recurse){
    if( !MissingLimbs[limb] ) return 0;
    if(!sizeof(Limbs[MissingLimbs[limb]["parent"]])) return 0;  
    Limbs[limb] = MissingLimbs[limb];
    map_delete(MissingLimbs, limb);
    Limbs[limb]["health"] = GetMaxHealthPoints(limb);

    // This ensures that the parent of the current limb has this
    // limb added to its children array.
    if(member_array(limb,Limbs[Limbs[limb]["parent"]]["children"]) == -1){
        Limbs[Limbs[limb]["parent"]]["children"] += ({ limb });
    }

    if(recurse && sizeof(MissingLimbs)){
        string *kinder = ({});
        foreach(string key, mixed val in MissingLimbs){
            if(MissingLimbs[key]["parent"] == limb) kinder += ({ key });
        }
        if(sizeof(kinder)){
            foreach(string element in kinder) this_object()->RestoreLimb(element, 1);
        }
    }

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
int DestLimb(string limb){
    string *kiddies;
    int i;

    if(!limb || !Limbs[limb]) return -1;
    if(!Limbs[limb]["parent"]){
        return -1;
    }
    Limbs[Limbs[limb]["parent"]]["children"] -= ({ limb });
    if( (i = sizeof(kiddies = Limbs[limb]["children"])) )
        while(i--) DestLimb(kiddies[i]);
    map_delete(Limbs, limb);
    return 1;
}

/* int this_object()->RemoveLimb(string limb, object agent)
 * string limb - the limb being removed
 * object agent - the agent who is responsible for the limb removal
 *
 * description
 * removes a limb from the limbs mapping and stores vital data in the
 * missing limbs mapping
 *
 * returns -1 on error, 0 on failure, 1 on success
 */
varargs int RemoveLimb(string limb, mixed agent, int quiet){
    string *kiddies;
    string limbname,adjname,templimbname, agentname;
    int i, fatal;

    if(agent){
        if(stringp(agent)){
            agentname = agent;
            agent = 0;
        }
        else if(objectp(agent)){
            if(agent != this_object()) agentname = agent->GetName();
        }
    }

    if(limb == this_object()->GetTorso() || limb == "neck") return 0;
    if(godmode || !SEVERABLE_LIMBS) return 0;
    if( sscanf(limb, "%s %s", adjname, templimbname) == 2 ) limbname=templimbname;
    else limbname=limb;

    if(!limb || !Limbs[limb]) return -1;
    if(!Limbs[limb]["parent"] || Limbs[limb]["class"] == 1) fatal = 1;

    MissingLimbs[limb] = copy(Limbs[limb]);
    Limbs[Limbs[limb]["parent"]]["children"] -= ({ limb });
    if( (i = sizeof(kiddies = Limbs[limb]["children"])) )
        while(i--) this_object()->RemoveLimb(kiddies[i], agent,quiet);
    map_delete(Limbs, limb);
    if( environment() ){
        object ob;
        if(!quiet){
            message("environment", possessive_noun(GetName()) + " " + limb +
                    " is severed!", environment(), ({ this_object() }));
            message("environment", "Your "+ limb + " is severed!", this_object());
        }
        if(GetRace() == "golem"){
            ob = new(LIB_CLAY);
            if(GetBodyComposition()) ob->SetComposition(GetBodyComposition());
        }
        else {
            if(GetRace() == "android" || GetRace() == "bot" ||
                    inherits(LIB_VEHICLE,this_object())) ob = new(LIB_BOT_LIMB);
            else ob = new(LIB_LIMB);
            ob->SetLimb(limb, GetCapName(), GetRace());
        }
        ob->eventMove(environment());
        i = sizeof(WornItems[limb]);
        while(i--){
            int number_of_slots = sizeof(WornItems[limb][i]->GetWorn());
            WornItems[limb][i]->SetWorn(0);
            if(number_of_slots == 1) WornItems[limb][i]->eventMove(ob);
            else WornItems[limb][i]->eventMove(this_object());
        }
        while( i = sizeof(WornItems[limb]) )
            eventRemoveItem(WornItems[limb][i]);
    }
    if(fatal){
        mixed smith;
        if( !this_object()->GetDying() ){
            this_object()->SetDying(1);
            Agent = agent;
            smith = (Agent||agentname);
            call_out("eventDie", 0, smith);
        }
    }
    return 1;
}

mapping GetLimb(string limb){
    if(!limb || !Limbs[limb]) return 0;
    else return copy(Limbs[limb]);
}

/* string GetRandomLimb(string targ)
 * string targ - the targetted limb
 *
 * description
 * returns a random limb weighted towards the targetted limb
 */

string GetRandomLimb(string targ){
    string array limbs;

    if( !targ ){
        targ = GetTorso();
    }
    if( member_array(targ, (limbs=keys(Limbs))) == -1){
        targ= GetTorso(); /* no target or illegal target, weight torso */
    }
    targ = (limbs + (targ ? ({ targ, targ }) : ({})))[random(sizeof(limbs)+2)];
    return targ;
}

string GetTorso(){
    string *limbs;
    int i;

    i = sizeof(limbs = keys(Limbs));
    while(i--){
        if(!Limbs[limbs[i]]["parent"]){
            return limbs[i];
        }
    }
    return 0;
}

string array GetLimbs(){
    return (Limbs ? keys(Limbs) : 0);
}

int GetLimbClass(string limb){
    if(sizeof(Limbs) && sizeof(Limbs[limb])) return Limbs[limb]["class"]; 
    return 5;
}

string GetLimbParent(string limb){ return Limbs[limb]["parent"]; }


//The following function courtesy of Garfield @ M*U*D
string GetMissingLimbParent(string limb){ return MissingLimbs[limb]["parent"]; } 

//The following function courtesy of Garfield @ M*U*D
string *GetMissingLimbParents(string limb){
    string *limbs;

    limbs = ({ limb });

    while(memberp(keys(MissingLimbs),GetMissingLimbParent(limbs[0]))){
        limbs = ({ GetMissingLimbParent(limbs[0]) }) + limbs;
    }

    return limbs;
} 

string array GetLimbChildren(string limb){
    return Limbs[limb]["children"] + ({});
}

mapping GetMissingLimb(string limb){
    return (limb ? copy(MissingLimbs[limb]) : 0);
}

// This function courtesy of Garfield 
// and Javelin at M*U*D
int eventCompareLimbs(string limb1, string limb2){
    if (memberp(GetMissingLimbParents(limb1), limb2)){
        return 1;
    }
    if (memberp(GetMissingLimbParents(limb2), limb1)){
        return -1;
    }
    return strcmp(limb1, limb2);    } 

    // New comparison functionality courtesy of
    // Garfield and Javelin at M*U*D
    varargs string array GetMissingLimbs(int not_default){
        if(not_default){
            string *tmp_arr = ({});
            if(sizeof(keys(MissingLimbs))){ 
                tmp_arr = sort_array(keys(MissingLimbs), (: eventCompareLimbs :) );
            }
            return tmp_arr;
        }
        else return keys(MissingLimbs);
    }

string GetLong(string nom){
    string *limbs;
    string *exempt;
    string str;
    float h;

    str = "";
    exempt = ({"bot","android","tree","plant","vehicle","strider","mech"});

    if(!(this_object()->GetNoCondition())){
        if(member_array(this_object()->GetRace(),exempt) == -1 &&
                !this_object()->GetUndead() ){
            str = "The "+this_object()->GetGender()+" ";
            str += this_object()->GetRace();
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
        }
    }
    if(this_object()->GetUndead()){
        str = capitalize(nominative(this_object()))+" has been killed, and ";
        str +=  "is one of the Walking Undead.\n";
    }

    limbs = GetMissingLimbs();
    if(!(GetRace() == "android") && !(GetRace() == "bot") &&
            !inherits(LIB_VEHICLE,this_object())){ 

        if( sizeof(limbs) ){
            int i, maxi;

            str += capitalize(nom) + " is missing " + add_article(limbs[0]); 
            for(i=1, maxi = sizeof(limbs); i<maxi; i++){
                if( i < maxi-1 ) str += ", " + add_article(limbs[i]);
                else {
                    if( maxi > 2 ) str += ",";
                    str += " and " + add_article(limbs[i]);
                }
            }
            str += ".\n";
        }
    }
    return str;
}

string array GetWieldingLimbs(){
    return filter(keys(Limbs), (: (Limbs[$1]["armors"] & A_WEAPON) :));
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

varargs int AddFingers(string limb, int x){
    if((Fingers[limb] += x) < 1) Fingers[limb] = 1;
    return Fingers[limb];
}

int GetFingers(string limb){
    return Fingers[limb];
}

varargs object *GetWorn(string limb){
    if(!limb){
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

varargs mixed GetWielded(string limb){
    if(!limb){
        object *ret = ({});
        string *limbs;
        int i;

        i = sizeof(limbs = keys(Limbs));
        while(i--){
            if(!WornItems[limbs[i]]) continue;
            else ret += filter(WornItems[limbs[i]],
                    (: $1->GetArmorType() == A_WEAPON :));
        }
        return distinct_array(ret);
    }
    else if(!WornItems[limb]) return 0;
    else {
        object *ret;

        ret = filter(WornItems[limb], (: $1->GetArmorType() == A_WEAPON :));
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

varargs static int AddHealthPoints(int x, string limb, mixed agent){
    int y = 0;
    string agentname;

    if(agent){
        if(stringp(agent)){
            agentname = agent;
            agent = 0;
        }
        else if(objectp(agent)){
            if(agent != this_object()) agentname = agent->GetName();
        }
    }

    if(godmode && x < 1) return 0;

    if( limb ){
        if( !Limbs[limb] ) return -1;
        y = GetMaxHealthPoints(limb);
        if(y < 1) return y;
        if((Limbs[limb]["health"] += x) < 1) Limbs[limb]["health"] = 0;
        else if(Limbs[limb]["health"] > y)
            Limbs[limb]["health"] = y;
        return Limbs[limb]["health"];
    }
    else {
        if((HealthPoints += x) < 1) HealthPoints = 0;
        else if(HealthPoints > (y = GetMaxHealthPoints())) HealthPoints = y;
        if( HealthPoints < 1 ){
            if( !this_object()->GetDying()){
                mixed smith;
                this_object()->SetDying(1);
                Agent = agent;
                smith = (Agent||agentname);
                call_out("eventDie", 0, smith);
            }
        }
        else {
            float h = percent(GetHealthPoints(), GetMaxHealthPoints());

            if( h < COLLAPSE_AT ){
                eventCollapse();
            }
        }
        return HealthPoints;
    }
}

varargs int GetHealthPoints(string limb){
    if(limb){
        if(!Limbs[limb]) return -1;
        else return Limbs[limb]["health"];
    }
    else return HealthPoints;
}

int SetHealthPoints(int x){
    if(interactive(this_object())) return GetHealthPoints();
    if( x > GetMaxHealthPoints() ){
        this_object()->SetStat("durability", (x-50)/10, 
                this_object()->GetStatClass("durability"));
        AddHealthPoints( x - GetHealthPoints() );
    }
    HealthPoints = x;
    return GetHealthPoints();
}

/* int AddMagicPoints(int x)
 * int x - the number of magic points being added, may be negative
 *
 * description
 * adds magic points to the body
 *
 * returns the remaining magic points
 */

int AddMagicPoints(int x){
    int y;

    if((MagicPoints += x) < 1) MagicPoints = 0;
    else if(MagicPoints > (y = GetMaxMagicPoints())) MagicPoints = y;
    return MagicPoints;
}

int GetMagicPoints(){ return MagicPoints; }

int GetMaxMagicPoints(){ return 0; }

/* int AddStaminaPoints(int x)
 * int x - number of stamina points being added
 *
 * description
 * adds "x" stamina points, can be negative
 *
 * returns the remaining number of stamina points
 */

int AddLead(string ammo,int number){
    if( !intp(number) ) error("Bad argument 2 to AddLead().\n");
    if( !stringp(ammo) ) error("Bad argument 1 to AddLead().\n");
    firearms_wounds += number;
    if( firearms_wounds < 0 ){
        firearms_wounds = 0;
    }
    return 1;
}

varargs int GetLead(string ammo){
    return firearms_wounds;
}

float AddStaminaPoints(mixed x){
    float y;

    if( !intp(x) && !floatp(x) )
        error("Bad argument 1 to AddStaminaPoints().\n");
    if( intp(x) ) x = to_float(x);
    if((StaminaPoints += x) < 0.1) StaminaPoints = 0.0;
    else if(StaminaPoints > (y = GetMaxStaminaPoints())) StaminaPoints = y;
    return StaminaPoints;
}

int GetStaminaPoints(){ return to_int(StaminaPoints); }

float GetMaxStaminaPoints(){  return 0; }

int AddExperiencePoints(mixed x){
    if( !intp(x)) error("Bad argument 1 to AddExperiencePoints().\n");
    x -= DEVIATION_D->GetDeviationCost(this_object(), x);
    
    if( ExperienceDebt > 0 ) {
        if( (ExperienceDebt -= x) < 0 ) {
            ExperiencePoints -= ExperienceDebt;
            ExperienceDebt = 0;
            write("The last of your experience debt has been paid.\n");
        }
    } else {
        ExperiencePoints += x;
    }

    return ExperiencePoints;
}

int GetExperiencePoints(){ return ExperiencePoints; }

int AddExperienceDebt(mixed x) {
    if( !intp(x) ) error("Bad argument 1 to AddExperienceDebt().\n");
    if( x < 0 ) {
        if( (x = ExperienceDebt + x) < 0) {
            ExperienceDebt = 0;
            AddExperiencePoints(ExperienceDebt);
            write("The last of your experience debt has been paid.\n");
        }
    } else {
        ExperienceDebt += x;
    }

    return ExperienceDebt;
}

int GetExperienceDebt() { return ExperienceDebt; }

int AddMagicProtection(class MagicProtection cl){
    if( ( !cl->absorb && !(cl->protect && cl->time) ) ||
            ( cl->hit && !functionp(cl->hit) ) ||
            ( cl->end && !functionp(cl->end) ) ||
            ( !cl->bits )
      ){
        error("Illegal class setting passed to AddMagicProtection.\n");
        return 0;
    }
    cl->timestamp = time();
    Protection += ({ cl });
    return 1;
}

class MagicProtection array GetMagicProtection(){ return Protection; }

int RemoveMagicProtection(int i){
    if( i > sizeof(Protection) - 1 ) return 0;
    if( Protection[i]->end ){
        if( !(functionp(Protection[i]->end) & FP_OWNER_DESTED) ){
            evaluate(Protection[i]->end, this_object());
        }
    }
    Protection -= ({ Protection[i] });
    return 1;
}

// This is for creatures that do not use weapons.
// I had to crank down unarmed combat for humanoids,
// so if your npc's are, say, dumb animals, be sure
// to include "SetMelee(1)" so they can fight
// Note that fighters are an exception to this.
//
int SetMelee(int i){ melee = i; return melee; }

int GetMelee(){ return melee; }


int GetDying(){
    return Dying; 
}

int SetDying(int x){
    if(x) Dying = 1;
    else Dying = 0;
    return Dying;
}

int SetSleeping(int x){
    if(RACES_D->GetNonMeatRace(GetRace()) || GetRace() == "elf") return 0;
    return (Sleeping = x);
}

int GetSleeping(){ return Sleeping; }

int AddAlcohol(int x){ 
    int i = Alcohol + x;
    if(RACES_D->GetNonMeatRace(GetRace())) return 0; 
    if(i < 0) i = 0;
    return (Alcohol = i);
}

int GetAlcohol(){ return Alcohol; }

int AddCaffeine(int x){ 
    if(RACES_D->GetNonMeatRace(GetRace())) return 0;
    return (Caffeine += x);
}

int GetCaffeine(){ return Caffeine; }

int AddDrink(int x){ return (Drink += x); }

int GetDrink(){ return Drink; }

int AddFood(int x){ return (Food += x); }

int GetFood(){ return Food; }

int AddPoison(int x){
    Poison += x;
    if( Poison < 1 ){
        Poison = 0;
    }
    return Poison;
}

int GetPoison(){
    int caff = GetCaffeine();
    int alc = GetAlcohol();
    int bonus_ret;
    if(alc > 100) bonus_ret += (alc - 100);
    if(caff > 100) bonus_ret += (caff - 100);

    return Poison + bonus_ret; 
}

string GetResistance(int type){ return "none"; }

string GetRace(){ return 0; }

string GetName(){ return 0; }

string GetCapName(){ return 0; }

int GetHeartRate(){
    int rate;

    rate = (GetAlcohol() - GetCaffeine());
    if( rate > 50 ) rate = 6;
    else if( rate > 25 ) rate = 5;
    else if( rate > 0 ) rate = 4;
    else if( rate > -25 ) rate = 3;
    else rate = 2;
    rate += HeartModifier;
    if( rate < 1 ){
        rate = 1;
    }
    else if( rate > 10 ){
        rate = 10;
    }
    return rate;
}

int GetHealRate(){
    int heal, mod = 1;
    int lead = GetLead();
    object dude = this_object();

    heal = 1 - (GetPoison() / 2);
    heal += ( (GetDrink() + GetFood()) || 1 ) / 40;
    if(GetSleeping()) mod++;
    if(GetAlcohol() > 10) mod++;
    if(dude->GetStatLevel("strength") > 50) mod++;
    if(dude->GetStatLevel("durability") > 50) mod++;
    if(dude->GetStatLevel("luck") > random(100)) mod++;
    if(dude->GetSkillLevel("faith") > 10) mod++;
    mod =  heal * to_float(mod * 0.1);
    heal += mod;
    if(lead && interactive(dude)){
        if(lead < 5) heal /= (lead + 1);
        else heal = 0;
    }
    return (heal < 0 ? 0 : heal);
}

string GetHealthShort(){
    string cl, sh;
    float h;

    if( !(sh = this_object()->GetShort()) ) return 0;
    h = percent(GetHealthPoints(), GetMaxHealthPoints());
    if( this_object()->GetNoCondition() || h > 90.0 ) cl = "%^BOLD%^GREEN%^";
    else if( h > 75.0 ) cl = "%^GREEN%^";
    else if( h > 50.0 ) cl = "%^BOLD%^BLUE%^";
    else if( h > 35.0 ) cl = "%^BLUE%^";
    else if( h > 20.0 ) cl = "%^BOLD%^RED%^";
    else cl = "%^RED%^";
    return cl + capitalize(sh);
}

mixed SetProtect(function f){ return (Protect = f); }

function GetProtect(){ return Protect; }

int GetHeartModifier(){
    return HeartModifier;
}

varargs int AddHeartModifier(int x, int t){
    HeartModifier += x;
    if( t > 0 ){
        call_out((: AddHeartModifier(-$(x)) :), t);
    }
    return HeartModifier;
}

int AddHP(int hp){
    if(hp < 1 && godmode) return 0;
    AddHealthPoints(hp);
    return hp;
}

string GetAffectLong(){
    object dude;
    string ret;
    int alclevel;

    dude = this_object();
    alclevel = dude->GetAlcohol();
    ret = "";

    if(dude->GetSleeping() > 0){
        ret += dude->GetName()+" is asleep.\n";
    }

    else if(alclevel > 10){
        if(alclevel < 20) ret += dude->GetName()+" looks tipsy.\n";
        else if(alclevel < 50) ret += dude->GetName()+" looks drunk.\n";
        else if(alclevel < 70) ret += dude->GetName()+" is very drunk.\n";
        else ret += dude->GetName()+" is completely wasted drunk.\n";
    }

    return ret;

}

int GetDeathEvents(){
    return DeathEvents;
}

int SetDeathEvents(int i){
    if(!i) DeathEvents = 0;
    else DeathEvents = 1;
    return DeathEvents;
}

int SetGodMode(int i){
    if(!this_player()) return 0;
    if(archp(this_object()) && !archp(this_player())) return 0;
    if(!archp(this_object()) && this_player() != this_object()) return 0;
    if(i) godmode = 1;
    else godmode = 0;
    return godmode;
}

int GetGodMode(){
    return godmode;
}

string SetKeepalive(string str){
    keepalive = str;
}

string GetKeepalive(){
    return keepalive;
}
