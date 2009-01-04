/*    /domains/Examples/etc/bag.c
 *    from the Dead Souls LPC Library
 *    a sample bag object
 *    created by Descartes of Borg 950529
 */

#include <lib.h>

inherit LIB_STORAGE;


void create() {
    ::create();
    SetKeyName("rack");
    SetId(({"wrack"}));
    SetAdjectives(({"wooden","large","weapon","weapons"}));
    SetShort("a wooden weapons rack");
    SetLong("This is a large wooden rack designed to contain "+
            "conventional or melee weapons. It is set into the wall.");
    SetMass(3000);
    SetDollarCost(500);
    SetMaxCarry(5000);
    SetInventory(([
                "/domains/campus/weap/dagger" : 5,
                "/domains/campus/weap/sword" : 5,
                "/domains/campus/weap/sharpsword" : 5,
                "/domains/campus/weap/staff" : 5,
                ]));
    SetCanClose(1);
    SetClosed(0);

}
mixed CanGet(object ob) { return "The rack does not budge.";}
void init(){
    ::init();
}
