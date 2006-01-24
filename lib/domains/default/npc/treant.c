#include <lib.h>

inherit LIB_SENTIENT;

static void create() {
    sentient::create();
    SetKeyName("treant");
    SetAdjectives( ({"great", "old", "giant", "ancient", "benevolent"}) );
    SetId( ({"protector", "tree"}) );
    SetShort("an ancient treant");
    SetLong("This is a great old treant, which looks much like a giant tree with a face, arms, and legs. Ancient protectors of forests, treants are widely respected for their general good will and benevolence.");
    SetLevel(1);
    SetMelee(1);
    SetRace("tree");
    SetGender("neuter");
}
void init(){
    ::init();
}
