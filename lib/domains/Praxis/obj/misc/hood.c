//  /domains/Praxis/obj/misc/hood.c
//  A hood for blinding players
//  For the Nightmare mortal law system
//  Created by Manny@Nightmare 940906

#include <lib.h>

inherit LIB_ITEM;

create() {
    ::create();
    SetKeyName("hood");
    SetId(({"hood", "black hood" }) );
    SetShort("a black hood");
    SetLong("A black hood to hide the eyes of victims");
    SetPreventDrop("The hood is securly fastened to your neck");
}

void init() {
    ::init();
    if(!living(environment(this_object()))) this_object()->remove();
    add_action("cmd_look", "look");
    add_action("cmd_look", "peek");
    add_action("cmd_look", "give");
}

int cmd_look() {
    message("my_action", "It is too dark.", this_player());
    return 1;
}

