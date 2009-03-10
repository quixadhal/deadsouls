#include <lib.h>
#include <vendor_types.h>
inherit LIB_ITEM;
string patient;

void create(){
    ::create();
    SetKeyName("regeneration slip");
    SetId( ({"slip","leaf","paper","healer token"}) );
    SetAdjectives( ({"regeneration","medical","paper"}) );
    SetShort("a regeneration slip");
    SetLong("This is a small, fragile piece of paper, possibly even a leaf. "
            "It has some script scribbled on it.");
    SetMass(1);
    SetBaseCost("silver",1200);
    SetVendorType(VT_HERB);
    SetItems( ([
                ({"script","writing","scribbling"}) : "There is writing on the slip."
                ]) );
    SetRead( ([
                ({"script","writing","scribbling"}) : "The slip reads: \"etaoin\""
                ]) );
    SetDefaultRead("default", "The slip reads: \"etaoin\"");
    SetProperty("problem","regenerate");
}

void init(){
    ::init();
    if( living(environment()) && environment()->GetKeyName() != "clepius"){
        patient = environment()->GetKeyName();
    }
}

int SetPatient(string str){
    if(str) patient = str;
    return 1;
}

string GetPatient(){
    if(patient) return patient;
    else return "none";
}

