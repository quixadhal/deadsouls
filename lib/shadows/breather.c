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
    if(rtype == R_VACUUM) return 1;
    if(breatherob && (rtype == R_WATER || rtype == R_METHANE)) return 0;
    if(!breatherob) return 0;
    if(!breatherob->GetRemainingCharge()) return 0;
    breatherob->eventDecrementCharge();
    return 1;
}

