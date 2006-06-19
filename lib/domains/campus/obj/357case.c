/*    /domains/Examples/etc/bag.c
 *    from the Nightmare IV LPC Library
 *    a sample bag object
 *    created by Descartes of Borg 950529
 */

#include <lib.h>

inherit LIB_STORAGE;

void create() {
    ::create();
    SetKeyName("cardboard case");
    SetAdjectives( ({"small", "cardboard", ".357 ammo", "ammo"}) );
    SetId( ({ "case" }) );
    SetShort("a small .357 ammo case");
    SetLong("A small cardboard case designed to carry .357 caliber amunition.");
    SetMass(10);
    SetDollarCost(1);
    SetMaxCarry(12);
}
void CanReceive(object ob){
    ::CanReceive();
    if(ob->GetKeyName() != "357round") {
	tell_object(this_player(),"This cardboard case is for .357 ammunition only.");
	return;
    }
}
void init(){
    ::init();
}
