#include <lib.h>
inherit LIB_ROOM;

void create() {
    ::create();
    SetProperty("light", 1);
    SetShort( "Flamme's grave");
    SetLong(
            "You have entered an area set aside for insurance agents, "
            "bankers and women who mud.  It's a sad and dismal place, "
            "seldom visited by mourners since there usually aren't any.  "
            "There is one grave here that catches your eye.  It is "
            "covered in broken pencils and wadded up paper with bits "
            "of code that don't work.  Some poor creature has come to "
            "a bad end. You see a small wooden marker, as no one thought "
            "to put up a headstone.");
    SetItems(
            (["cemetery" : "All about you stretch the silent graves.  "
             "You feel like you are being watched.",
             "grave" : "The grave is covered with the trash and "
             "mementos of a coder.",
             "pencils" : "The pencils are all short, chewed, and "
             "broken.  None write anymore.",
             "writing" : "Here lies Flamme.  This lovely young lass "
             "met an untimely end when she used humor at the wrong "
             "time and with the wrong god.  Let this be a lesson to "
             "young immortals.",
             "paper" : "Countless small bits and wads of paper litter the area.",
             "code" : "The pieces of code never did work, and are "
             "unintelligible to normal humans.",
             "marker" : "The plaque is of white marble.  You might "
             "be able to read the writing on it."])  );
    SetExits( 
            (["northeast" : "/domains/Praxis/cemetery/grave_yard5",
             "northwest" : "/domains/Praxis/cemetery/grave_yard2",
             "southwest" : "/domains/Praxis/cemetery/grave_yard3"]) );
}

void reset() {
    ::reset();
    if (!present("ghost"))
        new("/domains/Praxis/cemetery/mon/ghost4")->move(this_object());
}

void init() {
    ::init();
    add_action("read","read");
}

int read(string str) {
    if (str == "writing")  {
        write (::GetLong("writing"));
        return 1;
    }
    notify_fail("Read what?\n");
    return 0;
}

