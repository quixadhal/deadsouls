#include <lib.h>

inherit LIB_CHAIR;
inherit LIB_SWIVEL;

static void create() {
    chair::create();
    SetKeyName("chair");
    SetId("chair");
    SetAdjectives( ({ "swivel", "small", "black" }) );
    SetShort("a swivel chair");
    SetLong("This is a small swivel chair, with tasteful and understated "+
            "black covers.");
    SetMass(150);
    SetDollarCost(15);
    SetMaxSitters(1);
    SetPreventGet("The chair does not budge.");
}

void init(){
    chair::init();
}

varargs mixed eventSwivel(object who){
    if(member_array(who, GetSitters()) != -1){
        write("You swivel around in your swivel chair! Whee!");
        say(this_player()->GetName()+" swivels around in "
                +possessive(this_player())+" "+
                "swivel chair, yelling \"WHEEEE!!!\"");
        return 1;
    }
    else {
        write("You're not sitting in it!");
        return 1;
    }
}
