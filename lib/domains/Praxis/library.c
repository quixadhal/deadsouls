#include <lib.h>
#include <daemons.h>
#include <dirs.h>

inherit LIB_ROOM;

void init() {
    ::init();
    add_action("help", "help");
    add_action("read", "read");
}

void create() {
    ::create();
    SetSmell( "default", "The musty smell of old books fills the room.");
    SetProperties((["light":2, "no castle" : 1, "indoors" :1 ]) );
    SetShort( "Praxis Library");
    SetLong(
            "Welcome to the Praxis Library of higher learning!\n"
            "The library is filled with shelves and shelves full of books, "
            "reaching up towards the ceiling. The leather bound books are "
            "waiting to be read.  East Road is west of the library. "
            "The command <read books> will show you what books are available and "
            "type <read [title]> to read a particular book.");
    SetItems(
            (["library" : "The laws of reality are explained in here.",
             "shelf" : "It has books on it.",
             "shelves" : "Books fill them.",
             "book" : "One of many.",
             "books" : "They are everywhere on the shelves."]) );
    SetExits( 
            (["west" : "/domains/Praxis/east_road1"]) );

}

int read(string str) {
    string *dir;
    string *tmp;
    string tmp_str;
    int i,j,k;

    if(!str) {
        notify_fail("Read what?\n");
        return 0;
    }
    if(str == "volume" || str == "book") {
        notify_fail("Which "+str+"?\n");
        return 0;
    }
    if(str == "volumes" || str == "books") {
        message("info", "The books available for reading are:",this_player());
        dir = get_dir(DIR_LIBRARY + "/");
        for(i=0; i<sizeof(dir); i++) dir[i] = replace_string(dir[i], "_", " ");
        message("info", replace_string(format_page(dir, 3), "\n", " "),
                this_player());
        message("info", "Type <read [book name]> to read a particular book.", this_player());
    }
    else {
        str = replace_string(str, " ", "_");
        if(file_size(DIR_LIBRARY + "/"+str) == -1) {
            notify_fail("There is no book here by that title.\n");
            return 0;
        }
        this_player()->more(DIR_LIBRARY + "/"+str);
    }
    return 1;
}

int help(string str) {
    if(!str) return 0;
    if(str != "library") {
        notify_fail("You help "+str+".\n");
        return 0;
    }
    write("You can read what is in the library.\nType <read books>.\n");
    return 1;
}


