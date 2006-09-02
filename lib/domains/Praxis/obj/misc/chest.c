/*    /d/Praxis/obj/misc/chest.c
 *    from Nightmare IV
 *    a lockable treasure chest
 *    created by Descartes of Borg 940212
 */

#include <lib.h>
#include <objects.h>

inherit LIB_STORAGE;

void create() {
    object money;

    ::create();
    SetKeyName("chest");
    SetId( ({ "chest" }) );
    SetAdjectives( ({ "orc", "treasure" }) );
    SetShort("an orc treasure chest");
    SetLong("A huge treasure chest with a great big lock on it.");
    SetMass(2000);
    SetValue(100);
    SetMaxCarry(2000);
    SetCanClose(1);
    SetCanLock(1);
    SetPreventPut("A treasure chest?  Right!");
    SetProperty("magic hold", 10);
    SetKey("orc_treasure");
    money = new(LIB_PILE);
    money->SetCurrency("platinum", random(5));
    money->SetCurrency("gold", random(100));
    money->SetCurrency("copper", random(10000));
    money->move(this_object());
    SetClosed(1);
    SetLocked(1);
}
