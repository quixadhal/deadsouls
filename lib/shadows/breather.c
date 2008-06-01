#include <lib.h>
#include <dirs.h>
#include <respiration_types.h>
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
    object ob = GetShadowedObject();
    if(!ob) return 0;
    rtype = ob->GetRespiration();

    if(breatherob && breatherob->GetRemainingCharge()){
        if(rtype & R_AIR){
            breatherob->eventDecrementCharge();
            return 1;
        } 
        if(rtype & R_WATER || rtype & R_METHANE) return 0;
    }
    return ob->CanBreathe(args);
}
