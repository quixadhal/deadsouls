#include <lib.h>

inherit LIB_MEAL;

void create() {
    ::create();
    SetKeyName("easter egg");
    SetId( ({ "egg", "easter egg" }) );
    SetShort("an %^RED%^e%^GREEN%^a%^YELLOW%^s%^BLUE%^t%^MAGENTA%^e"
            "%^CYAN%^r%^RESET%^ egg");
    SetLong(
            "A beautiful easter egg brought by the easter bunny.  It looks "
            "yummy.  If you do not want to eat it, why not hide it for "
            "someone else?"
           );
    SetValue(0);
    SetMass(10);
    if(random(100) < 30) {
        SetStrength(-10);
        SetMealMessages("$N eat $O, and it is ROTTEN!",
                "$N eats $O, and it is ROTTEN!");
    }
    else {
        SetStrength(10);
        SetMealMessages("$N eat $O filled with delicious candy!",
                "$N eats $O filled with delicious candy!");
    }
}

void init() {
    ::init();
    add_action("cmd_search", "search");
    add_action("cmd_hide", "hide");
}

int cmd_hide(string str) {
    if(present(str, this_player()) != this_object()) return 0;
    message("my_action", "You hide "+GetShort()+".", this_player());
    message("other_action", this_player()->query_cap_name()+
            " hides something.", environment(this_player()), ({this_player()}));
    SetInvis(1);
    eventMove(environment(this_player()));
    return 1;
}

int cmd_search(string str) {
    if(!query_invis()) return 0;
    message("my_action", "You find "+GetShort()+"!", this_player());
    message("other_action", this_player()->query_cap_name()+
            "finds "+GetShort()+"!", environment(this_player()), 
            ({ this_player() }));
    SetInvis(0);
    return 1;
}
