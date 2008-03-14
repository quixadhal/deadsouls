#include <lib.h>
#include <dirs.h>
#include <vision.h>
#include <medium.h>
#include <terrain_types.h>
#include <damage_types.h>
#include <respiration_types.h>

inherit LIB_SHADOW;
inherit LIB_ID;

object me = this_object();
object suit;
int disguised, reporting = 1;

int SetDisguised(int i){
    if(i) disguised = 1;
    else disguised = 0;
    return disguised;
}

int GetDisguised(){
    return disguised;
}

int CheckDisguised(){
    object ob = GetShadowedObject();
    if(!ob || !disguised) return 0;
    else return 1;
}

int CheckSuit(){
    if(!suit){
        this_object()->unscoutshadow();
        return 0;
    }
    return 1;
}

varargs string GetExternalDesc(object who){
    object ob = GetShadowedObject();
    if(CheckDisguised()) return "A scout of the Extant Authority military forces.";
    else return ob->GetExternalDesc(who);
}

varargs string GetLong(object who){
    object ob = GetShadowedObject();
    if(CheckDisguised()) return "A scout of the Extant Authority military forces.";
    else return ob->GetLong(who);
}

string GetName(){
    object ob = GetShadowedObject();
    if(CheckDisguised()) return "Scout";
    else return ob->GetName();
}

string GetCapName(){
    object ob = GetShadowedObject();
    if(CheckDisguised()) return "Scout";
    else return ob->GetCapName();
}

string GetKeyName(){
    object ob = GetShadowedObject();
    if(CheckDisguised()) return "scout";
    else return ob->GetKeyName();
}

string GetShort(){
    object ob = GetShadowedObject();
    if(CheckDisguised()) return "a scout";
    else return ob->GetShort();
}

string GetRace(){
    object ob = GetShadowedObject();
    if(CheckDisguised()) return "human";
    else return ob->GetRace();
}

int eventShadow(object whom){
    object *shadows = keys(whom->GetShadows());
    if(sizeof(shadows)){
        foreach(object shade in shadows){
            if(base_name(shade) == base_name(me)) return 0;
        }
    }
    if(base_name(previous_object()) == "/domains/town/armor/scoutsuit"
      || base_name(previous_object()) == "/domains/default/armor/scoutsuit"){
        ::eventShadow(whom);
        suit = previous_object();
        if(CheckDisguised()) whom->SetId(({"scout"}));
        if(CheckDisguised()) whom->cmdParseRefresh();
        return 1;
    }
    return 0;
}

int unscoutshadow(){
    object ob = GetShadowedObject();
    if(!ob) return 0;
    ob->SetId();
    ob->cmdParseRefresh();
    eventUnshadow();
}

string array parse_command_id_list() {
    string array ids = (this_object()->GetId() + 
      (CheckDisguised() ? ({ "scout" }) : ({}) ) || ({}));
    return filter(ids, (: stringp($1) && ($1 != "") :));

}

string array parse_command_plural_id_list() {
    string array ids = (this_object()->GetId() +
      (CheckDisguised() ? ({ "scout" }) : ({}) ) || ({}));
    ids = filter(ids, (: stringp($1) && ($1 != "") :));
    return map(ids, (: pluralize :));
}

int GetSkillLevel(string skill) {
    object ob = GetShadowedObject();
    int ret;
    if(!CheckSuit()) return 0;
    if(!ob) return 0;
    if(!suit->GetActive()) return ob->GetSkillLevel(skill);
    switch(skill){
    case "melee attack" : ret = 50;break;
    default :
    }
    if(ret){
        suit->eventDecrementCharge();
        return ret;
    }
    else return ob->GetSkillLevel(skill);
}

int GetCanBite(){
    return 0;
}

int GetStatLevel(string stat){
    object ob = GetShadowedObject();
    int ret;
    if(!CheckSuit()) return 0;
    if(!ob) return 0;
    if(!suit->GetActive()) return ob->GetStatLevel(stat);
    switch(stat){
    case "strength" : ret = 40;break;
    case "agility" : ret = 50;break;
    case "durability" : ret = 60;break;
    case "coordination" : ret = 70;break;
    default :
    }
    if(ret){
        suit->eventDecrementCharge();
        return ret;
    }
    else return ob->GetStatLevel(stat);
}

int GetMelee(){
    object ob = GetShadowedObject();
    if(!CheckSuit()) return 0;
    if(!ob) return 0;
    if(!suit->GetActive()) return ob->GetMelee();
    if(!(ob->GetMelee())) suit->eventDecrementCharge();
    return 1;
} 

float AddStaminaPoints(mixed x) {
    float y;
    object ob = GetShadowedObject();
    if(!CheckSuit()) return 0;
    if(!ob) return 0;
    if(!suit->GetActive()) return ob->AddStaminaPoints(x);

    if( !intp(x) && !floatp(x) )
        error("Bad argument 1 to AddStaminaPoints().\n");
    x = abs(x);
    suit->eventDecrementCharge(x);
    return suit->GetRemainingCharge();
}

//This is a bit much
#if 0
int eventCollapse(){
    object ob = GetShadowedObject();
    if(!ob) return 0;
    if(!CheckSuit() || !suit->GetActive()) return ob->eventCollapse(); 
    suit->eventDecrementCharge();
    return suit->GetRemainingCharge();
}
#endif

varargs mixed GetEffectiveVision(mixed location, int raw_score){
    object ob = GetShadowedObject();
    if(!CheckSuit()) return 0;
    if(!ob) return 0;
    if(suit->GetActive()){
        suit->eventDecrementCharge();
        return VISION_CLEAR;
    }
    return ob->GetEffectiveVision(location, raw_score);
}

varargs int CanBreathe(mixed args...){
    object ob = GetShadowedObject();
    int rtype;
    if(!ob) return 0;
    if(!CheckSuit()) return ob->CanBreathe(args);
    rtype = ob->GetRespiration();
    if(suit->GetActive()){
        if(rtype & R_AIR){
            suit->eventDecrementCharge();
            return 1;
        }
        if(rtype & R_WATER || rtype & R_METHANE) return 0;
    }
    return ob->CanBreathe(args);
}

void eventDescribeEnvironment(int verbose) {
    object ob = GetShadowedObject();
    object env;
    string filename,foo,tmp;
    int x,y,z,hud = 1;
    object *livings; 
    string extra = "%^CYAN%^Heads-up display info:%^RESET%^\n";
    mapping invisibles = ([]);
    int medium, terrain;

    extra += "Power remaining: "+to_int(percent(suit->GetRemainingCharge(),
        suit->GetMaxCharge()))+"%\n";

    if(!ob) return 0;
    if(!CheckSuit() || !suit->GetActive()) return ob->eventDescribeEnvironment(verbose);

    if( !(env = environment(ob)) ) {
        message("room_description", "No environment.", this_object());
        return;
    }

    filename = file_name(env);
    livings = filter(get_livings(env), (: $1->GetInvis() :) );
    medium = env->GetMedium();
    terrain = env->GetTerrain();

    foo = last_string_element(filename,"/");
    if(sscanf(foo,"%d,%d,%d", x,y,z) == 3 ||
      sscanf(foo,"%d,%d", x,y) == 2){
        hud = 1;
        extra += "Grid coordinates: "+x+","+y+","+z+"\n";
    }

    if(terrain){
        hud = 1;
        extra += "Terrain: ";
        if(terrain & T_OUTDOORS) extra += " outdoors ";
        if(terrain & T_INDOORS) extra += " indoors ";
        if(terrain & T_ROAD) extra += " road ";
        if(terrain & T_UNDERWATER) extra += " underwater ";
        if(terrain & T_SURFACE) extra += "surface ";
        if(terrain & T_MIDAIR) extra += " midair ";
        if(terrain & T_SWAMP) extra += " swamp ";
        if(terrain & T_WOODS) extra += " woods ";
        if(terrain & T_JUNGLE) extra += " jungle ";
        if(terrain & T_ROUGH) extra += " rough ";
        if(terrain & T_UNDERGROUND) extra += " underground ";
        if(terrain & T_SPACE) extra += " space ";
        if(terrain & T_MAGMA) extra += " magma ";
        if(terrain & T_PLASMA) extra += " plasma ";
        if(terrain & T_PLANAR) extra += " planar ";
        if(terrain & T_SNOW) extra += " snow ";
        if(terrain & T_SAND) extra += " sand ";
        if(terrain & T_ICE) extra += " ice ";
        if(terrain & T_BIOLOGICAL) extra += " biological ";
        if(terrain & T_SEAFLOOR) extra += " seafloor ";
        extra += "\n";
    }

    if(medium){
        hud = 1;
        extra += "Medium: ";
        switch(medium){
        case MEDIUM_LAND : extra += "land\n"; break;
        case MEDIUM_AIR : extra += "air\n"; break;
        case MEDIUM_SPACE : extra += "space\n"; break;
        case MEDIUM_WATER : extra += "water\n"; break;
        case MEDIUM_SURFACE : extra += "surface\n"; break;
        default: extra += "unknown\n";
        }
    }

    if(sizeof(livings)){
        hud = 1;
        foreach(object dude in livings){
            if(!invisibles[dude->GetRace()]) invisibles[dude->GetRace()] = 1;
            else invisibles[dude->GetRace()]++;
        }
        foreach(mixed key, mixed val in invisibles){
            extra += "Invisible "+pluralize(key)+": "+val+"\n";
        }
    }

    ob->eventDescribeEnvironment(verbose);
    if(hud) message("system", extra, ob);
}

varargs int eventReceiveDamage(mixed agent, int type, int x, int internal, mixed limbs) {
    object ob = GetShadowedObject();
    string evidence, limb_string;

    if(!CheckSuit()) return 0;
    if(!ob) return 0;
    if(!suit->GetActive()) return ob->eventReceiveDamage(agent, type, x, internal, limbs);

    if(reporting){
        evidence = "The powered suit's Heads-Up-Display issues an alert:\n";
        evidence += "%^BOLD%^%^RED%^";
        if(objectp(agent)) evidence += "Damage received!";
        if(type) {
            switch(type){
            case BLUNT : evidence += " Damage type is BLUNT";break; 
            case BLADE : evidence += " Damage type is BLADE";break;
            case KNIFE : evidence += " Damage type is KNIFE";break;
            case WATER : evidence += " Damage type is WATER";break;
            case SHOCK : evidence += " Damage type is SHOCK";break;
            case COLD : evidence += " Damage type is COLD";break;
            case HEAT : evidence += " Damage type is HEAT";break;
            case GAS : evidence += " Damage type is GAS";break;
            case ACID : evidence += " Damage type is ACID";break;
            case MAGIC : evidence += " Damage type is MAGIC";break;
            case POISON : evidence += " Damage type is POISON";break;
            case DISEASE : evidence += " Damage type is DISEASE";break;
            case TRAUMA : evidence += " Damage type is TRAUMA";break;
            case PIERCE : evidence += " Damage type is PIERCE";break;
            case PSIONIC : evidence += " Damage type is PSIONIC";break;
            case ANOXIA : evidence += " Damage type is ANOXIA";break;
            case DEATHRAY : evidence += " Damage type is DEATHRAY";break;
            case EMOTIONAL : evidence += " Damage type is EMOTIONAL";break;
            case SONIC : evidence += " Damage type is SONIC";break;
            case BITE : evidence += " Damage type is BITE";break;
            case OTHER : evidence += " Damage type is OTHER";break;
            default : evidence += " Damage type is UNKNOWN";break;
            }
        }
        if(limbs) {
            if(stringp(limbs)) limb_string = limbs;
            else if(arrayp(limbs)) {
                if(stringp(limbs[0])) limb_string = implode(limbs,", ");
                else if(objectp(limbs[0])){
                    foreach(object limb in limbs){
                        limb_string += limb->GetKeyName()+", ";
                    }
                }
            }
        }
        else limb_string = ", location indeterminate. ";
        if(limbs) { 
            evidence += ", location: ";
            evidence += limb_string + ".";
        }
        this_object()->eventPrint(evidence+"%^RESET%^");

        this_object()->eventPrint("%^YELLOW%^Power level drained: "+x+" units.%^RESET%^");
        suit->eventDecrementCharge(x);
    }
    return 1;
}
