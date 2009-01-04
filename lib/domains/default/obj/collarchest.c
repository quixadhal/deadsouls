/*    /domains/Examples/etc/bag.c
 *    from the Nightmare IV LPC Library
 *    a sample bag object
 *    created by Descartes of Borg 950529
 */

#include <lib.h>

inherit LIB_STORAGE;

void create() {
    ::create();
    SetKeyName("chest");
    SetId(({"chest"}));
    SetAdjectives(({"medical","metal","stainless","steel"}));
    SetShort("a stainless steel chest");
    SetLong("This is a chest used to store medical tools.");
    SetMass(274);
    SetDollarCost(50);
    SetMaxCarry(500);
    SetPreventPut("You cannot put this in there!");
    SetInventory(([
                "/secure/obj/medtric" : 1,
                "/domains/default/armor/collar" : 5,
                ]) );
    SetCanClose(1);
    SetClosed(1);
}
mixed CanGet(object ob) { return "The chest does not budge.";}
