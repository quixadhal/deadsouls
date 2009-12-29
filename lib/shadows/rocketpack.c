#include <lib.h>
#include <dirs.h>
#include <respiration_types.h>
#include <daemons.h>

inherit LIB_SHADOW;

object packob;
object person;
int rtype;

int eventShadow(object whom){
    if(base_name(previous_object()) == "/domains/default/armor/rocketpack" ||
            base_name(previous_object()) == "/domains/town/armor/rocketpack"){
        person = whom;
        packob = previous_object();
        ::eventShadow(whom);
        return 1;
    }
    return 0;
}

varargs int eventFall(mixed args...){
    if(!packob || !packob->GetRunning()){
        //return person->eventFall(args);
    }
    packob->eventDecrementCharge();
    return 1;
}

