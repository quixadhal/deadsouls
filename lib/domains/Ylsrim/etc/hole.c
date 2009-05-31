/*    /domains/Ylsrim/etc/hole.c
 *    From the Dead Souls Mud Library
 *    An example of something people can jump into
 *    Created by Descartes of Borg 961222
 */

#include <lib.h>
#include <jump.h> // defines JUMP_INTO

inherit LIB_DUMMY; // These do not show up in desc, but you can look at them
inherit LIB_JUMP; // This makes it jumpable

static void create() {
    dummy::create();
    SetKeyName("hole");
    SetId("hole", "bank");
    SetShort("a hole in the roof");
    SetLong("It is too dark to see inside the bank, but perhaps you could "
            "jump into it?");
    AddJump("hole","/domains/Ylsrim/room/bank", JUMP_INTO);
}
