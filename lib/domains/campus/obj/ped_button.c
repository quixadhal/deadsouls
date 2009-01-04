/*    /domains/campus/etc/chuch_button.c
 *    From the Dead Souls Object Library
 *    An example dummy item that allows pressing
 *    Created by Descartes of Borg 961222
 */

#include <lib.h>

inherit LIB_DUMMY; // These do not show up in desc, but you can look at them
inherit LIB_PRESS; // Makes the item pressable

int PushButton(object who) {
    object *objects;
    objects=({ load_object("/domains/campus/doors/red_door") });
    objects+=({ load_object("/domains/campus/doors/green_door") });
    objects+=({ load_object("/domains/campus/doors/blue_door") });
    objects+=({ load_object("/domains/campus/room/red_room") });
    objects+=({ load_object("/domains/campus/room/green_room") });
    objects+=({ load_object("/domains/campus/room/blue_room") });
    objects+=({ load_object("/domains/campus/room/monty") });

    send_messages("press", "$agent_name $agent_verb the button, "
            "resetting the experiment.",
            who, 0, environment(who));

    foreach(object ob in objects){
        ob->ButtonPush();
    }

}

static void create() {
    dummy::create();
    SetKeyName("button");
    SetId(({"button","button on the pedestal" }));
    SetAdjectives("pedestal", "red","shiny","candylike","candy-like","big");
    SetShort("a big red button");
    SetLong("It is a red, shiny, candy-like button.");
    //SetPress((: PushButton :));
    SetPress(tell_object(this_player(),"hi!"));
}
