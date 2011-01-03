#include <lib.h>
#include <daemons.h>

inherit LIB_ROOM;

mixed *text_scan;

int bad(string str);

void init() {
    ::init();
    add_action("read", "read");
    add_action("scribble", "scribble");
}

void create() {
    ::create();
    SetProperties( ([ "light" : 2, "night light" : 1, "no castle" : 1 ]));
    SetShort("a graffitti covered wall");
    SetLong(
            "You are in among ruined buildings in a bad part of the village.  "
            "There is graffiti all over the wall to the east.");
    SetItems(
            (["buildings" : "They are worn down by years of neglect.",
             "graffiti" : "Some is fresh, some is old.",
             "wall" : "You can scribble something on it.  <scribble stuff>.\n"
             "You can also read what is on it."]) );
    SetExits( 
            (["south"	 : "/domains/Praxis/alley2"]) );
    call_out("fade", 900);
    unguarded( (: RestoreObject("/domains/Praxis/data/wall") :) );
}

int scribble(string str) {
    if(!text_scan) text_scan = ({});
    if(bad(str)) {
        notify_fail("The Nightmare Reality prevents you from spreading your crap.\n");
        return 0;
    }
    text_scan += ({ str });
    personal_log("graffitti: "+ this_player()->query_name()+" "+ctime(time())+"\n"+str+"\n");
    unguarded( (: SaveObject("/domains/Praxis/data/wall") :) );
    message("my_action", sprintf("You scribble: %s", str), this_player());
    message("other_action", sprintf("%s scribbles some graffitti on the wall.",
                this_player()->query_cap_name()), this_object(),
            ({ this_player() }));
    return 1;
}

int read(string str) {
    int i;

    if(!str) {
        notify_fail("Read what?\n");
        return 0;
    }
    if(str != "wall" && str != "graffitti") {
        notify_fail("Read what?\n");
        return 0;
    }
    for(i=0; i<sizeof(text_scan); i++) {
        message("info", text_scan[i], this_player());
    }
    message("other_action", sprintf("%s reads the graffitti on the wall.",
                this_player()->query_cap_name()), this_object(),
            ({ this_player() }));
    return 1;
}

int bad(string str) {
    string a, b;
    int i;
    string *bad_words;

    bad_words = ({ "fuck", "shit", "cock", "cunt", "bitch" });
    for(i=0; i<sizeof(bad_words); i++) {
        if(sscanf(lower_case(str), "%s"+bad_words[i]+"%s", a, b) == 2) return 1;
    }
    return 0;
}

void fade() {
    string str;

    if(!text_scan) return;
    if(text_scan == ({})) return;
    str = text_scan[0];
    text_scan -= ({ str });
    message("environment", "Some graffitti fades from the wall.",
            this_object());
    call_out("fade", 10000);
    SaveObject("/domains/Praxis/data/wall");
}
