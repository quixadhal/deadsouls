#include <lib.h>
#include <armor_types.h>
#include <damage_types.h>
inherit LIB_ARMOR;

int DoWear();
int CheckRing(string stat);

static void create(){
    armor::create();
    SetKeyName("ring");
    SetId(({"ring"}));
    SetAdjectives( ({"magic", "heavy", "metal"}) );
    SetShort("a heavy metal ring");
    SetLong("This ring seems to almost pulse with power.");
    SetMass(10);
    SetBaseCost("silver",5000);
    SetArmorType(A_RING);
    SetWear((: DoWear :));
    SetUnique(1);
}

void init(){
    ::init();
}

int DoWear(){
    this_player()->AddStatBonus("strength", (: CheckRing :) );
    write("You feel a resonant chord of strength roar within you "
      "as you wear the ring.");
    say(this_player()->GetName() + " wears " + GetShort() + ".");
    return 1;
}

int CheckRing(string stat) {
    if( !GetWorn() ) {
        previous_object()->RemoveStatBonus("strength", this_object());
        return 0;
    }
    else return 5;
}
