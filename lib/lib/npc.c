/*    /lib/npc.c
 *    from the Dead Souls LPC Library
 *    the standard non-player character object
 *    created by Descartes of Borg 950323
 *    Version: @(#) npc.c 1.11@(#)
 *    Last Modified: 96/12/21
 */

#include <lib.h>
#include <daemons.h>
#include <position.h>
#include <armour_types.h>
#include <message_class.h>
#include "include/npc.h"

inherit LIB_CHAT;
inherit LIB_COMMAND;
inherit LIB_CONTAINER;
inherit LIB_LIVING;
inherit LIB_MESSAGES;
inherit LIB_MOVE;
inherit LIB_OBJECT;

private int ActionChance, CombatActionChance;
private mixed Encounter;
private string *EnemyNames;
private static int Level, Unique;
private static mixed Die, Action, CombatAction;
private static mapping Inventory;

static void create() {
    AddSave( ({ "CarriedMass" }) );
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
    Inventory = ([]);
}

static void init() {
    string *enemies;
    
    if( !query_heart_beat() ) {
        eventCheckHealing();
        set_heart_beat( GetHeartRate() );
    }
    if( sizeof(enemies = GetEnemyNames()) ) {
	if( member_array((string)this_player()->GetKeyName(),enemies) != -1 ) {
	    eventExecuteAttack(this_player());
	    return;
	}
    }
    if( Encounter ) {
        int x = 0;

        if( functionp(Encounter) ) {
	    x = (int)evaluate(Encounter, this_player());
	}
        else if( arrayp(Encounter) ) {
	    if( member_array(this_player()->GetKeyName(), Encounter) > -1 ) {
		x = 1;
	    }
	    else {
		x = 1;
	    }
	}
        else if( (int)this_player()->GetStatLevel("charisma") < Encounter ) {
	    x = 1;
	}
        if( x ) {
	    SetAttack(this_player());
	}
    }
}

static void heart_beat() {
    int position;
    
    living::heart_beat();
    if( !ContinueHeart() ) {
        set_heart_beat(0);
        return;
    }
    position = GetPosition();
    if( position == POSITION_LYING || position == POSITION_SITTING ) {
	eventForce("stand up");
    }
    if( !GetInCombat() && ActionChance > random(100) ) {
        int x;

        if( functionp(Action) ) evaluate(Action);
        else if( pointerp(Action) && (x = sizeof(Action)) ) {
            string act;

            act = Action[random(x)];
            if( act && act != "" && act[0] == '!' && act != "!" ) {
                act = act[1..];
                eventForce(act);
            }
            else message("other_action", act, environment());
        }
    }
    else if( GetInCombat() && CombatActionChance > random(100) ) {
        int x;

        if( functionp(CombatAction) ) evaluate(CombatAction);
        else if( pointerp(CombatAction) && (x = sizeof(CombatAction)) ) {
            string act;

            act = CombatAction[random(x)];
            if( act && act != "" && act[0] == '!' && act != "!" ) {
                act = act[1..];
                eventForce(act);
            }
            else message("other_action", act, environment());
        }
    }
}

void receive_message(string cl, string msg) { catch_tell(msg); }

void catch_tell(string msg) { }

static int Destruct() {
    if( GetParty() ) PARTY_D->eventLeaveParty(this_object());
    living::Destruct();
    return object::Destruct();
}

void eventReconnect() { }

/* ***************  /lib/npc.c command functions  ************** */
static int cmdAll(string arg) {
    object env;
    string verb;

    verb = query_verb();
    env = environment();
    if( chat::chat_command(verb + " " + arg) == "" ) return 1;
    return command::cmdAll(arg);
}

/* ***************  /lib/npc.c events  *************** */
int eventCompleteMove(mixed dest) {
    mixed val;
    string file;
    int x;

    if( environment() ) return move::eventMove(dest);
    else x = move::eventMove(dest);
    if( !x ) return x;
    foreach(file, val in Inventory) {
        object ob;

        if( intp(val) ) {
            if( val < 0 ) {
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
        else if( functionp(val) ) {
            if( !(ob = new(file)) ) continue;
            if( ob->eventMove(this_object()) ) evaluate(val);
        }
    }
    return x;
}

int eventDestruct() {
    chat::eventDestruct();
    object::eventDestruct();
}

varargs int eventDie(object agent) {
    int x;

    if( (x = living::eventDie(agent)) != 1 ) return x;
    if( stringp(Die) )  {
        message("other_action", Die, environment(), ({ this_object() }));
        if( agent) message("my_action", "You kill " + GetName() + ".", agent);
    }
    else if( functionp(Die) && !evaluate(Die, agent) ) return 0;
    else {
        message("other_action", "%^BOLD%^%^RED%^"+ GetName() + " drops dead.",
                environment(), ({ this_object() }) );
        if( agent ) message("my_action", "You kill " + GetName() + ".", agent);
    }
    set_heart_beat(0);
    call_out( (: Destruct :), 0);
    return 1;
}

mixed eventTurn(object who) {
    if( !living::eventTurn(who) ) {
	return 0;
    }
    all_inventory()->eventDestruct();
    call_out((: Destruct :), 0);
    return 1;
}

void eventEnemyDied(object ob) {
    living::eventEnemyDied(ob);
    EnemyNames -= ({ (string)ob->GetKeyName() });
}

int eventMove(mixed dest) {
    int ret;

    ret = eventCompleteMove(dest);
    eventMoveFollowers(environment(this_object()));
    return ret;
}

varargs int eventMoveLiving(mixed dest, string omsg, string imsg) {
    object *inv;
    object prev, env;
    string msgclass;

    if( prev = environment() ) {
        if( stringp(dest) ) {
            if(dest[0] != '/') {
                string *arr;

                arr = explode(file_name(prev), "/");
                dest = "/"+implode(arr[0..sizeof(arr)-2], "/")+"/"+dest;
            }
        }
        if( !eventCompleteMove(dest) ) {
	    eventPrint("You remain where you are.");
            return 0;
        }
        inv = filter(all_inventory(prev), (: (!GetInvis($1) && living($1) &&
                                              ($1 != this_object())) :));
        if( !omsg || omsg == "" ) omsg = GetMessage("telout");
        else omsg = GetMessage("leave", omsg);
	inv->eventPrint(omsg, MSG_ENV);
    }
    else if( !eventCompleteMove(dest) ) {
	eventPrint("You remain where you are.");
        return 0;
    }
    inv = filter(all_inventory(environment()),
                 (: (!GetInvis($1) && living($1) && ($1 != this_object())) :));
    if( (!imsg || imsg == "") && (!omsg || omsg == "") )
      imsg = GetMessage(msgclass = "telin");
    else if( !imsg || imsg == "" ) imsg = GetMessage(msgclass = "come", imsg);
    else imsg = replace_string(imsg, "$N", GetName());
    inv->eventPrint(imsg, MSG_ENV);
    eventMoveFollowers(environment(this_player()));
    return 1;
}

varargs int eventPrint(string msg, mixed arg2, mixed arg3) {
    return 1;
}

int eventReceiveObject() {
    object ob;

    ob = previous_object();
    if( !ob || !container::eventReceiveObject() ) return 0;
    AddCarriedMass((int)ob->GetMass());
    return 1;
}

int eventReleaseObject() {
    object ob;

    ob = previous_object();
    if( !ob || !container::eventReleaseObject() ) return 0;
    AddCarriedMass( -((int)ob->GetMass()) );
    return 1;
}

varargs int eventShow(object who, string str) {
    if( !living::eventShow(who, str) ) return 0;
    eventPrint((string)this_player()->GetName() + " looks you over.");
    return 1;
}

/*  ***************  /lib/npc.c modal functions  *************** */

int CanCarry(int amount) { return living::CanCarry(amount); }

mixed CanGet(object who) { return GetName() + " is a living being!"; }

int CanReceive(object ob) { return CanCarry((int)ob->GetMass()); }

/*  ***************  /lib/npc.c lfuns  ***************  */

static int ContinueHeart() {
    object env;

    if( !(env = environment()) ) return 0;
    if( !sizeof(filter(all_inventory(env), (: userp :))) ) return 0;
    return 1;
}

/*  ***************  /lib/npc.c data functions  ***************  */

mapping SetInventory(mapping mp ) { return (Inventory = mp); }

varargs string SetRace(string race, mixed extra) {
    race = living::SetRace(race, extra);
    eventCompleteHeal(GetMaxHealthPoints());
    return race;
}

string SetClass(string cls) {
    string *skills;
    int x, i;

    cls = living::SetClass(cls);
    x = Level;
    i = sizeof(skills = GetSkills());
    while(i--) {
        int y;

        y = (GetSkillClass(skills[i]) || 5);
        SetSkill(skills[i], (3*x)/y, y);
    }
    return cls;
}

int SetLevel(int x) {
    string *tmp;
    int i;

    Level = x;
    i = sizeof(tmp = GetSkills());
    while(i--) {
        int y;

        y = (GetSkillClass(tmp[i]) || 5);
        SetSkill(tmp[i], (3*x)/y, y);
    }
    i = sizeof(tmp = GetStats());
    while(i--) {
        int y;

        y = (GetStatClass(tmp[i]) || 5);
        SetStat(tmp[i], ((5-y)*10) + (3*x)/y, y);
    }
    eventCompleteHeal(GetMaxHealthPoints());
    return Level;
}

int GetLevel() { return Level; }

int SetHealthPoints(int x) {
    if( x > GetMaxHealthPoints() )
      SetStat("durability", (x-50)/10, GetStatClass("durability"));
    AddHealthPoints( x - GetHealthPoints() );
    return GetHealthPoints();
}

int SetMaxHealthPoints(int x) {
    SetStat("durability", to_int((x-50)/10), GetStatClass("durability"));
    return GetMaxHealthPoints();
}

int SetMagicPoints(int x) {
    if( x > GetMaxMagicPoints() )
      SetStat("intelligence", (x-50)/10, GetStatClass("intelligence"));
    AddMagicPoints( x - GetMagicPoints() );
    return GetMagicPoints();
}

int SetMaxMagicPoints(int x) {
    SetStat("intelligence", (x-50)/10, GetStatClass("intelligence"));
    return GetMaxMagicPoints();
}

float SetStaminaPoints(float x) {
    if( x > GetMaxStaminaPoints() )
      SetStat("agility", to_int((x-50.0)/10.0), GetStatClass("agility"));
    AddStaminaPoints( x - GetStaminaPoints() );
    return to_float(GetStaminaPoints());
}

float SetMaxStaminaPoints(float x) {
    SetStat("agility", (x-50.0)/10.0, GetStatClass("agility"));
    return GetMaxStaminaPoints();
}

varargs void SetCurrency(mixed val, int amount) {
    if( stringp(val) ) AddCurrency(val, amount - GetCurrency(val));
    else if( mapp(val) ) {
        string *currs;
        int i;

        i = sizeof(currs = keys(val));
        while(i--) AddCurrency(currs[i], val[currs[i]]-GetCurrency(currs[i]));
    }
    else error("Bad argument 1 to SetCurrency().");
}

mixed SetEncounter(mixed val) { return (Encounter = val); }

string *AddEncounter(string nom) {
    if( !stringp(nom) ) error("Bad argument 1 to AddEncounter()\n");
    if( Encounter && !pointerp(Encounter) ) return 0;
    else Encounter += ({ convert_name(nom) });
    return Encounter;
}

string *RemoveEncounter(string nom) {
    if( !stringp(nom) ) error("Bad argument 1 to RemoveEncounter()\n");
    if( Encounter && !pointerp(Encounter) ) return 0;
    else Encounter -= ({ convert_name(nom) });
    return Encounter;
}

mixed GetEncounter() { return Encounter; }

mixed SetDie(mixed val) { return (Die = val); }

mixed GetDie() { return Die; }

string SetKeyName(string nom) {
    set_living_name(nom = object::SetKeyName(nom));
    return nom;
}

string GetName() { return object::GetName(); }

string GetCapName() { return object::GetCapName(); }

string GetShort() { return object::GetShort(); }

varargs string GetLong(string str) {
    mapping counts;
    string item, what;
    float h;

    str = object::GetLong() + "\n";
    what = "The "+GetGender()+" "+GetRace();
    str += living::GetLong(what);
    foreach(item in map(all_inventory(),
                        (: (string)$1->GetAffectLong(this_object()) :)))
      if( item ) str += item + "\n";
    counts = ([]);
    foreach(item in map(all_inventory(),(: (string)$1->GetEquippedShort() :)))
      if( item ) counts[item]++;
    if( sizeof(counts) ) str += GetCapName() + " is carrying:\n";
    foreach(item in keys(counts))
      str += capitalize(consolidate(counts[item], item)) + "\n";
    return str;
}

void SetAction(int chance, mixed val) {
    ActionChance = chance;
    if( stringp(val) ) val = ({ val });
    else if( !functionp(val) && !pointerp(val) )
      error("Bad argument 2 to SetAction()\n");
    Action = val;
}

mixed GetAction() { return Action; }

void SetCombatAction(int chance, mixed val) {
    CombatActionChance = chance;
    if( stringp(val) ) val = ({ val });
    else if( !functionp(val) && !pointerp(val) )
      error("Bad argument 2 to SetCombatAction()\n");
    CombatAction = val;
}

mixed GetCombatAction() { return CombatAction; }

int AddCarriedMass(int x) { return living::AddCarriedMass(x); }

mixed *GetCommands() { return commands(); }

int SetUnique(int x) {
    Unique = x;
    if( Unique ) UNIQUE_D->eventTouchObject();
    return Unique;
}

int GetUnique() { return Unique; }

string GetCommandFail() { return "What?"; }

int AddEnemy(object ob) {
    string tmp;
    
    if( !living::AddEnemy(ob) ) return 0;
    if( member_array(tmp = (string)ob->GetKeyName(), EnemyNames) == -1 )
      EnemyNames += ({ tmp });
    return 1;
}

string *GetEnemyNames() { return EnemyNames; }

int GetRadiantLight(int ambient) {
    return (object::GetRadiantLight(ambient) +
	    container::GetRadiantLight(ambient));
}

int *GetScreen() { return ({ 80, 24 }); }
