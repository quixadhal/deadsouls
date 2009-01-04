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
    SetId(({"locker"}));
    SetAdjectives(({"metal","tall","large","weapon","weapons"}));
    SetShort("a metal weapons locker");
    SetLong("This is a large metal locker designed to contain firearms."); 
    SetMass(3000);
    SetDollarCost(50);
    SetMaxCarry(500);
    SetInventory(([
                "/domains/campus/weap/9mil" : 5,
                //"/domains/campus/weap/m16rifle" : 5,
                //"/domains/campus/weap/50rifle" : 5,
                "/domains/campus/weap/357pistol" : 5,
                ]));
    SetCanClose(1);
    SetClosed(0);
    SetCanLock(1);
    SetLocked(0);
}
mixed CanGet(object ob) { return "The locker does not budge.";}
void init(){
    ::init();
}
