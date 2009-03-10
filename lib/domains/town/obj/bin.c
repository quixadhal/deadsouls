#include <lib.h>

inherit LIB_STORAGE;

void create() {
    ::create();
    SetKeyName("bin");
    SetAdjectives( ({"newbie", "charity", "freebie"}) );
    SetId( ({"newbie bin"}) );
    SetShort("the newbie bin");
    SetLong("This medium-sized bin is designed to hold stuff that may be of use to newbies. By default it contains stuff that won't sell for much, but players are encouraged to make generous contributions for the benefit of weaker players. To view its contents: look in bin");
    SetInventory(([
                "/domains/town/obj/slip_heal" : 1,
                "/domains/town/obj/match" : 5,
                "/domains/town/obj/slip_regenerate" : 1,
                "/domains/town/obj/slip_excise" : 3,
                "/domains/town/obj/rayovac" : 1,
                "/domains/town/obj/spam" : 1,
                "/domains/town/obj/dcell" : 2,
                "/domains/town/obj/torch" : 1,
                ]));
    SetMaxCarry(3500);
    SetNoCondition(1);
    SetCanClose(0);
    SetMass(100);
    SetBaseCost("silver",1);
    SetNoClean(1);
}

void init(){
    ::init();
}

mixed CanGet(object ob) { return "The bucket does not budge.";}
