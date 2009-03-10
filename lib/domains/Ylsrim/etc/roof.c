/*    /domains/Ylsrim/etc/roof.c
 *    From the Dead Souls Mud Library
 *    An example of something people can jump from
 *    Created by Descartes of Borg 961222
 */

#include <lib.h>
#include <jump.h> // defines JUMP_FROM

inherit LIB_DUMMY; // These do not show up in desc, but you can look at them
inherit LIB_JUMP; // This makes it jumpable

static void create() {
    dummy::create();
    SetKeyName("roof");
    SetId("roof");
    SetAdjectives("adobe", "bank");
    SetShort("a roof");
    SetLong("The bank roof looks over the road below. "
            "Maybe you could jump into it."
            "The adobe wall looks like you could climb down it.  ");
    //"A hole in the center of the roof opens into the bank.");
    AddJump("roof","/domains/Ylsrim/room/bank", JUMP_FROM);
}
