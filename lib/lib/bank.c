#include <lib.h>

inherit LIB_ROOM;

static void create(){
    room::create();
}

mixed teller_check(){
    object *livs = get_livings(this_object());
    object teller;
    if(sizeof(livs))
        foreach(object liv in livs){
            if(inherits(LIB_TELLER,liv)) return liv;
        }
    return 0;
}

void init(){
    ::init();
    add_action("HandleTeller","deposit");
    add_action("HandleTeller","withdraw");
    add_action("HandleTeller","exchange");
    add_action("HandleTeller","balance");
}

int HandleTeller(string str){
    object teller = teller_check();
    if(!teller) return 0;
    teller->cmdParse(this_player(),query_verb(),str);
    return 1;
}
