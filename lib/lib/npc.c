/*    /lib/npc.c
 *    from the Dead Souls LPC Library
 *    the standard non-player character object
 *    created by Descartes of Borg 950323
 *    Version: @(#) npc.c 1.11@(#)
 *    Last Modified: 96/12/21
 */

#include <lib.h>
#include ROOMS_H
#include <daemons.h>
#include <position.h>
#include <armor_types.h>
#include <message_class.h>
#include <vision.h>
#include "include/npc.h"

#ifndef CATCH_TELL_ROOM
#define CATCH_TELL_ROOM "/domains/default/room/catchtell"
#endif

inherit LIB_DESCRIBE;
inherit LIB_CHAT;
inherit LIB_COMMAND;
inherit LIB_CONTAINER;
inherit LIB_LIVING;
inherit LIB_MESSAGES;
inherit LIB_MOVE;
inherit LIB_OBJECT;
inherit LIB_DOMESTICATE;
inherit LIB_GUARD;
inherit LIB_UNIQUENESS;
inherit LIB_SHADOW_HOOK;

private int CustomXP, ActionChance, CombatActionChance, AutoStand;
private mixed Encounter;
private string *EnemyNames;
private static int NPCLevel, Unique;
private static mixed Die, Action, CombatAction;
private static mapping Inventory;
private static string MountStyle = "ridden";
private int VisibleRiders = 1;
private int actions_enabled = 1;
mapping Equipped = ([]);

int eventExtraAction(){ return 1; }

static void create(){
    SetSaveRecurse(1);
    chat::create();
    command::create();
    living::create();
    messages::create();
    object::create();
    Setup();
    SetPK(1);
    SetOpacity(0);
    EnemyNames = ({});
    Encounter = 0;
    ActionChance = 0;
    Unique = 0;
    CustomXP = 0;
    Inventory = ([]);
    AutoStand = 1;
    set_heart_beat( GetHeartRate() );
}

void CheckEncounter(){
    string *enemies;
    object env = environment(this_object());
    object *dudes;

    if(!env) return;

    dudes = singular_array(get_livings(env) + ({ this_player() }));

    if( !query_heart_beat() ){
        eventCheckHealing();
        set_heart_beat( GetHeartRate() );
    }

    foreach(object dude in dudes){
        object denv;
        if(dude) denv = environment(dude);
        if(!denv || !dude || denv != env || dude->GetInvis()) continue;
        if( sizeof(enemies = GetEnemyNames()) ){
            if( member_array(dude->GetKeyName(),enemies) != -1 ){
                eventExecuteAttack(dude);
                continue;
            }
        }
        if( Encounter ){
            int x = 0;

            if( functionp(Encounter) ){
                x = evaluate(Encounter, dude);
            }
            else if( arrayp(Encounter) ){	    
                if( member_array(dude->GetKeyName(), Encounter) > -1 ){
                    x = 1;
                }
                else {
                    x = 1;
                }
            }
            else if( dude->GetStatLevel("charisma") < Encounter ){
                x = 1;
            }
            if( x ){
                SetAttack(dude);
            }
        }
    }
}

static void init(){
    guard::init();
    CheckEncounter();
}

static void heart_beat(){
    int position;
    living::heart_beat();
    guard::heart_beat();
    if( !ContinueHeart() ){
        set_heart_beat(0);
        return;
    }
    eventExtraAction();
    position = GetPosition();
    if( position == POSITION_LYING || position == POSITION_SITTING ){
        if(AutoStand && 
                !RACES_D->GetLimblessRace(this_object()->GetRace())) 
            eventForce("stand up");
        if(GetInCombat() && 
                !RACES_D->GetLimblessRace(this_object()->GetRace()) ) 
            eventForce("stand up");
    }
    if( !GetInCombat() && actions_enabled && ActionChance > random(100) ){
        int x;

        if( functionp(Action) ) evaluate(Action);
        else if( pointerp(Action) && (x = sizeof(Action)) ){
            mixed act;

            act = Action[random(x)];
            if(functionp(act)){
                evaluate(act);
                return;
            }
            if( act && act != "" && act[0] == '!' && act != "!" ){
                act = act[1..];
                eventForce(act);
            }
            else message("other_action", act, environment());
        }
    }
    else if( GetInCombat() && CombatActionChance > random(100) ){
        int x;

        if( functionp(CombatAction) ){
            evaluate(CombatAction);
        }
        else if( pointerp(CombatAction) && (x = sizeof(CombatAction)) ){
            mixed mact;
            string act;

            x--;
            mact = CombatAction[random(x)];
            if( functionp(mact) ){

                evaluate(mact);
                return;
            }
            if( !stringp(mact) ) mact = "emote looks confused.";
            else act = mact;
            if( act && act != "" ){
                eventForce(act);
            }
            else message("other_action", act, environment());
        }
    }
}

static int Destruct(){
    if( GetParty() ) PARTY_D->eventLeaveParty(this_object());
    living::Destruct();
    return object::Destruct();
}

void eventReconnect(){ }

/* ***************  /lib/npc.c command functions  ************** */
static int cmdAll(string arg){
    object env;
    string verb;

    verb = query_verb();
    env = environment();
    if( chat::chat_command(verb + " " + arg) == "" ) return 1;
    return command::cmdAll(arg);
}

/* ***************  /lib/npc.c events  *************** */


int eventCompleteMove(mixed dest){
    mixed val;
    string file;
    int x;

    if( environment() ) return move::eventMove(dest);
    else x = move::eventMove(dest);
    if( !x ) return x;
    foreach(file, val in Inventory){
        object ob = previous_object();
        if(ob == dest && dest->GetPersistent()) break;

        if( intp(val) ){
            if( val < 0 ){
                ob = unique(file, -val);
                if( ob ) ob->eventMove(this_object());
            }
            else while(val--)
                if( ob = new(file) ) ob->eventMove(this_object());
        }
        else if( stringp(val) )  {
            if( !(ob = new(file)) ) continue;
            if( ob->eventMove(this_object()) ) eventForce(val);
        }
        else if( functionp(val) ){
            if( !(ob = new(file)) ) continue;
            if( ob->eventMove(this_object()) ) evaluate(val);
        }
    }
    return x;
}

int eventDestruct(){
    mixed array worn = ({});
    if(!valid_event(previous_object(), this_object())) return 0;
    if(room_environment() && room_environment()->GetPersistent()){
        if(!Equipped) Equipped = ([]);
        worn = this_object()->GetWorn();
        worn += this_object()->GetWielded();
        foreach(mixed thing in worn){
            if(arrayp(thing) && sizeof(thing)) thing = thing[0];
            if(!thing || !objectp(thing)) continue;
            if(Equipped[file_name(thing)]) continue;
            Equipped[file_name(thing)] =
                ([ "object" : thing, "where" : thing->GetWorn() ]);
        }
    }
    chat::eventDestruct();
    object::eventDestruct();
}

varargs int eventDie(mixed agent){
    int x;
    string death_verb = "dies";
    string death_action = "kill";
    string death_descriptor = "dead";
    object env = (environment() || load_object(ROOM_POD));

    if(RACES_D->GetNonMeatRace(GetRace())){
        death_verb = "breaks down completely";
        death_action = "destroy";
        death_descriptor = "broken";
    }

    if(this_object()->GetDead() || this_object()->GetDeathEvents()) return 0;

    if( (x = living::eventDie(agent)) != 1 ) return x;
    if( stringp(Die) )  {
        message("other_action", Die, env, ({ this_object() }));
        if( agent) message("my_action", "You "+death_action+" " + GetName() + ".", agent);
    }
    else if( functionp(Die) && !evaluate(Die, agent) ) return 0;
    else {
        if(GetPosition() == POSITION_STANDING) message("other_action", "%^BOLD%^%^RED%^"+ GetName() + " drops "+death_descriptor+".", env, ({ this_object() }) );
        else if(GetPosition() == POSITION_FLYING) message("other_action", "%^BOLD%^%^RED%^"+ GetName() + " falls "+death_descriptor+".", env, ({ this_object() }) );
        else message("other_action", "%^BOLD%^%^RED%^"+ GetName() + " "+death_verb+".", env, ({ this_object() }) );
        if( agent )
            message("my_action", "You "+death_action+" " + GetName() + ".",
                    (objectp(agent) ? agent : load_object(CATCH_TELL_ROOM)) );
    }
    set_heart_beat(0);
    call_out( (: Destruct :), 0);
    flush_messages();
    return 1;
}

mixed eventTurn(object who){
    if( !living::eventTurn(who) ){
        return 0;
    }
    all_inventory()->eventDestruct();
    call_out((: Destruct :), 0);
    return 1;
}

void eventEnemyDied(object ob){
    living::eventEnemyDied(ob);
    EnemyNames -= ({ ob->GetKeyName() });
}

int eventMove(mixed dest){
    int ret;
    ret = eventCompleteMove(dest);
    guard::CheckPending();
    return ret;
}

varargs int eventPrint(string msg, mixed arg2, mixed arg3){
    object *riders = filter(deep_inventory(), (: $1->GeteventPrints() :) );
    object *targs = ({});
    if(NPC_CATCH_TELL_DEBUG){
        tell_room(ROOM_CATCH_TELL,"-------");
        tell_room(ROOM_CATCH_TELL,timestamp());
        tell_room(ROOM_CATCH_TELL,"obj: "+identify(this_object()));
        tell_room(ROOM_CATCH_TELL,"msg: "+msg);
        tell_room(ROOM_CATCH_TELL,"arg2: "+identify(arg2));
        tell_room(ROOM_CATCH_TELL,"arg3: "+identify(arg3));
        tell_room(ROOM_CATCH_TELL,"stack: "+get_stack());
        tell_room(ROOM_CATCH_TELL,"previous: "+identify(previous_object(-1)));
        tell_room(ROOM_CATCH_TELL,"-------");
    }
    if(riders && sizeof(riders)){
        int i1, rider_source;
        if(!arg2) arg2 = 0;
        if(!arg3) arg3 = 0;
        if(sizeof(riders)){
            if(arg2 && intp(arg2)){
                object *tmp_riders = riders;
                if(arg2 & MSG_CONV || arg2 & MSG_ENV){
                    foreach(object ob in previous_object(-1)){
                        if(member_array(ob,riders) != -1){
                            tmp_riders -= ({ ob });
                            rider_source = 1;
                        }
                    }
                }
                if((arg2 & MSG_CONV))  true();
                else {
                    if(objectp(arg2)) targs = tmp_riders - ({ arg2 });
                    else if(arrayp(arg2)) targs =  tmp_riders - arg2;
                    else targs = tmp_riders;
                    targs->eventPrint(msg, arg3);
                }
            }
            i1 = sizeof(previous_object(-1)) -1;
            if(i1 < 0) i1 = 0;
            if(sizeof(previous_object(-1)) &&
                    (member_array(previous_object(),riders) != -1 ||
                     member_array(previous_object(-1)[i1],riders) != -1) &&
                    (!intp(arg2) || (!(arg2 & MSG_CONV) && !(arg2 & MSG_ENV))) && 
                    member_array(this_object(),previous_object(-1)) == -1){ 
                if(objectp(arg2)) targs = riders - ({ arg2 });
                else if(arrayp(arg2)) targs = riders - arg2;
                else targs = riders;
                if(VisibleRiders) environment()->eventPrint(msg, arg2, arg3);
                else targs->eventPrint(msg, arg2);
            }
        }
    }  
    return 1;
}

int eventReceiveObject(object who){
    object ob;

    ob = previous_object();
    if( !ob || !container::eventReceiveObject() ) return 0;
    AddCarriedMass(ob->GetMass());
    if(environment()) environment()->AddCarriedMass(ob->GetMass());
    return 1;
}

int eventReleaseObject(object who){
    object ob;

    ob = previous_object();
    if( !ob || !container::eventReleaseObject() ) return 0;
    if( ob->GetMass() ){
        AddCarriedMass( -(ob->GetMass()) );
        if(environment()) environment()->AddCarriedMass(-(ob->GetMass()));
    }
    return 1;
}

varargs int eventShow(object who, string str){
    if( !living::eventShow(who, str) ) return 0;
    eventPrint(this_player()->GetName() + " looks you over.");
    return 1;
}

/*  ***************  /lib/npc.c modal functions  *************** */

int CanCarry(int amount){ return living::CanCarry(amount); }

int CanReceive(object ob){ return CanCarry(ob->GetMass()); }

/*  ***************  /lib/npc.c lfuns  ***************  */

static int ContinueHeart(){
    object env;

    if( !(env = environment()) ) return 0;
    if( !sizeof(filter(all_inventory(env), (: living :))) ) return 0;
    return 1;
}

/*  ***************  /lib/npc.c data functions  ***************  */

mapping SetInventory(mapping mp ){ return (Inventory = mp); }
mapping GetInventory(){ return copy(Inventory); }

varargs string SetRace(string race, mixed extra){
    race = living::SetRace(race, extra);
    eventCompleteHeal(GetMaxHealthPoints());
    return race;
}

string SetClass(string cls){
    string *skills;
    int x, i;

    cls = living::SetClass(cls);
    x = NPCLevel;
    i = sizeof(skills = GetSkills());
    while(i--){
        int y;

        y = (GetSkillClass(skills[i]) || 5);
        SetSkill(skills[i], (3*x)/y, y);
    }
    return cls;
}

int SetLevel(int x){
    string *tmp;
    int i;

    if(!NPCLevel){
        NPCLevel = x;
        i = sizeof(tmp = GetSkills());
        while(i--){
            int y;

            y = (GetSkillClass(tmp[i]) || 5);
            SetSkill(tmp[i], (3*x)/y, y);
        }
        i = sizeof(tmp = GetStats());
        while(i--){
            int y;

            y = (GetStatClass(tmp[i]) || 5);
            SetStat(tmp[i], ((5-y)*10) + (3*x)/y, y);
        }
        eventCompleteHeal(GetMaxHealthPoints());
    }
    else NPCLevel = x;
    return NPCLevel;
}

int GetLevel(){ return NPCLevel; }

int SetCustomXP(int i){
    if(!i) i = 0;
    CustomXP = i;
    return CustomXP;
}

int GetCustomXP(){
    if(!strsrch(base_name(this_object()), ESTATES_DIRS)){
        return -1;
    }
    return CustomXP;
}

int SetMagicPoints(int x){
    if( x > GetMaxMagicPoints() )
        SetStat("intelligence", (x-50)/10, GetStatClass("intelligence"));
    AddMagicPoints( x - GetMagicPoints() );
    return GetMagicPoints();
}

int SetMaxMagicPoints(int x){
    SetStat("intelligence", (x-50)/10, GetStatClass("intelligence"));
    return GetMaxMagicPoints();
}

float SetStaminaPoints(float x){
    if( x > GetMaxStaminaPoints() )
        SetStat("agility", to_int((x-50.0)/10.0), GetStatClass("agility"));
    AddStaminaPoints( x - GetStaminaPoints() );
    return to_float(GetStaminaPoints());
}

float SetMaxStaminaPoints(float x){
    SetStat("agility", (x-50.0)/10.0, GetStatClass("agility"));
    return GetMaxStaminaPoints();
}

varargs void SetCurrency(mixed val, int amount){
    if( stringp(val) ) AddCurrency(val, amount);
    else if( mapp(val) ){
        string *currs;
        int i;

        i = sizeof(currs = keys(val));
        while(i--) AddCurrency(currs[i], val[currs[i]]);
    }
    else error("Bad argument 1 to SetCurrency().");
}

mixed SetEncounter(mixed val){ return (Encounter = val); }

mixed SetAggressive(mixed val){
    if(!sizeof(Encounter)){
        if(val) Encounter = 100;
        else Encounter = 0;
    }
    return Encounter;
}

string *AddEncounter(string nom){
    if( !stringp(nom) ) error("Bad argument 1 to AddEncounter()\n");
    if( Encounter && !pointerp(Encounter) ) return 0;
    else Encounter += ({ convert_name(nom) });
    return Encounter;
}

string *RemoveEncounter(string nom){
    if( !stringp(nom) ) error("Bad argument 1 to RemoveEncounter()\n");
    if( Encounter && !pointerp(Encounter) ) return 0;
    else if(Encounter) Encounter -= ({ convert_name(nom) });
    return Encounter;
}

mixed GetEncounter(){ return Encounter; }

mixed SetDie(mixed val){ return (Die = val); }

mixed GetDie(){ return Die; }

string SetKeyName(string nom){
    set_living_name(nom = object::SetKeyName(nom));
    return nom;
}

string GetName(){ return object::GetName(); }

string GetCapName(){ return object::GetCapName(); }

string GetShort(){
    string ret = object::GetShort(); 
    object *riders = GetRiders();
    string *names = ({});
    if(sizeof(riders)) riders = filter(riders,
            (: (!$1->GetInvis() || this_player()->GetWizVision()) :) );
    if(riders && sizeof(riders) && VisibleRiders){
        foreach(object rider in riders){
            names += ({ rider->GetShort() });
        }
        ret += " "+MountStyle+" by "+conjunction(names);
    }
    return ret;
}

string GetPlainShort(){
    return object::GetShort();
}

varargs string GetLong(string str){
    mapping counts;
    string item, what;
    string *affects = ({});
    object *riders = this_object()->GetRiders();
    string *ridernames = ({});

    str = object::GetLong() + "\n";
    what = "The "+GetGender()+" "+GetRace();
    str += living::GetLong(what);
    foreach(item in map(all_inventory(),
                (: $1->GetAffectLong(this_object()) :))){
        if(item && member_array(item,affects) == -1) affects += ({ item });
    }
    if(sizeof(affects)) str += implode(affects,"\n")+"\n";
    if(this_object()->GetAffectLong()) str += this_object()->GetAffectLong();
    if(riders && sizeof(riders) && VisibleRiders){
        foreach(object rider in riders){
            ridernames += ({ rider->GetShort() });
        }
        str += capitalize(GetPlainShort())+" is "+MountStyle+" by "+conjunction(ridernames)+".\n";
    }
    counts = ([]);
    foreach(item in map(
                filter(all_inventory(), (: !($1->GetInvis(this_object())) :)),
                (: $1->GetEquippedShort() :)))
        if( item ) counts[item]++;
    if( sizeof(counts) ) str += GetCapName() + " is carrying:\n";
    foreach(item in keys(counts))
        str += capitalize(consolidate(counts[item], item)) + "\n";
    return str;
}

int DisableActions(int x){
    if(x) actions_enabled = 0;
    else actions_enabled = 1;
    return actions_enabled;
}

int EnableActions(int x){
    if(x) actions_enabled = 1;
    else actions_enabled = 0;
    return actions_enabled;
}

int GetActionsEnabled(){
    return actions_enabled;
}

void SetAction(int chance, mixed val){
    ActionChance = chance;
    if( stringp(val) ) val = ({ val });
    else if( !functionp(val) && !pointerp(val) )
        error("Bad argument 2 to SetAction()\n");
    Action = val;
}

mixed GetAction(){ return Action; }

void SetCombatAction(int chance, mixed val){
    CombatActionChance = chance;
    if( stringp(val) ) val = ({ val });
    else if( !functionp(val) && !pointerp(val) )
        error("Bad argument 2 to SetCombatAction()\n");
    CombatAction = val;
}

mixed GetCombatAction(){ return CombatAction; }

int AddCarriedMass(int x){ return living::AddCarriedMass(x); }

mixed *GetCommands(){ return commands(); }

string GetCommandFail(){ return "What?"; }

int AddEnemy(object ob){
    string tmp;

    if( !living::AddEnemy(ob) ) return 0;
    if( member_array(tmp = ob->GetKeyName(), EnemyNames) == -1 )
        EnemyNames += ({ tmp });
    return 1;
}

string *GetEnemyNames(){ return EnemyNames; }

int GetRadiantLight(int ambient){
    return (object::GetRadiantLight(ambient) +
            container::GetRadiantLight(ambient));
}

int *GetScreen(){ return ({ 80, 24 }); }

int GetAutoStand(){ return AutoStand; }

int SetAutoStand(int i){
    AutoStand = i;
    return AutoStand;
}

string GetMountStyle(){
    return MountStyle;
}

string SetMountStyle(string str){
    if(str && stringp(str)) MountStyle = str;
    return MountStyle;
}

int GetVisibleRiders(){
    return VisibleRiders;
}

int SetVisibleRiders(int i){
    if(i) VisibleRiders = 1;
    else VisibleRiders = 0;
    return VisibleRiders;
}
