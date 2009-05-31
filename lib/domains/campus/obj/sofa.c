/*    /domains/campus/etc/stool.c
 *    From the Dead Souls Object Library
 *    An example of a chair object
 *    Created by Descartes of Borg 961221
 */

#include <lib.h>

inherit LIB_CHAIR;


static void create() {
    chair::create();
    SetKeyName("sofa");
    SetId("sofa");
    SetAdjectives( ({ "black", "small", "tasteful", "upholstered" }) );
    SetShort("a sofa");
    SetLong("This is a small, plush sofa tastefully upholstered "+
            "in black. It appears designed for sitting on.");
    SetMass(1500);
    SetDollarCost(15);
    SetMaxSitters(2);
}
mixed CanGet(object ob) { return "The bench does not budge.";}
void init(){
    ::init();
}
