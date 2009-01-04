/*    /domains/Examples/etc/bag.c
 *    from the Dead Souls LPC Library
 *    a sample bag object
 *    created by Descartes of Borg 950529
 */

#include <lib.h>

inherit LIB_STORAGE;

void create() {
    ::create();
    SetKeyName("redbox");
    SetId( ({ "box","redbox","rbox" }) );
    SetAdjectives( ({ "small", "plastic","red", "a" }) );
    SetShort("a small, %^RED%^red%^RESET%^ plastic box");
    SetLong("It is a simple plastic box used to hold things. It is %^RED%^red%^RESET%^, and it has a cute Virtual Campus "+
            "logo on it.");
    SetMass(274);
    SetDollarCost(1);
    SetMaxCarry(100);
    SetCanClose(1);
    SetClosed(0);
    //    SetPreventPut("You cannot put this in there!");
}
void init(){
    ::init();
}
