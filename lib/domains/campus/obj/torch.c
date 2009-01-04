/*    /domains/Praxis/etc/torch.c
 *    from Dead Souls
 *    created by Descartes of Borg 951023
 */

#include <lib.h>

inherit LIB_TORCH;


static void create() {
    torch::create();
    SetKeyName("torch");
    SetId( ({ "torch", "old torch", "wooden torch" }) );
    SetAdjectives( ({ "old", "wooden" }) );
    SetShort("an old, wooden torch");
    SetLong("An old, wooden torch with a bit of cloth wrapped around "
            "one end and dipped into a flamable substance.");
    SetRadiantLight(7);
    SetFuelRequired(1);
    SetMaxFuel(1000);
    SetFuelAmount(1000);
    SetRefuelable(1);
    SetMass(50);
    SetDollarCost(60);
    SetBurntValue(10);
    SetClass(10);
}
void init(){
    ::init();
}
