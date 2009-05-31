/*    /domains/Ylsrim/etc/road.c
 *    From the Dead Souls Mud Library
 *    An example of something people can jump into
 *    Created by Descartes of Borg 961222
 */

#include <lib.h>
#include <jump.h> // defines JUMP_INTO
#include <damage_types.h> // defines BLUNT

inherit LIB_DUMMY; // These do not show up in desc, but you can look at them
inherit LIB_JUMP; // This makes it jumpable

int eventJump(object who) {
    who->eventReceiveDamage(0, BLUNT, random(100) + 5, 1);
    who->eventMoveLiving("/domains/Ylsrim/room/kaliid6",
            "$N tries to jump into the road "
            "and seriously hurts " + reflexive(who) + ".",
            "$N comes falling in.");
    return 1;
}

static void create() {
    dummy::create();
    SetKeyName("road");
    SetId("road");
    SetAdjectives("kaliid");
    SetShort("Kaliid Road", 1); // it is a proper noun
    SetLong("The main road through Ylsrim.");
    AddJump("road","/domains/Ylsrim/room/kaliid6",JUMP_INTO);
}
