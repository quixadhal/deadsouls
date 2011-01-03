#include <lib.h>
#undef CANDIDATE

inherit LIB_SENTIENT;

int manchurian;

int ProcessTalk(mixed args...){
    string speech = lower_case(args[2]);
    if(sizeof(speech) > 1) speech = truncate(speech,1);
    if(manchurian){
#ifdef CANDIDATE
        call_out( (: eventForce :), 1, speech);
#endif
    }
    if(!manchurian && grepp(speech,"solitaire") && grepp(speech,"play")){
        manchurian = 1;
#ifndef CANDIDATE
        eventForce("say No, thank you. I'm on duty.");
#else
        eventForce("nod");
#endif
    }
    return 1;
}

static void create(){
    sentient::create();
    SetKeyName("gate guard");
    SetId(({"human","guard","mp","soldier","policeman","military policeman"}));
    SetAdjectives(({"military"}));
    SetShort("a military gate guard");
    SetLong("This is a military policeman. He has "+
            "spent months in arduous training, and by the looks of him, has seen "+
            "plenty of action in his lifetime.  He is large, looks very "+
            "tough, and whatever he is guarding is probably very safe.");
    SetInventory(([
                "/domains/town/armor/bdu" : "wear bdu",
                "/domains/town/armor/helmet2" : "wear helmet",
                "/domains/town/weap/m16rifle_mp" : "wield rifle",
                ]));
    SetLevel(20);
    SetRace("human");
    SetClass("fighter");
    SetGender("male");
    SetTalkResponses( ([ "default" : (: ProcessTalk :) ]) );
    SetPolyglot(1);
    SetLanguage("common", 100);
    SetDefaultLanguage("common");
}

void init(){
    ::init();
}

