#include <lib.h>

inherit LIB_SENTIENT;
int listen = 1;

static void create() {
    sentient::create();
    SetKeyName("rain man");
    SetId(({"rainman","man","raymond"}));
    SetAdjectives(({"rain"}));
    SetShort("Rain Man");
    SetLong("A distracted-looking human.");
    SetClass("explorer");
    SetLevel(1);
    SetMelee(0);
    SetRace("human");
    SetGender("male");
}

void init(){
    ::init();
}

void receive_message(string s1, string s2){
    if(!listen) return;
    listen = 0;
    eventForce("say I definitely saw "+s2);
    listen = 1;
    if(grepp(s2," attacks ")){
        eventForce("yell STOP THE H8!");
    }
}

varargs mixed eventHearTalk(object who, object target, int cls, string verb,
        string msg, string lang) {
    this_object()->receive_message("me",who->GetName()+" "+verb+
            "s: "+msg) ;
    return;
}

varargs int doPrint(string msg, string msg_class){
    this_object()->receive_message("me again",msg) ;
    return 1;
}

varargs int eventPrint(string msg, string msg_class){
    doPrint( msg, msg_class) ;
    return 1;
}
