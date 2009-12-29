#include <lib.h>

inherit LIB_VERB;

static void create(){
    verb::create();
    SetVerb("wake");
    SetRules("");
    SetErrorMessage("Wake?");
    SetSynonyms(({"wake up","awaken","unsleep"}));
    SetHelp("Syntax: wake\n"
            "If asleep, you become less so.");
}

mixed can_wake(){
    if( this_player()->GetAlcohol() > 70 ) {
        return "You are too drunk to wake right now.";
    }
    if(this_player()->GetSleeping() < 1) {
        return "You are already awake.";
    }
    return 1;
}

mixed do_wake(){
    if(creatorp(this_player())){
        this_player()->SetSleeping(0);
        tell_player(this_player(),"You rouse from your slumber.");
        tell_room(environment(this_player()), this_player()->GetName()+
                " rouses from "+possessive(this_player())+
                " slumber.", ({this_player()}) );
        return 1;
    }
    if(this_player()->GetSleeping() > 1){
        tell_player(this_player(),"You become somewhat more wakeful.");
        tell_room(environment(this_player()), this_player()->GetName()+ 
                " appears to rouse somewhat from "+possessive(this_player())+
                " slumber.", ({this_player()}) );
        return this_player()->SetSleeping(this_player()->GetSleeping() - 1);
    }
    else tell_player(this_player(),"You are nearing full wakefulness...");
    return 1;
}
