#include <lib.h>

inherit LIB_DAEMON;

int cmd(string str) {
    object ob;
    string basename, filename;

    if(!str || str == "here") {
        ob = environment(this_player());
    }

    else ob = get_object(str, this_player());

    if(!ob) {
        write("That object can't be found.");
        return 1;
    }

    filename = file_name(ob);
    basename = base_name(ob);

    write("The unique object identifier is: "+filename);
    write("The file it is loaded from is: "+basename+".c");

    return 1;
}

string GetHelp() {
    return "Syntax: file [thing]\n\n"+
        "Provides information about the thing's name and file of origin.\n"+
        "With no argument, it defaults to your location.\n";
}
