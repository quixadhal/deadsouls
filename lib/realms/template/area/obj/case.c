#include <lib.h>
#include "../customdefs.h"

inherit LIB_STORAGE;

void create() {
    ::create();
    SetKeyName("case");
    SetId( ({ "container" }) );
    SetAdjectives( ({ "generic","nondescript" }) );
    SetShort("a glass case");
    SetLong("This is an expensive looking, ornamented glass case. It looks perfect for storing trophies or jewelry for display. It is an example of a transparent container.");
    SetInventory(([
                MY_AREA "/obj/watch" : 1,
                ]));
    SetOpacity(10);
    SetCanClose(1);
    SetClosed(1);
    SetMass(274);
    SetBaseCost("silver",1);
    SetMaxCarry(500);
}
void init(){
    ::init();
}

mixed CanGet(object ob) { return "The case does not budge.";}
