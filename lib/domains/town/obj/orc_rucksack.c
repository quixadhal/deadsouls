#include <lib.h>

#include <armor_types.h>
#include <damage_types.h>
inherit LIB_WORN_STORAGE;

static void create() {
    ::create();
    SetKeyName("rucksack");
    SetId(({"ruck","sack"}));
    SetAdjectives(({"sturdy","green","olive","olive-green","od green","od-green"}));
    SetShort("a sturdy, olive-green rucksack");
    SetLong("This is a large, sturdy rucksack made of some sort of "
            "strong, canvas-like material. It appears suitable "+
            "for just about any adventure you'd find yourself in.");
    SetMass(100);
    SetDollarCost(200);
    SetCanClose(1);
    SetClosed(1);
    SetMaxCarry(1000);
    SetDamagePoints(100);
    SetArmorType(A_VEST);
    SetInventory(([
                "/domains/town/obj/torch" : 1,
                "/domains/town/obj/match" : 3,
                "/domains/town/meals/rebel_yell" : (random(100) < 30),
                "/domains/town/meals/green_spice" : (random(100) < 10),
                "/domains/town/meals/potion_healing" : (random(100) < 10),
                "/domains/town/meals/spazalin" : (random(100) < 40),
                "/domains/town/meals/winebottle" : (random(100) < 20),
                "/domains/town/meals/ham_sand" : random(4),
                "/domains/town/meals/shark" : random(2),
                ]));
}
void init(){
    ::init();
}
