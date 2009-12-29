/*    /domains/Ylsrim/fish/shark.c
 *    from the Dead Souls Mud Library
 *    created by Descartes of Borg 960302
 */

#include <lib.h>
#include <meal_types.h>
#include <dirs.h>
#include <damage_types.h>

inherit LIB_FISH;

void BiteMe(object who);
static void create() {
    fish::create();
    SetKeyName("bull shark");
    SetId( ({ "shark", "bull shark" }) );
    SetShort("a bull shark");
    SetLong("It is a huge, long fish with razor sharp teeth and a carnivorous "
            "appetite.");
    SetMealType(MEAL_FOOD);
    SetMass(100); 
    SetStrength(50);
    SetBaseCost("silver", 10);
    SetFight(40);
    SetFood("/domains/town/meals/shark");
}
int eventCatch(object who, object pole) { 
    call_out((: BiteMe, who :), 0);
    return 1;
}
void BiteMe(object who) {
    who->eventPrint("The shark bites you before it dies!");
    environment(who)->eventPrint("The shark bites " + who->GetName() +
            " before it dies!", who);
    who->eventReceiveDamage(this_object(), BITE, random(30), 0,
            who->GetRandomLimb("right hand"));
}
void init(){
    ::init();
}
