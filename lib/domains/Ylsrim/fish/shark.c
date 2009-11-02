/*    /domains/Ylsrim/fish/shark.c
 *    from the Dead Souls Mud Library
 *    created by Descartes of Borg 960302
 */

#include <lib.h>
#include <dirs.h>
#include <damage_types.h>

inherit LIB_FISH;

void BiteMe(object who);

static void create() {
    fish::create();
    SetKeyName("blue shark");
    SetId( ({ "shark", "blue shark" }) );
    SetShort("a blue shark");
    SetLong("It is a huge, long fish with razor sharp teeth and a carnivorous "
            "appetite.");
    SetMass(100); 
    SetFight(40);
    SetFood("/domains/Ylsrim"+ "/meal/shark");
}

int eventCatch(object who, object pole) { 
    call_out((: BiteMe, who :), 0);
    return 1;
}

void BiteMe(object who) {
    who->eventPrint("The shark bites you before it dies!");
    environment(who)->eventPrint("The shark bites " + who->GetName() +
            " before it dies!", who);
    who->eventReceiveDamage(this_object(), KNIFE, random(20), 0,
            who->GetRandomLimb("right hand"));
}
