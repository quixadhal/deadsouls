#include <lib.h>

inherit LIB_STORAGE;


void create() {
    ::create();
    SetKeyName("stove");
    SetId(({"stove"}));
    SetAdjectives(({"large","imported","expensive"}));
    SetShort("a large stove");
    SetLong("This is a very large kitchen stove, which can "+
            "handle quite a lot of food at once. The exterior is "+
            "finished in a very tasteful brushed metal motif, and "+
            "it seems that this is one of those expensive, imported "+
            "stoves that only ridiculously rich people have in "+
            "their homes.");
    SetMass(5000);
    SetBaseCost("silver",1000);
    SetMaxCarry(5000);
    SetInventory(([
                "/domains/town/npc/rat" : 1
                ]));
    SetCanClose(1);
    SetClosed(1);
}

varargs mixed eventOpen(object who, string id, object tool) {
    object rat;
    mixed tmp = ::eventOpen(who, id, tool);
    if( tmp != 1 ) {
        return tmp;
    }
    rat = present("rat",this_object());
    if( rat ) {
        rat->eventMove(environment(this_object()));
        tell_room(environment(this_object()),"A mangy little rat leaps out of the stove!");
    }
    return 1;
}

void init(){
    ::init();
}
