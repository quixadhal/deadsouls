#include <lib.h>
#include <dirs.h>
#include <respiration_types.h>
#include <damage_types.h>
#include <daemons.h>

inherit LIB_SHADOW;

object breatherob;
int rtype;

int eventShadow(object whom){
    if(base_name(previous_object()) == "/domains/default/armor/breather" ||
            base_name(previous_object()) == "/domains/town/armor/breather"){
        breatherob = previous_object();
        ::eventShadow(whom);
        rtype = RACES_D->GetRaceRespirationType(whom->GetRace());
        return 1;
    }
    return 0;
}

varargs int CanBreathe(mixed args...){
    int ret;
    object ob = GetShadowedObject();
    if(!ob) return 0;
    rtype = ob->GetRespiration();

    if(breatherob && breatherob->GetRemainingCharge()){
        if(rtype & R_AIR){
            breatherob->eventDecrementCharge();
            return 1;
        } 
        if(rtype & R_VACUUM) ret = 1;
        else if(rtype & R_WATER || rtype & R_METHANE) ret = 0;
    }
    if(!undefinedp(ret)) return ret;
    return ob->CanBreathe(args...);
}

string GetResistance(int type){
    object ob = GetShadowedObject();
    if(!ob) return 0;
    if(type != GAS) return ob->GetResistance(type);
    if(breatherob && breatherob->GetRemainingCharge()){
        breatherob->eventDecrementCharge();
        return "immune";
    }
    return ob->GetResistance(type);
}
