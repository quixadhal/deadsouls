/*    /domains/Ylsrim/etc/stool.c
 *    From the Dead Souls V Object Library
 *    An example of a chair object
 *    Created by Descartes of Borg 961221
 */

#include <lib.h>

inherit LIB_CHAIR;

static void create() {
    chair::create();
    SetKeyName("stool");
    SetId("stool");
    SetAdjectives("wooden", "rickety");
    SetShort("a rickety wooden stool");
    SetLong("The stool is sturdy enough for you to sit in.");
    SetMass(1500);
    SetValue(15);
    SetMaxSitters(1);
}
