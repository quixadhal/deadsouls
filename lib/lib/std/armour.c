/*    /lib/std/armour.c
 *    From the Dead Souls Object Library
 *    The standard armour object
 *    Created by Descartes of Borg 950408
 *    Version: @(#) armour.c 1.9@(#)
 *    Last modified: 97/01/01
 */
  
#include <lib.h>
#include <function.h>
#include <armour_types.h>
#include <damage_types.h>
#include <vendor_types.h>
 
inherit LIB_BURY;
inherit LIB_DETECT;
inherit LIB_DETERIORATION;
inherit LIB_DROP;
inherit LIB_EQUIP;
inherit LIB_GET;
inherit LIB_GIVE;
inherit LIB_LOCK_WITH;
inherit LIB_MASS;
inherit LIB_MOVE;
inherit LIB_OBJECT;
inherit LIB_PERSIST;
inherit LIB_POISON;
inherit LIB_PUT;
inherit LIB_READ;
inherit LIB_SELL;
inherit LIB_STEAL;
inherit LIB_UNIQUENESS;
inherit LIB_VALUE;
inherit LIB_WEAR;

private int            Fingers       = 5;
private mapping        Protection    = ([]);
private string array   RestrictLimbs = 0;
private static mixed   Wear          = 0;
private static mapping MaxProtection = ([]);

int GetMaxProtection(int type);

private int RetainOnDeath = 0;

int SetRetainOnDeath(int x ) { return (RetainOnDeath = x); }

int GetRetainOnDeath() { return RetainOnDeath; }

/*  ***************  /lib/armour.c data functions  ***************  */
varargs string GetEquippedDescription(object who) {
    if( GetWorn() ) {
	string tmp = "It is worn on ";

	if( !who ) {
	    who = this_player();
	}
	if( who == environment()) {
	    tmp += "your";
	}
	else {
	    tmp += possessive_noun(environment());
	}
	tmp += " " + item_list(GetWorn()) + ".";
	return tmp;
    }
    return 0;
}

string GetEquippedShort() {
    string tmp = GetShort();
 
    if( GetWorn() ) {
	tmp += " (worn)";
    }
    return tmp;
}

int GetFingers() {
    return Fingers;
}

int SetFingers(int x) {
    return (Fingers = x);
}
 
/*  Just check the surface for blade/knife/blunt damage
 *  Other damage types will be checked via more costly methods
 *  -blitz
 */
string GetItemCondition() {
    int cuts, dents;
    string ret;
    cuts = dents = -1;

    foreach(int type, int val in Protection) {
        int x;
        if( BLADE & type || KNIFE & type ) {
            x = to_float(val) / GetMaxProtection(type) * 100;
            if( cuts == -1 || x < cuts ) cuts = x;
        }
        else if( BLUNT & type ) {
            x = to_float(val) / GetMaxProtection(type) * 100;
            if( dents == -1 || x > dents ) dents = x;
        }
    }
    if( cuts == -1 && dents == -1 ) return 0;
    if( cuts > -1 ) switch( cuts ) {
        case 0..10: ret = "shredded to pieces"; break;
        case 11..20: ret = "shredded"; break;
        case 21..40: ret = "mildly shredded"; break;
        case 41..60: ret = "slashed up"; break;
        case 61..80: ret = "scratched"; break;
        case 81..90: ret = "somewhat nicked"; break;
        default: ret = "unbroken";
    }
    if( dents > -1 ) {
        if( ret ) ret += " and ";
        else ret = "";
        switch( dents ) {
            case 0..10: ret += "utterly battered"; break;
            case 11..20: ret += "terribly pounded"; break;
            case 21..40: ret += "serverly dented"; break;
            case 41..60: ret += "pretty dented"; break;
            case 61..80: ret += "dented"; break;
            case 81..90: ret += "slightly dented"; break;
            default: ret += "unmarred";
        }
    }
    return "Its surface is " + ret + ".";
}
 
int GetMaxProtection(int type) {
    return MaxProtection[type];
}
 
int GetProtection(int type) {
    int array types;
    int i;

    foreach(int t, int val in Protection) {
	if( t & type ) {
	    int blessing = GetProperty("blessed");

	    if( !intp(blessing) ) {
		blessing = 0;
	    }
	    return (val + blessing);
	}
    }
    return 0;
}
 
int SetProtection(int type, int amount) {
    MaxProtection[type] = amount;
    return (Protection[type] = amount);
}
 
string array GetRestrictLimbs() {
    return RestrictLimbs;
}
 
string array SetRestrictLimbs(string array limbs) {
    return (RestrictLimbs = limbs);
}
 
static mixed array AddSave(mixed array vars) {
    return persist::AddSave(vars);
}

string array GetSave() {
    return persist::GetSave();
}

mixed GetWear() {
    return Wear;
}
 
mixed SetWear(mixed val) {
    return (Wear = val);
}
 
/* ****************** armour.c modals ********************* */ 
mixed CanEquip(object who, string array limbs) {
    mixed tmp;
    
    if( !limbs ) { /* let's try and guess */
	string array guess = who->GetLimbs();
	int armour = GetArmourType();
        string limb;

	if( !guess ) {
	    return "You have no limbs!";
	}
        limbs = ({});
        foreach(limb in guess) {
            mapping data;
 
            data = who->GetLimb(limb);
            if( data["armours"] & armour ) {
		limbs += ({ limb });
	    }
        }
	if( equip::CanEquip(who, limbs) != 1 ) {
	    return "Wear " + GetDefiniteShort() + " on which limb?";
	}
        else {
	    return 1;
	}
    }
    else if( sizeof(limbs) == 1 ) {
        string which;
 
        switch(GetArmourType()) {
            case A_SHIELD:
		if( which = who->GetParentLimb(limbs[0]) ) {
		    limbs = ({ limbs[0], "torso", which });
		}
		break;
 
            case A_LONG_GLOVE: case A_LONG_BOOT:
		if( which = who->GetParentLimb(limbs[0]) ) {
		    limbs = ({ limbs[0], which });
		}
		else {
		    limbs = ({ limbs[0] });
		}
		break;
 
            default:
		limbs = ({ limbs[0] });
		break;
        }
    }
    return equip::CanEquip(who, limbs);
}
 
mixed CanRepair(object who) {
    return 1;
}
 
mixed CanSteal(object who) {
    if( GetWorn() ) {
	return "You can't steal something equipped!";
    }
    return steal::CanSteal(who);
}
 
/* ********************* armour.c events *********************** */ 
static int Destruct() {
    if( GetWorn() && environment() ) {
	eventUnequip(environment());
    }
    return object::Destruct();
}
 
void eventDeteriorate(int type) {
    foreach(int t, int val in Protection) {
	if( (t & type) && val ) {
	    if( GetProperty("blessed") ) {
		Protection[t] -= 2;
	    }
	    else {
		Protection[t]--;
	    }
	    if( Protection[t] < 1 ) {
		Protection[t] = 0;
	    }
	}
    }
    SetDestroyOnSell(1);
    SetValue(GetValue()/2);
}
 
mixed eventEquip(object who, string array limbs) {
    mixed tmp;
 
    if( !limbs ) { /* let's try and guess */
	int armour = GetArmourType();
        string limb;
 
        limbs = ({});
        foreach(limb in who->GetLimbs()) {
            mapping data = who->GetLimb(limb);
 
            if( data["armours"] & armour ) {
		limbs += ({ limb });
	    }
        }
	if( equip::CanEquip(who, limbs) != 1 ) {
	    return "Wear " + GetDefiniteShort() + " on which limb?";
	}
    }
    else if( sizeof(limbs) == 1 ) {
        string which;
 
        switch(GetArmourType()) {
            case A_SHIELD:
		if( which = who->GetParentLimb(limbs[0]) ) {
		    limbs = ({ limbs[0], "torso", which });
		}
		break;
 
            case A_LONG_GLOVE: case A_LONG_BOOT:
		if( which = who->GetParentLimb(limbs[0]) ) {
		    limbs = ({ limbs[0], which });
		}
		else {
		    limbs = ({ limbs[0] });
		}
		break;
 
	    default:
		limbs = ({ limbs[0] });
		break;
        }
    }
    if( functionp(Wear) ) {
	if( functionp(Wear) & FP_OWNER_DESTED ) {
	    return "Error in evaluating function pointer.";
	}
	if( !evaluate(Wear, who, limbs) ) {
	    return 1;
	}
    }
    tmp = equip::eventEquip(who, limbs);
    if( tmp != 1 ) {
	if( tmp ) {
	    who->eventPrint(tmp);
	}
	else {
	    who->eventPrint("Error in wearing armour.");
	}
	return 1;
    }
    tmp = GetShort();
    SetWorn(limbs);
    if( functionp(Wear) ) {
	return 1;
    }
    else if( stringp(Wear) ) {
	who->eventPrint(Wear);
    }
    else {
	who->eventPrint("You wear " + tmp + ".");
    }
    environment(who)->eventPrint(who->GetName() + " wears " + tmp + ".", who);
    return 1;
}
 
int eventMove(mixed dest) {
    if( !environment() && GetWorn() ) {
	mixed array limbs = GetWorn();

	SetWorn(0);
	call_out((: eventRestoreEquip :), 0, limbs);
    }
    if( GetWorn() && environment() ) {
	eventUnequip(environment());
    }
    return move::eventMove(dest);
}
 
int eventReceiveDamage(object agent, int type, int amt, int i, mixed array l) {
    int x = -1;

    foreach(int t, int val in Protection) {
	if( t & type ) {
	    if( x == -1 || val < x ) {
		x = val;
	    }
	}
    }
    x = x/2 + random(x/2);
    x = deterioration::eventReceiveDamage(agent, type, x, i, l);
    return x;
}
 
varargs mixed eventRepair(object who, int strength, int type) {
    if( !who || !strength ) {
	return 0;
    }
    if( !type ) {
	type = ALL_DAMAGE;
    }
    foreach(int i in keys(Protection)) {
        if( !(i & type) || !MaxProtection[i]) {
	    continue;
	}
        Protection[i] += strength;
        if( Protection[i] > MaxProtection[i] ) {
	    Protection[i] = MaxProtection[i];
	}
    }
    return 1;
}
 
mixed eventShow(object who, string component) {
    mixed tmp = object::eventShow(who, component);

    if( component || tmp != 1 ) {
	return tmp;
    }
    if( GetPoison() ) {
        if( random(100) < who->GetSkillLevel("stealth") ) {
            who->eventPrint("You notice a strange substance on it.");
	}
    }
    return 1;
}

varargs mixed eventUnequip(object who) {
    mixed tmp = equip::eventUnequip(who);
 
    if( tmp != 1 ) {
	return tmp;
    }
    send_messages("remove", "$agent_name $agent_verb $target_name.",
		  who, this_object(), environment(who));
    return 1;
}
 
/* ******************** armour.c driver applies ******************** */
static void create() {
    AddSave(equip::GetSave() + value::GetSave() + mass::GetSave() +
	    poison::GetSave() + deterioration::GetSave());
    steal::create();
    object::create();
    SetVendorType(VT_ARMOUR);
}

void init() { }
