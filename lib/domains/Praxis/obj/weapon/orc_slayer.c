#include <lib.h>
inherit LIB_ITEM;

create() {
    ::create();
    SetId(({"sword", "the_one_orc_slayer","orcslayer","fine sword"}));
    SetKeyName("orcslayer");
    SetShort("a fine sword");
    SetLong("It is a very finely crafter sword with the picture of a dying orc on it.\nThere are writings on it.\n");
    SetMass(611);   SetValue( 200);

    SetClass(7);
    SetWeaponType("blade");
    SetWield("You feel a great hatred for orcs as you wield the sword.\n");
    SetRead( "Orcslayer\n");

    true();
}

int eventStrike(object ob) {
    int x, y;

    if(ob->query_race() != "orc") return 0;
    y = random(10);
    x = this_player()->query_alignment();
    if(x> 0) x = (x/100);
    else x = 0;
    y += x;
    write("The Orcslayer draws more orc blood!\n");
    say(this_player()->query_cap_name()+" Orcslayer draws more orc blood!\n", this_player());
    return y;
}
