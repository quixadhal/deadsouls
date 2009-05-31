#include <lib.h>
#include <armor_types.h>
#include <damage_types.h>
#include <size_types.h>
inherit LIB_ARMOR;

static void create(){
    ::create();
    SetKeyName("pajamas");
    SetId(({"pj","pjs", "peejays"}));
    SetAdjectives(({"small","tiny","pair of","set of",
                "bloodstained", "blood stained", "stained"}));
    SetShort("tiny, bloodstained pajamas");
    SetLong("This is a set of very small pajamas, stained with old, "
            "crusted blood.");
    SetItems( ([
                ({"blood","crusted blood", "old, crusted blood"}) : "It seems "+
                "that whoever was wearing these pajamas suffered some terrible "+
                "injuries.",
                ]) );
    SetMass(2);
    SetBaseCost("silver",1);
    SetArmorType(A_BODY_ARMOR);
    SetSize(S_SOMEWHAT_SMALL|S_SMALL);
}

void init(){
    ::init();
}
