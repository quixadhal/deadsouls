#include <lib.h>
#include ROOMS_H
#include <meal_types.h>

inherit LIB_MEAL;

int MealCure(object who);

static void create() {
    meal::create();
    SetKeyName("pill");
    SetId(({"claritin","loratidine","dose","drug"}));
    SetShort("a claritin pill");
    SetAdjectives(({"10mg", "10 milligram", "non drowsy","non prescription", "prescription strength"}));
    SetLong("A tiny, 10 milligram dose of a drug that removes cold symptoms.");
    SetMass(10);
    SetStrength(1);
    SetBaseCost("silver",10);
    SetMealType(MEAL_FOOD);
    SetMealAction((: MealCure :));
    SetMealMessages("You swallow the pill.",
            "$N swallows a pill.");
}
void init(){
    ::init();
}
int MealCure(object who){
    mixed *stuff;
    int effect = 0;

    if(!who) return 0;

    stuff = all_inventory(who);

    foreach(object ob in stuff){
        if(ob && ob->isGerm() && ob->GetGermName() == "cold virus"){ 
            effect = 1;
            ob->eventMove(ROOM_FURNACE);
        }
    }
    if(effect){
        tell_player(who,"You feel a little better already.");
    }

    return 1;
} 
