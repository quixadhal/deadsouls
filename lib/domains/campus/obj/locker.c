/*    /domains/Examples/etc/bag.c
 *    from the Dead Souls LPC Library
 *    a sample bag object
 *    created by Descartes of Borg 950529
 */

#include <lib.h>

inherit LIB_STORAGE;

void create() {
    ::create();
    SetKeyName("locker");
    //SetMatching(0);
    SetId(({"locker"}));
    SetAdjectives(({"metal","tall","school"}));
    SetShort("a tall metal locker");
    SetLong("This is a large school locker, the kind one might find in any of "+
            "thousands of schools around the world.");
    SetInventory(([
                "/domains/campus/weap/waterpistol" : 1,
                "/domains/town/obj/9mmclip" : 1,
                "/domains/campus/obj/bag" : 1,
                "/domains/campus/obj/dcell_std" : 4,
                ]));
    SetMass(274);
    SetDollarCost(50);
    SetMaxCarry(500);
    SetCanClose(1);
    SetClosed(1);
    SetCanLock(1);
    SetLocked(1);
    SetKey("locker_key_1");
}
mixed CanGet(object ob) { return "The locker does not budge.";}
void init(){
    ::init();
}
