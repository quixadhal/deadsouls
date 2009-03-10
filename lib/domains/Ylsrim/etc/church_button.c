/*    /domains/Ylsrim/etc/chuch_button.c
 *    From the Dead Souls Mud Library
 *    An example dummy item that allows pressing
 *    Created by Descartes of Borg 961222
 */

#include <lib.h>

inherit LIB_DUMMY; // These do not show up in desc, but you can look at them
inherit LIB_PRESS; // Makes the item pressable

int openDoor(object who) {
    object door = find_object("/domains/Ylsrim/etc/church_door");

    send_messages("press", "$agent_name $agent_verb the button.",
            who, 0, environment(who));
    if( !door->GetClosed() ) {
        return 1;
    }
    environment(who)->eventPrint("You hear a click from the door.");
    if( door->GetLocked() ) {
        door->SetLocked(0);
    }
    else {
        door->SetLocked(1);
    }
    return 1;
}

static void create() {
    dummy::create();
    SetKeyName("button");
    SetId("button");
    SetAdjectives("big", "huge");
    SetShort("a huge button");
    SetLong("It is a wooden button that you could probably press.");
    SetPress((: openDoor :));
}
