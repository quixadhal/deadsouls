#include <lib.h>

inherit LIB_ITEM;

int __InvisTime;
mixed *__Invis;

void create() {
    ::create();
    SetKeyName("invis");
    SetId( ({ "invis" }) );
    SetInvis(1);
    SetPreventPut("What are you doing?");
    SetPreventDrop("What are you doing?");
}

void create_invis(object who, string what, int x) {
    __Invis = ({ who, what });
    __InvisTime = time() + x;
    who->SetInvis( (: this_object(), "test_invis" :) );
    eventMove(who);
}

int test_invis(object who, object whom) {
    if(!__Invis) {
        if(this_object()) this_object()->reeventMove();
        return 0;
    }
    if(__InvisTime < time()) {
        message("environment", "You are feeling more vulnerable now.",
                __Invis[0]);
        this_object()->reeventMove();
        return 0;
    }
    if(__Invis[0] != who) return 0;
    if(whom->query_race() == __Invis[1]) return 1;
    return 0;
}

int remove() {
    int x;

    //if(!__Invis || !sizeof(__Invis)) return ::reeventMove();
    //if(__Invis[0])
    //  __Invis[0]->remove_invis_test( (: this_object(), "test_invis" :) );
    //x = ::reeventMove();
    if(!this_object()) return x;
    else __Invis[0]->SetInvis( (: this_object(), "test_invis" :) );
    return x;
}
