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
#include <armor_types.h>
#include <message_class.h>
#include <vision.h>
#include "include/npc.h"

inherit LIB_CHAT;
inherit LIB_COMMAND;
inherit LIB_CONTAINER;
inherit LIB_LIVING;
inherit LIB_MESSAGES;
inherit LIB_MOVE;
inherit LIB_OBJECT;
inherit LIB_SAVE;
inherit LIB_DOMESTICATE;

private int CustomXP, ActionChance, CombatActionChance, AutoStand;
private int MaximumHealth = 0;
private mixed Encounter;
private string *EnemyNames;
private static int NPCLevel, Unique;
private static mixed Die, Action, CombatAction;
private static mapping Inventory;

int eventExtraAction(){ return 1; }

static void create() {
    //AddSave( ({ "CarriedMass" }) );
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
}

void CheckEncounter(){
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

    if( Encounter && !query_invis(this_player(),this_object())) {
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

static void init() {
    CheckEncounter();
}

static void heart_beat() {
    int position;

    living::heart_beat();
    if( !ContinueHeart() ) {
	set_heart_beat(0);
	return;
    }
    eventExtraAction();
    position = GetPosition();
    if( position == POSITION_LYING || position == POSITION_SITTING ) {
	if(AutoStand && 
	  !RACES_D->GetLimblessRace(this_object()->GetRace())) 
	    eventForce("stand up");
	if(GetInCombat() && 
	  !RACES_D->GetLimblessRace(this_object()->GetRace()) ) 
	    eventForce("stand up");
    }
    if( !GetInCombat() && ActionChance > random(100) ) {
	int x;

	if( functionp(Action) ) evaluate(Action);
	else if( pointerp(Action) && (x = sizeof(Action)) ) {
	    mixed act;

	    act = Action[random(x)];
	    if(functionp(act)) {
		evaluate(act);
		return;
	    }
	    if( act && act != "" && act[0] == '!' && act != "!" ) {
		act = act[1..];
		eventForce(act);
	    }
	    else message("other_action", act, environment());
	}
    }
    else if( GetInCombat() && CombatActionChance > random(100) ) {
	int x;

	if( functionp(CombatAction) ) {
	    evaluate(CombatAction);
	}
	else if( pointerp(CombatAction) && (x = sizeof(CombatAction)) ) {
	    mixed mact;
	    string act;

	    x--;
	    mact = CombatAction[random(x)];
	    if( functionp(mact) ) {

		evaluate(mact);
		return;
	    }
	    if( !stringp(mact) ) mact = "emote looks confused.";
	    else act = mact;
	    if( act && act != "" ) {
		eventForce(act);
	    }
	    else message("other_action", act, environment());
	}
    }
}

void eventDescribeEnvironment(int brief) {
    object env;
    mixed tmp;
    string *shorts;
    string desc, smell, sound, touch;
    int i, maxi;

    if(!(env = environment(this_object()))) {
	eventPrint("You are nowhere.", MSG_ROOMDESC);
	return;
    }
    switch( i = GetEffectiveVision() ) {
    case VISION_BLIND:
	eventPrint("You are blind and can see nothing.");
	break;
    case VISION_TOO_DARK:
	eventPrint("It is much too dark to see.");
	break;
    case VISION_DARK:
	eventPrint("It is too dark to see.");
	break;
    case VISION_TOO_BRIGHT:
	eventPrint("It is much too %^YELLOW%^bright%^RESET%^ to see.");
	break;
    case VISION_BRIGHT:
	eventPrint("It is too %^YELLOW%^bright%^RESET%^ to see.");
	break;
    }
    if( !brief ) {
	if( i == VISION_CLEAR ) {
	    desc = (string)env->GetObviousExits() || "";
	    desc = capitalize((string)env->GetShort() || "")
	    + " [" + desc + "]\n";
	}
	else desc = "";
	if( i == VISION_CLEAR || i == VISION_LIGHT || i == VISION_DIM )
	    desc += (string)env->GetLong();
	if(functionp(tmp = (mixed)env->GetSmell("default")))
	    tmp = (string)(*tmp)("default");
	smell = tmp;
	if(functionp(tmp = (mixed)env->GetListen("default")))
	    tmp = (string)(*tmp)("default");
	sound = tmp;
	if( functionp(tmp = (mixed)env->GetTouch("default")) )
	    tmp = evaluate(tmp, "default");
	touch = tmp;
    }
    else {
	if( i == VISION_CLEAR || i == VISION_LIGHT || i == VISION_DIM ) {
	    desc = (string)env->GetShort();
	    if( (tmp = (string)env->GetObviousExits()) && tmp != "" )
		desc += " [" + tmp + "]";
	}
	else desc = "";
    }
    if( desc ) eventPrint(desc, MSG_ROOMDESC);
    if( smell ) eventPrint("%^GREEN%^" + smell, MSG_ROOMDESC);
    if( sound ) eventPrint("%^CYAN%^" + sound, MSG_ROOMDESC);
    if( touch ) eventPrint("%^YELLOW%^" + touch, MSG_ROOMDESC);
    desc = "";
    if( i == VISION_CLEAR ) {
	mapping lying = ([]);
	shorts = map(filter(all_inventory(env),
	    function(object ob) {
		if( living(ob) ) return 0;
		if( (int)ob->GetInvis(this_object()) )
		    return 0;
		if( (int)ob->isFreshCorpse() ) return 0;
		return 1;
	      }), (: (string)$1->GetShort() :));
	  foreach(string s in shorts) {
	      if( s ) {
		  lying[s]++;
	      }
	  }
	  for(i=0, desc = 0, maxi = sizeof(shorts = keys(lying)); i<maxi; i++) {
	      string key = shorts[i];
	      int val = lying[shorts[i]];

	      if( val < 2 ) {
		  if( !desc ) desc = "%^MAGENTA%^" +
		      capitalize(key) + "%^RESET%^MAGENTA%^";
		  else desc += key + "%^RESET%^MAGENTA%^";
	      }
	      else {
		  if( !desc ) desc = "%^MAGENTA%^" +
		      capitalize(consolidate(val, key)) +
		      "%^RESET%^MAGENTA%^";
		  else desc += consolidate(val, key) +
		      "%^RESET%^MAGENTA%^";
	      }
	      if( i == maxi - 1 ) {
		  if( maxi > 1 || val >1 )
		      desc += " are here.%^RESET%^\n";
		  else desc += " is here.%^RESET%^\n";
	      }
	      else if( i == maxi - 2 ) {
		  if( maxi == 2 ) {
		      desc += " and ";
		  }
		  else {
		      desc += ", and ";
		  }
	      }
	      else desc += ", ";
	  }
      }
	i = GetEffectiveVision();
	if( i == VISION_CLEAR || i == VISION_LIGHT || i == VISION_DIM ) {
	    mapping lying = ([]), sitting = ([]), standing = ([]), flying = ([]);
	    object *obs;
	    string key;
	    int val;

	    obs = filter(all_inventory(env), function(object ob) {
		  if( (int)ob->GetInvis(this_object()) ) return 0;
		  if( living(ob) ) return 1;
		  if( (int)ob->isFreshCorpse() )
		      return 1;
		}) - ({ this_object() });
	      maxi = sizeof(shorts = map(obs, (: (string)$1->GetHealthShort() :)));
	      foreach(object liv in obs) {
		  string s = (string)liv->GetHealthShort();
		  int pos = (int)liv->GetPosition();

		  if( !s ) continue;
		  if( pos == POSITION_STANDING) standing[s]++;
		  else if( pos == POSITION_LYING || (int)liv->isFreshCorpse() )
		      lying[s]++;
		  else if( pos == POSITION_SITTING ) sitting[s]++;
		  else if( pos == POSITION_FLYING ) flying[s]++;
		  else lying[s]++;
	      }
	      if( !desc ) {
		  tmp = "";
	      }
	      else {
		  tmp = desc;
	      }
	      desc = "";
	      foreach(key, val in lying) {
		  if( val<2 ) desc += capitalize(key) + "%^RESET%^ is lying down.";
		  else desc += capitalize(consolidate(val, key)) +
		      "%^RESET%^ are lying down.";
		  desc += "\n";
	      }
	      foreach(key, val in sitting) {
		  if( val<2 )
		      desc += capitalize(key) + "%^RESET%^ is sitting down.";
		  else desc += capitalize(consolidate(val, key)) +
		      "%^RESET%^ are sitting down.";
		  desc += "\n";
	      }
	      foreach(key, val in standing) {
		  if( val<2 )
		      desc += capitalize(key) + "%^RESET%^ is standing here.";
		  else desc += capitalize(consolidate(val, key)) +
		      "%^RESET%^ are standing here.";
		  desc += "\n";
	      }
	      foreach(key, val in flying) {
		  if( val<2 )
		      desc += capitalize(key) + "%^RESET%^ is hovering here.";
		  else desc += capitalize(consolidate(val, key)) +
		      "%^RESET%^ are hovering here.";
		  desc += "\n";
	      }
	  }
	    if( tmp ) {
		desc = tmp + desc;
	    }
	    if( sizeof(desc) ) {
		eventPrint(desc + "\n", MSG_ROOMDESC);
	    }
	}

	/*
	 *	void receive_message(string cl, string msg) { 
	 *	    object *riders = get_livings(this_object());
	 *	    if(NPC_CATCH_TELL_DEBUG){
	 *	    tell_room("/domains/default/room/catchtell","-------");
	 *	    tell_room("/domains/default/room/catchtell",timestamp());
	 *	    tell_room("/domains/default/room/catchtell","obj: "+identify(this_object()));
	 *	    tell_room("/domains/default/room/catchtell","cl: "+cl);
	 *	    tell_room("/domains/default/room/catchtell","msg: "+msg);
	 *	    tell_room("/domains/default/room/catchtell","-------");
	 *            }
	 *	    if(sizeof(riders)){
	 *		foreach(object living in riders){
	 *		    if(living->GetProperty("mount") == this_object())
	 *			tell_object(living, "(mount): "+msg);
	 *		}
	 *	    }
	 *	}
	 */

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

	varargs int eventDie(mixed agent) {
	    int x;

	    if(this_object()->GetDead() || this_object()->GetDeathEvents()) return 0;

	    if( (x = living::eventDie(agent)) != 1 ) return x;
	    if( stringp(Die) )  {
		message("other_action", Die, environment(), ({ this_object() }));
		if( agent) message("my_action", "You kill " + GetName() + ".", agent);
	    }
	    else if( functionp(Die) && !evaluate(Die, agent) ) return 0;
	    else {
		if(GetPosition() == POSITION_STANDING) message("other_action", "%^BOLD%^%^RED%^"+ GetName() + " drops dead.", environment(), ({ this_object() }) );
		else if(GetPosition() == POSITION_FLYING) message("other_action", "%^BOLD%^%^RED%^"+ GetName() + " falls dead.", environment(), ({ this_object() }) );
		else message("other_action", "%^BOLD%^%^RED%^"+ GetName() + " dies.", environment(), ({ this_object() }) );
		if( agent ) message("my_action", "You kill " + GetName() + ".", agent);
	    }
	    set_heart_beat(0);
	    call_out( (: Destruct :), 0);
	    flush_messages();
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
	    //if(environment(this_object())) eventMoveFollowers(environment(this_object()));
	    return ret;
	}

	varargs int eventMoveLiving(mixed dest, string omsg, string imsg) {
	    object *inv;
	    object prev;
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
		else if(GetPosition() == POSITION_SITTING ||
		  GetPosition() == POSITION_LYING ){
		    omsg = this_object()->GetName()+" crawls "+omsg+".";
		}
		else if(GetPosition() == POSITION_FLYING ){
		    omsg = this_object()->GetName()+" flies "+omsg+".";
		}

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
	    else if(GetPosition() == POSITION_SITTING ||
	      GetPosition() == POSITION_LYING ){
		imsg = this_object()->GetName()+" crawls in";
	    }
	    else if(GetPosition() == POSITION_FLYING ){
		imsg = this_object()->GetName()+" flies in.";
	    }

	    else if( !imsg || imsg == "" ) imsg = GetMessage(msgclass = "come", imsg);
	    else imsg = replace_string(imsg, "$N", GetName());
	    inv->eventPrint(imsg, MSG_ENV);
	    this_object()->eventDescribeEnvironment(0);
	    eventMoveFollowers(environment(this_player()));
	    return 1;
	}

	varargs int eventPrint(string msg, mixed arg2, mixed arg3) {
	    object *riders = GetRiders();
	    if(NPC_CATCH_TELL_DEBUG){
		tell_room("/domains/default/room/catchtell","-------");
		tell_room("/domains/default/room/catchtell",timestamp());
		tell_room("/domains/default/room/catchtell","obj: "+identify(this_object()));
		tell_room("/domains/default/room/catchtell","msg: "+msg);
		tell_room("/domains/default/room/catchtell","arg2: "+identify(arg2));
		tell_room("/domains/default/room/catchtell","arg3: "+identify(arg3));
		tell_room("/domains/default/room/catchtell","stack: "+get_stack());
		tell_room("/domains/default/room/catchtell","previous: "+identify(previous_object(-1)));
		tell_room("/domains/default/room/catchtell","-------");
	    }
	    if(riders){
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
			if(arg2 & MSG_CONV && !rider_source) true();
			else tmp_riders->eventPrint(msg, arg2, arg3);
		    }
		    i1 = sizeof(previous_object(-1)) -1;
		    if(i1 < 0) i1 = 0;
		    if(sizeof(previous_object(-1)) &&
		      (member_array(previous_object(),riders) != -1 ||
			member_array(previous_object(-1)[i1],riders) != -1) &&
		      (!intp(arg2) || (!(arg2 & MSG_CONV) && !(arg2 & MSG_ENV))) && 
		      member_array(this_object(),previous_object(-1)) == -1){ 
			environment()->eventPrint(msg, arg2, arg3);
		    }
		}
	    }  
	    return 1;
	}

	int eventReceiveObject(object who) {
	    object ob;

	    ob = previous_object();
	    if( !ob || !container::eventReceiveObject() ) return 0;
	    AddCarriedMass((int)ob->GetMass());
	    if(environment()) environment()->AddCarriedMass((int)ob->GetMass());
	    return 1;
	}

	int eventReleaseObject(object who) {
	    object ob;

	    ob = previous_object();
	    if( !ob || !container::eventReleaseObject() ) return 0;
	    if( ob->GetMass() ){
		AddCarriedMass( -((int)ob->GetMass()) );
		if(environment()) environment()->AddCarriedMass(-(ob->GetMass()));
	    }
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
	    if( !sizeof(filter(all_inventory(env), (: living :))) ) return 0;
	    return 1;
	}

	/*  ***************  /lib/npc.c data functions  ***************  */

	mapping SetInventory(mapping mp ) { return (Inventory = mp); }
	mapping GetInventory() { return copy(Inventory); }

	varargs string SetRace(string race, mixed extra) {
	    //if(arrayp(extra) && !sizeof(extra)) extra = ({ ({}), ({}), ({}), ({}), ({}) });
	    race = living::SetRace(race, extra);
	    eventCompleteHeal(GetMaxHealthPoints());
	    return race;
	}

	string SetClass(string cls) {
	    string *skills;
	    int x, i;

	    cls = living::SetClass(cls);
	    x = NPCLevel;
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

	    NPCLevel = x;
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
	    return NPCLevel;
	}

	int GetLevel() { return NPCLevel; }

	int SetCustomXP(int i){
	    if(!i) i = 0;
	    CustomXP = i;
	    return CustomXP;
	}

	int GetCustomXP(){
	    return CustomXP;
	}

	int SetHealthPoints(int x) {
	    if( x > GetMaxHealthPoints() )
		SetStat("durability", (x-50)/10, GetStatClass("durability"));
	    AddHealthPoints( x - GetHealthPoints() );
	    return GetHealthPoints();
	}

	varargs int GetMaxHealthPoints(string limb){
	    if(MaximumHealth) return MaximumHealth;
	    else return living::GetMaxHealthPoints(limb);
	}

	int SetMaxHealthPoints(int x) {
	    if(x) MaximumHealth = x;
	    else SetStat("durability", to_int((x-50)/10), GetStatClass("durability"));
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
	    if( stringp(val) ) AddCurrency(val, amount);
	    else if( mapp(val) ) {
		string *currs;
		int i;

		i = sizeof(currs = keys(val));
		while(i--) AddCurrency(currs[i], val[currs[i]]);
	    }
	    else error("Bad argument 1 to SetCurrency().");
	}

	mixed SetEncounter(mixed val) { return (Encounter = val); }

	mixed SetAggressive(mixed val){
	    if(sizeof(Encounter)) return Encounter;
	    else if(val) Encounter = 100;
	    else Encounter = 0;
	}


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

	string GetShort(){
	    string ret = object::GetShort(); 
	    object *riders = GetRiders();
	    string *names = ({});
	    if(riders && sizeof(riders)){
		foreach(object rider in riders){
		    names += ({ rider->GetShort() });
		}
		ret += " ridden by "+conjunction(names);
	    }
	    return ret;
	}

	string GetPlainShort(){
	    return object::GetShort();
	}

	varargs string GetLong(string str) {
	    mapping counts;
	    string item, what;
	    string *affects = ({});
	    object *riders = this_object()->GetRiders();
	    string *ridernames = ({});

	    str = object::GetLong() + "\n";
	    what = "The "+GetGender()+" "+GetRace();
	    str += living::GetLong(what);
	    foreach(item in map(all_inventory(),
		(: (string)$1->GetAffectLong(this_object()) :))){
		if(item && member_array(item,affects) == -1) affects += ({ item });
	    }
	    if(sizeof(affects)) str += implode(affects,"\n")+"\n";
	    if(this_object()->GetAffectLong()) str += this_object()->GetAffectLong();
	    if(riders && sizeof(riders)){
		foreach(object rider in riders){
		    ridernames += ({ rider->GetShort() });
		}
		str += capitalize(GetPlainShort())+" is ridden by "+conjunction(ridernames)+".\n";
	    }
	    counts = ([]);
	    foreach(item in map(
		filter(all_inventory(), (: !((int)$1->GetInvis(this_object())) :)),
		(: (string)$1->GetEquippedShort() :)))
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

	int GetAutoStand(){ return AutoStand; }

	int SetAutoStand(int i){
	    AutoStand = i;
	    return AutoStand;
	}
