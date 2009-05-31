#include <lib.h>
inherit LIB_ROOM;
inherit LIB_CLIMB;
inherit "/lib/props/ambiance";

int ClimbDown();
static void create() {
    room::create();
    SetAmbientLight(25);
    SetClimate("outdoors");
    SetShort("up a tree");
    SetLong("You are standing on some sturdy branches of the large "+ 
            "tree in University Square. ");
    SetExits( ([ 
                "down" : "/domains/campus/room/usquare"
                ]) );
    SetProperty("no attack", 1);
    SetClimb( (: ClimbDown :) ,2);
}
int ClimbDown(){
    string omsg,imsg,dest;
    object who;
    dest="/domains/campus/room/usquare";
    who=this_player();
    omsg = "$N climbs down.";
    imsg = "$N comes climbing down from the tree.";
    who->eventMoveLiving(dest, omsg, imsg);
    //return 1;
}
void init(){
    ::init();
}
