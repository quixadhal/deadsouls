#include <lib.h>

inherit LIB_SENTIENT;

int CheckRat(mixed val){
    string *allowed_races = ({ "orc", "half-orc", "rodent" });
    if(!val) return 0;
    if(!objectp(val)) return 0;
    if(member_array(val->GetRace(), allowed_races) != -1) return 0;
    return 1;
}

static void create() {
    sentient::create();
    SetKeyName("rat");
    SetMaxHealthPoints(10);
    SetAdjectives( ({"mangy", "little", "dirty"}) );
    SetId( ({"rat"}) );
    SetShort("a rat");
    SetLong("A scruffy little dirty rat.");
    SetLevel(1);
    SetRace("rodent");
    SetGender("male");
    SetClass("fighter");
    SetEncounter( (: CheckRat :) );
    SetMessage("come","$N scurries in.");
    SetMessage("leave","$N scurries $D.");
    SetAction(5, ({ 
                "The rat squeaks.", "You hear a rat scuttling about.", 
                "A scruffy little rat brushes against your leg.",
                "You hear tiny munching sounds."}));
}
void init(){
    ::init();
}
