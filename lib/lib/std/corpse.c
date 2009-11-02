/*    /lib/corpse.c
 *    from the Dead Souls LPC Library
 *    standard corpse object
 *    created by Descartes of Borg 960711
 *    Version: @(#) corpse.c 1.10@(#)
 *    Last Modified: 96/12/31
 */

#include <lib.h>
#include <medium.h>
#include <respiration_types.h>
#include <message_class.h>

inherit LIB_SURFACE;

int Count, CallOut, Fresh;
string Owner, Race;
mapping Stats, Skills;
string BaseFile, Class, Gender, LivingShort, LivingLong;
int Player, Level, slowdecay, nodecay;
string array MissingLimbs;
object Playerob;
mapping Equipped = ([]);

mixed direct_resurrect_obj(){ return 1; }
mixed indirect_resurrect_obj(){ return 1; }

int eventDecay(){
    int smell, medium, rtype;
    if(nodecay) return 0;
    if( !room_environment() ){
        Destruct();
        return 0;
    }
    medium = environment()->GetMedium();
    rtype = environment()->GetRespirationType();
    Fresh = 0;
    if(rtype & R_AIR) smell = 1;
    if(medium == MEDIUM_SPACE && random(100) < 90) return 0;
    switch(Count){
        case 10:
            if(smell){
                environment()->eventPrint(possessive_noun(Owner) + " corpse " +
                        "is starting to stink.", MSG_ROOMDESC);
                SetId(GetId()..., "corpse", "remains","flesh","pile","pile of flesh");
                SetAdjectives(GetAdjectives()..., "stinky", "rotting");
                SetShort("the stinky remains of a rotting corpse");
                SetSmell("This corpse is beginning to stink up the entire area.");
            }
            break;
        case 20:
            if(smell){
                environment()->eventPrint("A rotting stench fills the entire "
                        "area.", MSG_ROOMDESC);
                SetId(GetId()..., "flesh", "pile", "pile of flesh");
                SetShort("a pile of rotting flesh");
                SetSmell("Its smell is nearly unbearable.");
            }
            break;
        case 30:
            Destruct();
            return 0;
    }
    if(slowdecay){
        if(random(100) > slowdecay) Count++;
    }
    else Count++;
    return Count;
}

static int Destruct(){
    return ::Destruct();
}

void SetCorpse(object who){
    object *worn = ({});
    string tmpshort = who->GetShort();
    if(sizeof(who->GetRiders())) tmpshort = who->GetPlainShort();
    SetKeyName(who->GetKeyName());
    SetId(({ "body","corpse",who->GetId()... }) );
    Owner = who->GetCapName();
    Race = who->GetRace();
    Count = 1;
    Fresh = 1;
    SetShort("the corpse of "+tmpshort);
    SetLong("As you look closely at " + who->GetCapName() +
            ", you notice that " +  nominative(who) +
            " does not appear to be moving.");
    Skills = who->GetSkillsMap();
    Stats = who->GetStatsMap();
    Level = who->GetLevel();
    Gender = who->GetGender();
    Class = who->GetClass();
    MissingLimbs = who->GetMissingLimbs();
    Player = interactive(who);
    Playerob = who;
    BaseFile = base_name(who)+".c";
    if(who->GetProperty("basefile") && !strsrch(BaseFile, LIB_SENTIENT)){
        BaseFile = who->GetProperty("basefile");
    }
    LivingLong = who->GetLong();
    LivingShort = who->GetShort();
    SetMaxCarry(who->GetMaxCarry());
    worn = who->GetWorn();
    worn += who->GetWielded();
    foreach(mixed thing in worn){
        if(arrayp(thing) && sizeof(thing)) thing = thing[0];
        if(!Equipped) Equipped = ([]);
        if(!thing || !objectp(thing)) continue;
        if(Equipped[file_name(thing)]) continue;
        Equipped[file_name(thing)] = 
            ([ "object" : thing, "where" : thing->GetWorn() ]); 
    }
    if(Player){
        SetNoClean(1);
        slowdecay = 50; 
    }
}

int isPlayer(){
    return Player;
}

int isCorpse(){
    return 1;
}

int isFreshCorpse(){
    return Fresh;
}

string GetHealthShort(){
    return "%^RED%^" + GetShort();
}

string GetOwner(){ return Owner; }

string GetRace(){ return Race; }

string GetSaveString(){ return 0; }

int CanReceive(object ob){ return 1; }


static void create(){
    ::create();
    SetId( ({ "corpse", "flesh", "remains" }) );
    SetAdjectives( ({"pile of", "rotting", "stinky"}) );
    Count = 0;
    CallOut = 0;
    Owner = 0;
    Race = 0;
    SetNoCondition(1);
}

int direct_animate_obj(){
    return 1;
}

int direct_offer_obj(){
    return 1;
}

string GetItemCondition(){ return "";}

mapping SetStats(mapping stats){
    Stats = stats;
    return Stats;
}

mapping GetStats(){
    return Stats;
}

mapping SetSkills(mapping skills){
    Skills = skills;
    return Skills;
}

mapping GetSkills(){
    return Skills;
}

string SetGender(string gender){
    return Gender = gender;
}

string GetGender(){
    return Gender;
}

int SetLevel(int level){
    return Level = level;
}

int GetLevel(){
    return Level;
}

string SetClass(string the_class){
    return Class = the_class;
}

string GetClass(){
    return Class;
}

string array SetMissingLimbs(string *limbs){
    return MissingLimbs = limbs;
}

string array GetMissingLimbs(){
    return MissingLimbs;
}

object GetPlayerob(){
    return Playerob;
}

string GetCapName(){
    return Owner;
}

string SetBaseFile(string file){
    return BaseFile = file;
}

string GetBaseFile(){
    return BaseFile;
}

string SetLivingShort(string short){
    return LivingShort = short;
}

string GetLivingShort(){
    return LivingShort;
}

string SetLivingLong(string long){
    return LivingLong = long;
}

string GetLivingLong(){
    return LivingLong;
}

int SetNoDecay(int i){
    if(i) nodecay = 1;
    else nodecay = 0;
    return nodecay;
}

int GetNoDecay(){
    return nodecay;
}

int SetSlowDecay(int i){
    if(i) slowdecay = i;
    else slowdecay = 0;
    return slowdecay;
}

int GetSlowDecay(){
    return slowdecay;
}

int SetCount(int i){
    Count = i;
    return Count;
}

int GetCount(int i){
    return Count;
}

mapping GetEquipped(){
    return copy(Equipped);
}
