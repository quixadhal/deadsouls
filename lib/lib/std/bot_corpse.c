#include <lib.h>
#include <medium.h>
#include <message_class.h>

inherit LIB_CORPSE;
int stank;

void create(){
    corpse::create();
    SetId(({"chassis","body","synthetics"}));
    SetAdjectives(({"melting","corroding"}));
}

void init(){
    ::init();
    if(environment() && environment()->GetMedium() == MEDIUM_LAND) stank = 1;
}

int eventDecay(){
    if( !environment() ){
        Destruct();
        return 0;
    }
    Fresh = 0;
    switch(Count){
        case 10:
            if(stank)
                environment()->eventPrint(possessive_noun(Owner) + " chassis " +
                        "begins to corrode.", MSG_ROOMDESC);
            SetId(GetId()..., "chassis", "body");
            SetAdjectives(GetAdjectives()..., "melting", "corroding");
            SetShort("the corroding chassis of an artificial body");
            SetSmell("The chassis emits an acrid chemical odor.");
            break;
        case 20:
            if(stank)
                environment()->eventPrint("An acrid chemical odor fills the entire "
                        "area.", MSG_ROOMDESC);
            SetId(GetId()..., "synthetics", "pile", "pile of synthetics");
            SetShort("a pile of corroding synthetics");
            SetSmell("Its smell is nearly unbearable.");
            break;
        case 30:
            Destruct();
            return 0;
    }
    Count++;
    return Count;
}

void SetCorpse(object who){
    string tmpshort = who->GetShort();
    if(sizeof(who->GetRiders())) tmpshort = who->GetPlainShort();
    SetKeyName(who->GetKeyName());
    SetId(({ "remains","wreckage",who->GetId()... }) );
    Owner = who->GetCapName();
    Race = who->GetRace();
    Count = 1;
    Fresh = 1;
    SetShort("the wreckage of "+tmpshort);
    SetLong("As you look closely at " + who->GetCapName() +
            ", you notice that " +  nominative(who) +
            " does not appear to be moving.");
}
