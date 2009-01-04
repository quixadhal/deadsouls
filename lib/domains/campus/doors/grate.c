/*    /domains/Examples/etc/door.c
 *    from the Dead Souls LPC Library
 *    an example door object connecting doorroom1.c and doorroom2.c
 *    created by Descartes of Borg 950411
 */

#include <lib.h>

inherit LIB_DOOR;
//inherit "/lib/events/close";

static void create() {
    door::create();
    SetSide("down", ([ "id" : ({"grate","metal grate","sewer","sewer grate","rainwater grate","water grate"}),
                "short" : "a drainage grate",
                "long" : "This is a standard grate "+
                "for draining excess water from the road into the sewer system below.",
                //Though "+
                //"it is man-sized and apparently can be opened, there is no visible way "+
                //"of opening it from up here.",
                "lockable" : 0 ]) );
    SetSide("up", ([ "id" : ({"grate","metal grate","sewer","sewer grate","rainwater grate","water grate"}),
                "short" : "a drainage grate",
                "long" : "This is a standard drainage grate. You can see "+
                "light through it, and hear the sounds of the village above.",
                "lockable" : 0 ]) );
    SetPerforated(1);
    SetClosed(1);
}

int CanOpen(object ob){
    if(base_name(environment(ob)) == "/domains/campus/room/sewer3"){
        return 1;
    }
    else return 0;
}

