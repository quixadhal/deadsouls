#include <lib.h>
#include <armor_types.h>
#include <damage_types.h>
inherit LIB_ARMOR;

int DoWear();
int CheckRing(string skill);

static void create(){
    armor::create();
    SetKeyName("ring");
    SetId(({"ring"}));
    SetAdjectives( ({"magic", "thin", "unobtrusive"}) );
    SetShort("a thin ring");
    SetLong("This thin ring seems designed to be "+
      "as light and unobtrusive as possible.");
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
    this_player()->AddSkillBonus("blade attack", (: CheckRing :) );
    this_player()->AddSkillBonus("blade defense", (: CheckRing :) );
    write("You wear the ring and feel oddly dextrous...and somehow "+
      "fancy.");
    say(this_player()->GetName() + " wears " + GetShort() + ".");
    return 1;
}

int CheckRing(string skill) {
    if( !GetWorn() ) {
        previous_object()->RemoveSkillBonus("blade attack", this_object());
        previous_object()->RemoveSkillBonus("blade defense", this_object());
        return 0;
    }
    else return 5;
}
