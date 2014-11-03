//    project_room.c
//    Nightmare Mudlib
//    The Project Room
//    by Kalinash@Nightmare on June 30, 1994.

#include <lib.h>
#include <daemons.h>

inherit LIB_ROOM;

void init() {
    ::init();
    add_action("start_fun", "start");
    add_action("list_fun", "list");
}

void create() {
    ::create();
    SetProperties( ([
                "light" : 2, 
                "no kill" : 1,
                "no steal" : 1,
                "no magic" : 1,
                "no teleport" : 1,
                "no castle" : 1
                ]) );
    SetShort("Project Room");
    SetLong("Welcome to the Project Tracking Department!\n"
            "A list of commands is printed on the wall, use <read list> to view it.  "
            "This is the room you can come to find out about "
            "what is going on on the immortal plane on Nightmare.");
    SetExits( ([ "west" : "/domains/Praxis/mudlib" ]) );
}

int start_fun(string group) {
    if(!group) {
        message("my_action", "Start what?", this_player());
        return 1;
    }
    //if(!PROJECT_D->add_group(group, this_player()->query_name()))
    //   return notify_fail("Error in the daemon.\n");
    //message("my_action", sprintf("Group '%s' successfully created.",
    // group), this_player());
    return 1;
}

int list_fun(string which) {
    if(!which)
        return notify_fail("List what?\n");
    if(which == "groups")
        return;
}
