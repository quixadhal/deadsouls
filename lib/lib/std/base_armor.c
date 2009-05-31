/*    /lib/std/armor.c
 *    From the Dead Souls Mud Library
 *    The standard armor object
 *    Created by Descartes of Borg 950408
 *    Version: @(#) armor.c 1.9@(#)
 *    Last modified: 97/01/01
 */

#include <lib.h>
#include <daemons.h>
#include <function.h>
#include <armor_types.h>
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
inherit LIB_SHOW;
inherit LIB_STEAL;
inherit LIB_VALUE;
inherit LIB_WEAR;
inherit LIB_FALL;
inherit LIB_SINK;
inherit LIB_UNIQUENESS;

private int            Size          = 0;
private int            Fingers       = 5;
private mapping        Protection    = ([]);
private string array   RestrictLimbs = 0;
private string array   BaseLimbs     = 0;
private static mixed   Wear          = 0;
private static mapping MaxProtection = ([]);
private static mixed my_save = ({});

int GetMaxProtection(int type);

private int RetainOnDeath = 0;

int SetRetainOnDeath(int x ){ return (RetainOnDeath = x); }

int GetRetainOnDeath(){ return RetainOnDeath; }

/*  ***************  /lib/armor.c data functions  ***************  */
varargs string GetEquippedDescription(object who){
    if(!who) who = this_player();
    if( GetWorn() ){
        string tmp = "It is worn on ";
        if(who == environment()){
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

varargs string GetExternalDesc(object who){
    string desc;
    desc = ::GetExternalDesc(who);
    if(GetWorn()) desc += " "+GetEquippedDescription(who);
    return desc;
}

string GetEquippedShort(){
    string tmp = GetShort();

    if( GetWorn() ){
        tmp += " (%^GREEN%^worn%^RESET%^)";
    }
    return tmp;
}

int GetFingers(){
    return Fingers;
}

int SetFingers(int x){
    return (Fingers = x);
}

int GetSize(){
    return Size;
}

int SetSize(int x){
    return (Size = x);
}

string *LimbGuess(object who){
    mixed tmp;
    int memb, armor = GetArmorType();
    string limb, *limbs;
    string *base_limbs = this_object()->GetBaseLimbs();

    limbs = ({});
    if(sizeof(base_limbs)){
        tmp = who->GetLimbs();
        if(member_array("hand", base_limbs) != -1 &&
                member_array("arm", base_limbs) != -1){
            string *hands = filter(tmp, (: last($1, 4) == "hand" :) );
            foreach(limb in hands){
                if((memb = member_array(limb[0..(sizeof(limb)-5)]+"arm",
                                tmp)) != -1 ){
                    if(who->CanWear(this_object(),({limb, tmp[memb]})) 
                            == 1){ 
                        limbs += ({limb, tmp[memb]});
                        base_limbs = remove_member(base_limbs,
                                member_array("arm", base_limbs));
                        base_limbs = remove_member(base_limbs,
                                member_array("hand", base_limbs));
                        if(!sizeof(filter(base_limbs, (: $1 == "hand" :))))
                            break;
                    }
                }
            } 
        }
        if(member_array("foot", base_limbs) != -1 &&
                member_array("leg", base_limbs) != -1){
            string *feet = filter(tmp, (: last($1, 4) == "foot" :) );
            foreach(limb in feet){
                if((memb = member_array(limb[0..(sizeof(limb)-5)]+"leg",
                                tmp)) != -1 ){
                    if(who->CanWear(this_object(),({limb, tmp[memb]}))
                            == 1){
                        limbs += ({limb, tmp[memb]});
                        base_limbs = remove_member(base_limbs,
                                member_array("leg", base_limbs));
                        base_limbs = remove_member(base_limbs,
                                member_array("foot", base_limbs));
                        if(!sizeof(filter(base_limbs, (: $1 == "foot" :))))
                            break;
                    }
                }
            }
        }
        if(sizeof(base_limbs)){
            foreach(limb in who->GetLimbs()){
                if(sscanf(reverse_string(limb),"%s %*s",tmp) != 2){
                    tmp = limb;
                }
                else tmp = reverse_string(tmp);
                if((memb = member_array(tmp, base_limbs)) != -1 &&
                        who->CanWear(this_object(), ({ limb })) == 1){
                    base_limbs = remove_member(base_limbs, memb);
                    limbs += ({ limb });
                }
            }
        }
    }
    else {
        foreach(limb in who->GetLimbs()){
            mapping data = who->GetLimb(limb);

            if( data["armors"] & armor ){
                limbs += ({ limb });
            }
        }
    }
    return limbs;
}


/*  Just check the surface for blade/knife/blunt damage
 *  Other damage types will be checked via more costly methods
 *  -blitz
 */
string GetItemCondition(){
    int cuts, dents;
    string ret;
    cuts = dents = -1;

    foreach(int type, int val in Protection){
        int x;
        if( BLADE & type || KNIFE & type ){
            x = to_float(val) / GetMaxProtection(type) * 100;
            if( cuts == -1 || x < cuts ) cuts = x;
        }
        else if( BLUNT & type ){
            x = to_float(val) / GetMaxProtection(type) * 100;
            if( dents == -1 || x > dents ) dents = x;
        }
    }
    if( cuts == -1 && dents == -1 ) return 0;
    if( cuts > -1 ) switch( cuts ){
        case 0..10: ret = "shredded to pieces"; break;
        case 11..20: ret = "shredded"; break;
        case 21..40: ret = "mildly shredded"; break;
        case 41..60: ret = "slashed up"; break;
        case 61..80: ret = "scratched"; break;
        case 81..90: ret = "somewhat nicked"; break;
        default: ret = "unbroken";
    }
    if( dents > -1 ){
        if( ret ) ret += " and ";
        else ret = "";
        switch( dents ){
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

int GetMaxProtection(int type){
    return MaxProtection[type];
}

int GetProtection(int type){
    int array types;
    int i;

    foreach(int t, int val in Protection){
        if( t & type ){
            int blessing = GetProperty("blessed");

            if( !intp(blessing) ){
                blessing = 0;
            }
            return (val + blessing);
        }
    }
    return 0;
}

int SetProtection(int type, int amount){
    MaxProtection[type] = amount;
    return (Protection[type] = amount);
}

int SetAC(int i){
    SetProtection(ALL_EXTERNAL_DAMAGE, i * 5);
    return i;
}

string array GetRestrictLimbs(){
    return RestrictLimbs;
}

string array SetRestrictLimbs(string array limbs){
    return (RestrictLimbs = limbs);
}

string array GetBaseLimbs(){
    return BaseLimbs;
}

string array SetBaseLimbs(string array limbs){
    return (BaseLimbs = limbs);
}

string array GetSave(){
    return persist::GetSave();
}

static mixed array AddSave(mixed array vars){
    if(!vars) vars = ({});
    my_save = distinct_array( my_save + vars );
    return persist::AddSave(my_save);
}

mixed GetWear(){
    return Wear;
}

mixed SetWear(mixed val){
    return (Wear = val);
}

/* ****************** armor.c modals ********************* */ 
mixed CanEquip(object who, string array limbs){
    mixed tmp;

    if(Size && !( Size & who->GetSize() ) ){
        return "It doesn't fit.";
    }

    if( !limbs ){ /* let's try and guess */
        string array guess = who->GetLimbs();
        int armor = GetArmorType();
        string limb;
        if( !guess ){
            return "You have no limbs!";
        }
        limbs = LimbGuess(who);
        if(!limbs){
            foreach(limb in guess){
                mapping data;

                data = who->GetLimb(limb);
                if( data["armors"] & armor ){
                    limbs += ({ limb });
                }
            }
        }
        if( equip::CanEquip(who, limbs) != 1 ){
            string ret = "#Wear " + GetDefiniteShort() + " on which ";
            int type = this_object()->GetArmorType();
            if(type & A_RING) ret += "hand?";
            else ret += "limb?";
            return ret;
        }
        else {
            return 1;
        }
    }
    else if( sizeof(limbs) == 1 ){
        string which;

        switch(GetArmorType()){
            case A_SHIELD:
                if( which = who->GetLimbParent(limbs[0]) ){
                    limbs = ({ limbs[0], "torso", which });
                }
                break;

            case A_LONG_GLOVE: case A_LONG_BOOT:
                if( which = who->GetLimbParent(limbs[0]) ){
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

mixed CanRepair(object who){
    return 1;
}

mixed CanSteal(object who){
    if( GetWorn() ){
        return "You can't steal something equipped!";
    }
    return steal::CanSteal(who);
}

/* ********************* armor.c events *********************** */ 
static int Destruct(){
    if( GetWorn() && environment() ){
        eventUnequip(environment());
    }
    return object::Destruct();
}

void eventDeteriorate(int type){
    foreach(int t, int val in Protection){
        if( (t & type) && val ){
            if( GetProperty("blessed") ){
                Protection[t] -= 2;
            }
            else {
                Protection[t]--;
            }
            if( Protection[t] < 1 ){
                Protection[t] = 0;
            }
        }
    }
    SetDestroyOnSell(1);
    SetValue(GetValue()/2);
}

mixed eventEquip(object who, string array limbs){
    mixed tmp;

    if( !limbs ){ /* let's try and guess */
        limbs = LimbGuess(who);
    }
    else if( sizeof(limbs) == 1 ){
        string which;
        switch(GetArmorType()){
            case A_SHIELD:
                if( which = who->GetLimbParent(limbs[0]) ){
                    limbs = ({ limbs[0], "torso", which });
                }
                break;

            case A_LONG_GLOVE: case A_LONG_BOOT:
                if( which = who->GetLimbParent(limbs[0]) ){
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
    if( functionp(Wear) ){
        if( functionp(Wear) & FP_OWNER_DESTED ){
            return "You can't wear that there at the moment.";
        }
        if( !evaluate(Wear, who, limbs) ){
            return 1;
        }
    }
    tmp = equip::eventEquip(who, limbs);
    if( tmp != 1 ){
        if( tmp ){
            who->eventPrint(tmp);
        }
        else {
            who->eventPrint("You can't wear that there right now.");
        }
        return 1;
    }
    tmp = GetShort();
    SetWorn(limbs);
    if( functionp(Wear) ){
        return 1;
    }
    else if( stringp(Wear) ){
        who->eventPrint(Wear);
    }
    else {
        who->eventPrint("You wear " + tmp + ".");
    }
    environment(who)->eventPrint(who->GetName() + " wears " + tmp + ".", who);
    return 1;
}

int eventMove(mixed dest){
    if( !environment() && GetWorn() ){
        mixed array limbs = GetWorn();

        SetWorn(0);
        call_out((: eventRestoreEquip :), 0, limbs);
    }
    if( GetWorn() && environment() ){
        eventUnequip(environment());
    }
    return move::eventMove(dest);
}

varargs mixed eventRepair(object who, int strength, int type){
    if( !who || !strength ){
        return 0;
    }
    if( !type ){
        type = ALL_DAMAGE;
    }
    foreach(int i in keys(Protection)){
        if( !(i & type) || !MaxProtection[i]){
            continue;
        }
        Protection[i] += strength;
        if( Protection[i] > MaxProtection[i] ){
            Protection[i] = MaxProtection[i];
        }
    }
    return 1;
}

mixed eventShow(object who, string component){
    mixed tmp = object::eventShow(who, component);

    if( component || tmp != 1 ){
        return tmp;
    }
    if( GetPoison() ){
        if( random(100) < who->GetSkillLevel("stealth") ){
            who->eventPrint("You notice a strange substance on it.");
        }
    }
    return 1;
}

varargs mixed eventUnequip(object who){
    mixed tmp = equip::eventUnequip(who);

    if( tmp != 1 ){
        return tmp;
    }
    if(!who->GetDead()) send_messages("remove", "$agent_name $agent_verb $target_name.",
            who, this_object(), environment(who));
    return 1;
}

varargs int restrict(mixed arg, int i){
    if(!arg) return 1;
    if(arg && i) SetBaseLimbs(arg);
    else if(arg) SetRestrictLimbs(arg);
    return 2;
}

/* ******************** armor.c driver applies ******************** */
static void create(){
    steal::create();
    object::create();
    my_save = equip::GetSave() + value::GetSave() + mass::GetSave() +
        poison::GetSave() + deterioration::GetSave();
    persist::AddSave(my_save);
    SetVendorType(VT_ARMOR);
}

void init(){
    int atype;
    atype = GetArmorType();
    if(atype & A_RING ) restrict(({"hand"}),1);
    if(atype & A_SHIELD ) restrict(({"hand","arm","torso"}),1);
    if(atype & A_LONG_GLOVE ) restrict(({"hand","arm"}),1);
    if(atype & A_LONG_SOCK ) restrict(({"foot","leg"}),1);
    if(atype & A_LONG_BOOT ) restrict(({"foot","leg"}),1);
    if(atype & A_SOCK ) restrict(({"foot"}),1);
    if(atype & A_BOOT ) restrict(({"foot"}),1);
    if(atype & A_GLOVE ) restrict(({"hand"}),1);
    if(atype & A_HELMET ) restrict(({"head"}));
    if(atype & A_VISOR ) restrict(({"head"}));
    if(atype & A_PANTS ) restrict(({"right leg","left leg"}));
    if(atype & A_BELT ) restrict(({"torso"}));
    if(atype & A_SHIRT ) restrict(({"torso"}));
    if(atype & A_CLOAK ) restrict(({"torso"}));
    if(atype & A_VEST ) restrict(({"torso"}));
    if(atype & A_ARMOR ) restrict(({"torso","right arm","left arm"}));
    if(atype & A_COLLAR ) restrict(({"neck"}));
    if(atype & A_BODY_ARMOR ) restrict(({"torso","right arm","left arm","left leg","right leg"}) );
} 

mapping GetProtectionMap(){
    return copy(Protection);
}
