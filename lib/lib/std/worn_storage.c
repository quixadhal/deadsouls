/*    /lib/std/armor.c
 *    From the Dead Souls Object Library
 *    The standard armor object
 *    Created by Descartes of Borg 950408
 *    Version: @(#) armor.c 1.9@(#)
 *    Last modified: 97/01/01
 */

#include <lib.h>
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
inherit LIB_UNIQUENESS;
inherit LIB_VALUE;
inherit LIB_WEAR;
inherit LIB_HOLDER;
inherit LIB_SEAL;
inherit LIB_MONEY;
inherit LIB_FALL;
inherit LIB_SINK;

private int            	Fingers       	= 5;
private int            	Size       	= 0;
private mapping        	Protection    	= ([]);
private string array   	RestrictLimbs 	= 0;
private static mixed   	Wear          	= 0;
private static mapping 	MaxProtection 	= ([]);
private int		CanClose	= 0;
private int 		CanLock  	= 0;
private int		MaxRecurseDepth	= 3;
private int		RecurseDepth  	= 1;
private int 		RetainOnDeath 	= 0;

void PutCheck();

int GetMaxProtection(int type);

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

string array GetRestrictLimbs(){
    return RestrictLimbs;
}

string array SetRestrictLimbs(string array limbs){
    return (RestrictLimbs = limbs);
}

static mixed array AddSave(mixed array vars){
    if(!vars) vars = ({});
    vars += ({ "Properties" });
    return persist::AddSave(vars);
}

string array GetSave(){
    return persist::GetSave();
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
        limbs = ({});
        foreach(limb in guess){
            mapping data;

            data = who->GetLimb(limb);
            if( data["armors"] & armor ){
                limbs += ({ limb });
            }
        }
        if( equip::CanEquip(who, limbs) != 1 ){
            string ret = "Wear " + GetDefiniteShort() + " on which ";
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
        int armor = GetArmorType();
        string limb;

        limbs = ({});
        foreach(limb in who->GetLimbs()){
            mapping data = who->GetLimb(limb);

            if( data["armors"] & armor ){
                limbs += ({ limb });
            }
        }
        if( equip::CanEquip(who, limbs) != 1 ){
            string ret = "Wear " + GetDefiniteShort() + " on which ";
            int type = this_object()->GetArmorType();
            if(type & A_RING) ret += "hand?";
            else ret += "limb?";
            return ret;
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
    send_messages("remove", "$agent_name $agent_verb $target_name.",
      who, this_object(), environment(who));
    return 1;
}

int restrict(mixed arg){
    if(!arg) return 1;
    if(arg) SetRestrictLimbs(arg);
    return 2;
}

/* ******************** armor.c driver applies ******************** */
static void create(){
    AddSave( ({ "Closed", "CanClose", "CanLock", "RecurseDepth" , "MaxRecurseDepth" }) +
      (equip::GetSave()) + (value::GetSave()) + (mass::GetSave()) +
      (poison::GetSave()) + (deterioration::GetSave()));
    steal::create();
    object::create();
    holder::create();
    seal::create();
    PutCheck();
    SetVendorType(VT_ARMOR);
}

void init(){
    int atype;
    atype = GetArmorType();
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

int GetMaxRecurseDepth(){
    return MaxRecurseDepth;
}

int GetRecurseDepth(){
    return RecurseDepth;
}

int SetMaxRecurseDepth(int i){
    MaxRecurseDepth =i;
    return 1;
}

int SetRecurseDepth(int i){
    RecurseDepth=i;
    return 1;
}

int AddRecurseDepth(int i){
    RecurseDepth += i;
    return 1;
}

int GetCanClose(){
    return CanClose;
}

int SetCanClose(int x){
    CanClose = x;
}

int GetCanLock(){
    return CanLock;
}

int SetCanLock(int x){
    return SetCanClose(CanLock = x);
}

void SetKey(string key){ 
    SetKeys(key);
} 

int GetOpacity(){
    return holder::GetOpacity();
}

int GetRadiantLight(int ambient){
    return (object::GetRadiantLight(ambient)) + (holder::GetRadiantLight(ambient));
}

int SetSaveRecurse(int x){
    return persist::SetSaveRecurse(x);
}

mixed CanClose(object who, string id){
    if( !GetCanClose() ){
        return 0;
    }
    else {
        return seal::CanClose(who, id);
    }
}

mixed CanGetFrom(object who, object item){
    mixed tmp = holder::CanGetFrom(who, item);

    if( tmp != 1 ){
        return tmp;
    }
    if( GetClosed() ){
        return capitalize(GetDefiniteShort()) + " is closed.";
    }
    return 1;
}

mixed CanLock(object who, string id){
    if( !GetCanLock() ){
        return 0;
    }
    else {
        return seal::CanLock(who, id);
    }
}

mixed CanOpen(object who, string id){
    if( !GetCanClose() ){
        return 0;
    }
    else {
        return seal::CanOpen(who, id);
    }
}

mixed CanPick(object who, string id){
    if( !GetCanLock() ){
        return "It isn't lockable in the first place.";
    }
    else {
        return seal::CanPick(who, id);
    }
}

mixed CanPutInto(object who, object what){
    mixed tmp;
    int mydepth,yourdepth,total,indirectp,indirectpp;
    string wherefrom,stackstring;
    string *callstack;

    if(!tmp = holder::CanPutInto(who, what)){
        if(GetClosed()) return capitalize(GetDefiniteShort()) + " is closed right now.";
        else return "You can't do that right now.";
    }

    wherefrom=origin();
    callstack=call_stack(2);
    stackstring=implode(callstack," ");
    indirectp=member_array("indirect_put_obj_word_obj",callstack);
    indirectpp=member_array("indirect_put_objs_word_obj",callstack);


    if( tmp != 1 ){
        if( GetClosed() ) return capitalize(GetDefiniteShort()) + " is closed.";
        else return "You can't do that at this time.";
    }
    if( GetClosed() ){
        return capitalize(GetDefiniteShort()) + " is closed.";
    }

    if(inherits("/lib/std/storage",what) ){
        yourdepth = what->GetRecurseDepth();
        mydepth = this_object()->GetRecurseDepth();
        if(yourdepth && mydepth) total = yourdepth + mydepth;
        if(total && total > this_object()->GetMaxRecurseDepth()) return "Doesn't fit.";
    }

    return 1;
}

varargs mixed CanShowInterior(object who, object target){
    if( GetClosed() && this_object()->GetOpacity() > 33){
        return capitalize(GetDefiniteShort()) + " is closed.";
    }
    else return holder::CanShowInterior();
}

mixed CanUnlock(object who, string id, object key){
    if( !GetCanLock() ){
        return 0;
    }
    else {
        return seal::CanUnlock(who, id, key);
    }
}

int eventReceiveObject(object ob){
    if( GetClosed() ){
        return 0;
    }

    return holder::eventReceiveObject(ob);
}

void PutCheck(){

    if(RecurseDepth >= MaxRecurseDepth){
        SetPreventPut("You have enough containers inside containers there. This one will have to stay out.");
    }


}

int inventory_accessible(){
    return seal::inventory_accessible();
}

int inventory_visible(){
    return (seal::inventory_visible() || holder::inventory_visible());
}

mapping GetProtectionMap(){
    return copy(Protection);
}
