#include <lib.h>
#include ROOMS_H

inherit LIB_DAEMON;

int cmd(string str) {
    object *rooms = ({});

    write("Number of currently loaded objects: "+sizeof(objects()));

    write("Destroying objects in the furnace...\n");
    flush_messages(this_player());
    update(ROOM_FURNACE);


    write("Destroying non-environment objects...");
    flush_messages(this_player());

    reap_other();
    reap_other();

    write("Done.");

    write("Number of currently loaded objects: "+sizeof(objects()));
    return 1;
}

string GetHelp(){
    return ("Syntax: flushobs\n\n"+
            "Immediately incinerates anything in the furnace and "+
            "destructs all cloned non-interactive objects that "+
            "lack an environment.");
}
