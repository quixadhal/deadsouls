/*    /lib/obj/room.c
 *    From the Dead Souls LPC Library
 *    The room object used to represent any room in the game
 *    Created by Descartes of Borg 940711
 *    Version: @(#) room.c 1.20@(#)
 *    Last Modified: 050912
 */


#include <lib.h>
#include <rooms.h>
#include <config.h>
#include <daemons.h>
#include <function.h>
#include <medium.h>
#include <message_class.h>
#include <talk_type.h>
#include <privs.h>

inherit LIB_AMBIANCE;
inherit LIB_CLEAN;
inherit LIB_CONTAINER;
inherit LIB_EXITS;
inherit LIB_DESCRIPTION;
inherit LIB_INVENTORY;
inherit LIB_LOOK;
inherit LIB_PROPERTIES;

private function        Bury          = 0;
private string          Climate       = "temperate";
private int             DayLight      = 0;
private static string   DayLong       = 0;
private object array    DummyItems    = ({});
private static int      GasCheck      = time();
private float           Gravity       = 1.0;
private static mixed    Listen        = 0;
private int             NightLight    = 0;
private static string   NightLong     = 0;
private static int      NoReplace     = 0;
private static int      PlayerKill    = 0;
private static int      PoisonGas     = 0;
private static int      ResetNumber   = 0;
private static mixed    Search        = 0;
private static int      Shade         = 0;
private static mixed    Smell         = 0;
private static mixed    Touch         = 0;
private string          Town          = "wilderness";

string GetClimate();
int GetNightLight();
int GetDayLight();
int GetShade();
int elderp(object foo);
varargs int eventPrint(string msg, mixed arg2, mixed arg3);

/***********      /lib/room.c data manipulation functions      **********/

int GetAmbientLight() {
	int a;

	if( GetClimate() == "indoors" ) {
		return ambiance::GetAmbientLight();
	}
	if( query_night() ) {
		a = GetNightLight();
	}
	else {
		a = GetDayLight();
	}
	a += SEASONS_D->GetRadiantLight() - GetShade();
	return a;
}

function GetBury() {
	return Bury;
}

function SetBury(function what) {
	Bury = what;
}

static string GetExtraLong() {
	int i;
	string *l,*tmp;
	string ret;
	object array stuff;
	ret = "  ";
	tmp = ({});
	stuff=all_inventory(this_object()); 
	for(i=0; i<sizeof(stuff);i++){
		if(tmp = ({ stuff[i]->GetAffectLong() }) && !sizeof(l)) l = tmp;
		if( !sizeof(l) ) return 0;
		if(tmp = ({ stuff[i]->GetAffectLong() }) ) l += tmp;
	}
	if( !sizeof(l) ) return 0;
	ret += implode(l, "	 ");
	return ret;
}

string GetInternalDesc() {
	string ret, tmp;
	object ob;

	if( DayLong && !query_night() ) {
		ret = DayLong;
	}
	else if( NightLong && query_night() ) {
		ret = NightLong;
	}
	else {
		ret = container::GetInternalDesc();
	}
	if( !ret ) {
		ret = "";
	}
	if( tmp = GetExtraLong() ) {
		ret += GetExtraLong();
	}
	return ret;
}

int GetResetNumber() {
	return ResetNumber;
}

string array GetId() { return ({}); }

string SetDayLong(string str) { return (DayLong = str); }

string GetDayLong() { return DayLong; }

string SetNightLong(string str) { return (NightLong = str); }

string GetNightLong() { return NightLong; }

string SetClimate(string str) { return (Climate = str); }

string GetClimate() { return Climate; }

float SetGravity(float h) { return (Gravity = h); }

float GetGravity() { return Gravity; }

int GetDayLight() {
	return DayLight;
}

static int SetDayLight(int x) {
	return (DayLight = x);
}

static object array GetDummyItems() {
	DummyItems = filter(DummyItems, (: $1 :));
	return DummyItems;
}

varargs void AddItem(mixed item, mixed val, mixed adjectives) {
	object ob;

	if( objectp(item) ) {
		ob = item;
	}
	else {
		if( stringp(item) ) {
			item = ({ item });
		}
		if( stringp(adjectives) ) {
			adjectives = ({ adjectives });
		}
		ob = new(LIB_DUMMY, item, val, adjectives);
	}
	ob->eventMove(this_object());
	DummyItems = ({ DummyItems..., ob });
}

void RemoveItem(mixed item) {
	if( objectp(item) ) {
		DummyItems -= ({ item });
		item->eventDestruct();
		return;
	}
	else if( !arrayp(item) ) {
		item = ({ item });
	}
	foreach(object ob in GetDummyItems()) {
		if( sizeof(ob->GetId() & item) ) {
			ob->eventDestruct();
			DummyItems -= ({ ob });
			return;
		}
	}
}

void SetItems(mixed items) {
	DummyItems->eventDestruct();
	DummyItems = ({});
	if( arrayp(items) ) {
		items->eventMove(this_object());
		DummyItems = items;
	}
	else if( mapp(items) ) {
		foreach(mixed key, mixed val in items) {
			string array adjs = ({});
			object ob;

			if( stringp(key) ) {
				key = ({ key });
			}
			else {
				if( sizeof(key) == 2 && arrayp(key[0]) ) {
					adjs = key[1];
					key = key[0];
				}
			}
			ob = new(LIB_DUMMY, key, val, adjs);
			ob->eventMove(this_object());
			DummyItems = ({ DummyItems..., ob });
		}
	}
	else {
		error("Bad argument 1 to SetItems(), expected object array or "
				"mapping.\n");
	}
}

varargs void AddListen(mixed item, mixed val) {
	if( !val ) {
		Listen = item;
		return;
	}
	if( !item || item == "default" ) {
		Listen = val;
		return;
	}
	if( stringp(item) ) {
		item = ({ item });
	}
	foreach(string tmp in item) {
		foreach(object ob in GetDummyItems()) {
			if( ob->id(tmp) ) {
				ob->SetListen(val);
				break;
			}
		}
	}
}

mixed GetListen() {
	return Listen;
}

varargs void RemoveListen(mixed item) {
	if( !item || item == "default" ) {
		Listen = 0;
		return;
	}
	foreach(object ob in GetDummyItems()) {
		if( stringp(item) ) {
			if( ob->id(item) ) {
				ob->SetListen(0);
			}
		}
		else if( arrayp(item) ) {
			if( sizeof(ob->GetId() & item) ) {
				ob->SetListen(0);
			}
		}
	}
}

varargs void SetListen(mixed items, mixed arg) {
	if( !mapp(items) ) {
		if( !arg ) {
			AddListen("default", items);
		}
		else {
			AddListen(items, arg);
		}
		return;
	}
	foreach(mixed key, mixed val in items) {
		AddListen(key, val);
	}
}

int GetMedium() {
	return MEDIUM_LAND;
}

int GetNightLight() {
	return NightLight;
}

static int SetNightLight(int x) {
	return (NightLight = x);
}

int SetNoReplace(int x) { return (NoReplace = x); }

int GetNoReplace() { return NoReplace; }

int GetPlayerKill() {
	return PlayerKill;
}

int SetPlayerKill(int x) {
	return (PlayerKill = x);
}

int AddPoisonGas(int x) {
	PoisonGas += x;
	return PoisonGas;
}

int GetPoisonGas() {
	int x;

	if( PoisonGas < 1 ) {
		return 0;
	}
	x = time() - GasCheck;
	GasCheck = time();
	if( x > 0 && x < 4 ) {
		x = 1;
	}
	else {
		x = x/4;
	}
	PoisonGas -= x;
	if( PoisonGas < 0 ) {
		PoisonGas = 0;
	}
	return PoisonGas;
}

int SetPoisonGas(int x) {
	return (PoisonGas = x);
}

void AddRead(mixed item, mixed val) {
	if( stringp(item) ) {
		item = ({ item });
	}
	foreach(string tmp in item) {
		foreach(object ob in GetDummyItems()) {
			if( ob->id(tmp) ) {
				ob->SetRead(val);
				break;
			}
		}
	}
}

void RemoveRead(mixed item) {
	foreach(object ob in GetDummyItems()) {
		if( stringp(item) ) {
			if( ob->id(item) ) {
				ob->SetRead(0);
			}
		}
		else if( arrayp(item) ) {
			if( sizeof(ob->GetId() & item) ) {
				ob->SetRead(0);
			}
		}
	}
}

varargs void SetRead(mixed items, mixed arg) {
	if( !mapp(items) ) {
		AddRead(items, arg);
		return;
	}
	foreach(mixed key, mixed val in items) {
		AddRead(key, val);
	}
}

int GetShade() {
	return Shade;
}

static int SetShade(int x) {
	return (Shade = x);
}

varargs void AddSearch(mixed item, mixed val) {
	if( !val ) {
		Search = item;
		return;
	}
	if( !item || item == "default" ) {
		Search = val;
		return;
	}
	if( stringp(item) ) {
		item = ({ item });
	}
	foreach(string tmp in item) {
		foreach(object ob in GetDummyItems()) {
			if( ob->id(tmp) ) {
				ob->SetSearch(val);
				break;
			}
		}
	}
}

varargs mixed GetSearch(string str) {
	return Search;
}

varargs void RemoveSearch(mixed item) {
	if( !item || item == "default" ) {
		Search = 0;
		return;
	}
	foreach(object ob in GetDummyItems()) {
		if( stringp(item) ) {
			if( ob->id(item) ) {
				ob->SetSearch(0);
			}
		}
		else if( arrayp(item) ) {
			if( sizeof(ob->GetId() & item) ) {
				ob->SetSearch(0);
			}
		}
	}
}

varargs void SetSearch(mixed items, mixed arg) {
	if( !mapp(items) ) {
		if( !arg ) {
			AddSearch("default", items);
		}
		else {
			AddSearch(items, arg);
		}
		return;
	}
	foreach(mixed key, mixed val in items) {
		AddSearch(key, val);
	}
}

varargs void AddSmell(mixed item, mixed val) {
	if( !val ) {
		Smell = item;
		return;
	}
	if( !item || item == "default" ) {
		Smell = val;
		return;
	}
	if( stringp(item) ) {
		item = ({ item });
	}
	foreach(string tmp in item) {
		foreach(object ob in GetDummyItems()) {
			if( ob->id(tmp) ) {
				ob->SetSmell(val);
				break;
			}
		}
	}
}

varargs mixed GetSmell(string str) {
	return Smell;
}

varargs void RemoveSmell(mixed item) {
	if( !item || item == "default" ) {
		Smell = 0;
		return;
	}
	foreach(object ob in GetDummyItems()) {
		if( stringp(item) ) {
			if( ob->id(item) ) {
				ob->SetSmell(0);
			}
		}
		else if( arrayp(item) ) {
			if( sizeof(ob->GetId() & item) ) {
				ob->SetSmell(0);
			}
		}
	}
}

varargs void SetSmell(mixed items, mixed arg) {
	if( !mapp(items) ) {
		if( !arg ) {
			AddSmell("default", items);
		}
		else {
			AddSmell(items, arg);
		}
		return;
	}
	foreach(mixed key, mixed val in items) {
		AddSmell(key, val);
	}
}

varargs void AddTouch(mixed item, mixed val) {
	if( !val ) {
		Touch = item;
		return;
	}
	if( !item || item == "default" ) {
		Touch = val;
		return;
	}
	if( stringp(item) ) {
		item = ({ item });
	}
	foreach(string tmp in item) {
		foreach(object ob in GetDummyItems()) {
			if( ob->id(tmp) ) {
				ob->SetTouch(val);
				break;
			}
		}
	}
}

varargs void RemoveTouch(mixed item) {
	if( !item || item == "default" ) {
		Touch = 0;
		return;
	}
	foreach(object ob in GetDummyItems()) {
		if( stringp(item) ) {
			if( ob->id(item) ) {
				ob->SetTouch(0);
			}
		}
		else if( arrayp(item) ) {
			if( sizeof(ob->GetId() & item) ) {
				ob->SetTouch(0);
			}
		}
	}
}

varargs void SetTouch(mixed items, mixed arg) {
	if( !mapp(items) ) {
		if( !arg ) {
			AddTouch("default", items);
		}
		else {
			AddTouch(items, arg);
		}
		return;
	}
	foreach(mixed key, mixed val in items) {
		AddTouch(key, val);
	}
}

string GetTown() { return Town; }

string SetTown(string town) { return (Town = town); }

/** this stuff is for backwards compat **/

mixed SetProperty(string prop, mixed val) {
	if( prop == "light" ) {
		if( !val ) return val;
		if( GetClimate() == "indoors" ) {
			if( val < 0 ) return val;
			else return SetAmbientLight(val*13);
		}
		else {
			if( val < 0 ) return SetShade(-val);
			else return SetDayLight(val);
		}
	}
	else if( prop == "night light" ) {
		if( !val ) return val;
		if( val < 0 ) return val;
		else return SetNightLight(4*val);
	}
	else return properties::SetProperty(prop, val);
}

mixed SetProperties(mapping mp) {
	if( mp["light"] ) {
		SetProperty("light", mp["light"]);
	}
	else if( mp["night light"] ) {
		SetProperty("night light", mp["night light"]);
	}
	return properties::SetProperties(mp);
}


string GetLong() {
	return GetInternalDesc();
}

string SetLong(string str) {
	return SetInternalDesc(str);
}

int CanAttack( object attacker, object who ) {
	if( PlayerKill ) {
		return 1;
	}
	attacker->RemoveHostile( who );
	return 0; 
}

varargs int eventShow(object who, string args) {
	string str;

	if( !(str = (string)SEASONS_D->GetLong(args)) ) {
		who->eventPrint("You do not see that there.");
		return 1;
	} 
	who->eventPrint(str);
	eventPrint(who->GetName() + " looks at the " + args + ".", who);
}

/***********          /lib/room.c events          ***********/
mixed eventBuryItem(object who, object tool, object what) {
	if( !functionp(Bury) ) {
		return "You cannot bury things here!";
	}
	if( functionp(Bury) & FP_OWNER_DESTED ) {
		return "You cannot bury things here.";
	}
	return evaluate(Bury, who, tool, what);
}

varargs mixed eventHearTalk(object who, object target, int cls, string verb,
		string msg, string lang) {
	object *obs;
	string exit, door;

	switch(cls) {
		case TALK_PRIVATE:
			return 1;

		case TALK_SEMI_PRIVATE:
			target->eventHearTalk(who, target, cls, verb, msg, lang);
			eventPrint("%^BOLD%^CYAN%^" + (string)who->GetName() +
					" whispers something to " + (string)target->GetName() + ".",
					MSG_CONV, ({ who, target }));
			return 1;

		case TALK_LOCAL:
			obs = filter(all_inventory(),
					(: (int)$1->is_living() && $1 != $(who) :));
			obs->eventHearTalk(who, target, cls, verb, msg, lang);
			return 1;

		case TALK_AREA:
			foreach(exit in GetExits()) {
				string tmp;

				tmp = GetExit(exit);
				if( !find_object(tmp) ) continue;
				if( (door = GetDoor(exit)) && (int)door->GetClosed() ) continue;
				tmp->eventHearTalk(who, target, TALK_LOCAL, verb, msg, lang);
			}
			foreach(exit in GetEnters()) {
				string tmp;

				tmp = GetEnter(exit);
				if( !find_object(tmp) ) continue;
				if( (door = GetDoor(exit)) && (int)door->GetClosed() ) continue;
				tmp->eventHearTalk(who, target, TALK_LOCAL, verb, msg, lang);
			}
			obs = filter(all_inventory(),
					(: (int)$1->is_living() && $1 != $(who) :));
			obs->eventHearTalk(who, target, cls, verb, msg, lang);
			return 1;

	}
}

int eventMove(mixed dest) { return 0; }

object array MySpecialPurpose(object *ob){
	object *temparr,*stuff,*lstuff;
	int i;
	stuff=all_inventory();
	lstuff = ({});
	for(i=0;i<sizeof(stuff);i++){
		temparr= ({ stuff[i] });
		if(sizeof(temparr) && member_array(stuff[i],ob) != -1 )  stuff[i]=new("/lib/std/dummy");
		if(living(stuff[i]) && !sizeof(lstuff)) lstuff = ({stuff[i]});
		if(living(stuff[i]) && sizeof(lstuff) > 0 && member_array(stuff[i],lstuff) == -1) lstuff += ({stuff[i]});
	}
	if(sizeof(lstuff) > 0)	return lstuff;
	if(!sizeof(lstuff)) return 0;
}


varargs int eventPrint(string msg, mixed arg2, mixed arg3) {
	object *targs;
	int msg_class,i;
	if( !arg2 && !arg3 ) {
		targs=MySpecialPurpose( ({ new("/lib/std/dummy") }) );
		msg_class = MSG_ENV;
	}
	else if( objectp(arg2) || arrayp(arg2) ) {
		if( objectp(arg2) ) arg2 = ({ arg2 });
		targs=MySpecialPurpose(arg2);
		msg_class = MSG_ENV;
	}
	else if( !arg3 ) {
		targs=MySpecialPurpose(({ new("/lib/std/dummy") }));
		msg_class = arg2;
	}
	else if( objectp(arg3) || arrayp(arg3) ) {
		if( objectp(arg3) ) arg3 = ({ arg3 });
		targs=MySpecialPurpose(arg3);
		msg_class = arg2;
	}
	for(i=0; i<sizeof(targs); i++) {
		targs[i]->eventPrint(msg, msg_class);
	}
	return 1;
}

static void create() {
	exits::create();
	reset(query_reset_number());
	if( replaceable(this_object()) && !GetNoReplace() ) {
		string array tmp= inherit_list(this_object());

		if( sizeof(tmp) == 1 ) {
			replace_program(tmp[0]);
		}
	}
}

varargs void reset(int count) {
	inventory::reset(count);
	all_inventory()->reset(count);
	ResetNumber++;
}

int id(string str) {
	return 0;
}

int inventory_accessible() {
	return 1;
}

int inventory_visible() {
	return 1;
}

static void init() {
}

