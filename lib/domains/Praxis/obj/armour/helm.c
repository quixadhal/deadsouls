#include <lib.h>
#include <armor_types.h>
inherit "/lib/std/armor";

void create() {
    ::create();
    SetKeyName("helm");
    SetId( ({ "helm", "helmet", "knight's helm" }) );
    SetShort( "Knight's helm");
    SetLong( "The helm of a knight of justice.");
    SetMass(375);    SetValue( 95);

    SetArmorType(A_HELMET);
    SetRestrictLimbs( ({ "head" }) );
    SetAC(3);
    SetWear( (: this_object(), "extra_worn" :) );
}

int extra_worn() {
    if(this_player()->query_alignment() < 200) {
        write("This helmet burns with disgust.");
        say(this_player()->query_cap_name()+" is burned trying to wear the knight's helm.");
        return 0;
    }
    write("You feel the the powers of goodness flowing through you.");
    say(this_player()->query_cap_name() + " shines with the powers of goodness of the knight's helm.");
    return 1;
}
