/*    /domains/Examples/etc/bag.c
 *    from the Dead Souls LPC Library
 *    a sample bag object
 *    created by Descartes of Borg 950529
 */

#include <lib.h>

inherit LIB_STORAGE;

void create() {
    ::create();
    SetKeyName("box");
    SetId( ({ "box" }) );
    SetAdjectives( ({ "small", "plastic","green", "a" }) );
    SetShort("a small plastic box");
    SetLong("It is a simple plastic box used to hold things. It is green, and it has a cute Virtual Campus "+
            "logo on it.");
    SetMass(274);
    SetDollarCost(1);
    SetMaxCarry(10);
    //    SetPreventPut("You cannot put this in there!");
}
void init(){
    ::init();
}
