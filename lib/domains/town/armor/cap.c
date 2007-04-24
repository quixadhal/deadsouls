#include <lib.h>
#include <armor_types.h>
#include <damage_types.h>
inherit LIB_ARMOR;

static void create(){
    armor::create();
    SetKeyName("baseball cap");
    SetAdjectives( ({"red", "old", "greasy", "dirty", "baseball", "cat"}) );
    SetId( ({"cap", "bane", "repellent_collar", "hat", "cover"}) );
    SetShort("a red baseball cap");
    SetLong("A greasy, old, dirty baseball cap that was once mostly red, with "
      "the word 'CAT' on the front.");
    SetMass(10);
    SetBaseCost("silver",5);
    SetArmorType(A_HELMET);
}
void init(){
    ::init();
}
