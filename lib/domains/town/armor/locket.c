#include <lib.h>
#include <armor_types.h>
#include <damage_types.h>
inherit LIB_WORN_STORAGE;

static void create(){
    ::create();
    SetKeyName("necklace");
    SetId(({"necklace","chain" "locket"}));
    SetAdjectives(({"silver","braided","finest"}));
    SetShort("a silver necklace");
    SetLong("A braided chain, made of finest silver. A locket is on it.");  
    SetMass(50);
    SetMaxCarry(300);
    SetBaseCost("silver",1);
    SetProtection(BLUNT,1);
    SetProtection(BLADE,1);
    SetProtection(KNIFE,1);
    SetArmorType(A_COLLAR);
    SetItems(([
                ({"locket"}) : "It bears the name of the man that Brandy loved.",
                ])); 
}

void init(){
    ::init();
}
