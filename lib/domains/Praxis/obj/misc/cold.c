#include <lib.h>

inherit LIB_GERM;

int coughs;

void create() {
    germ::create();
    coughs = 80;
    SetKeyName("cold");
    SetId( ({ "cold" }) );
    SetShort("the cold");
    SetLong("A viral infection which is mostly harmless, but which "
            "generally afflicts most beings during their lifetime.");
    SetCommunicable(10);
    SetCure(1);
    SetLifeSpan(300);
    SetType("viral");
}

void suffer(object ob) {
    coughs--;
    if(coughs%2) return;
    ob->add_hp(-(ob->query_hp()/10));
    message("my_action", "You cough horridly!", ob);
    message("other_action", ob->query_cap_name()+" coughs horribly!",
            environment(ob), ob);
    if(!coughs) this_object()->remove();
}
