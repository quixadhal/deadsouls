#include <lib.h>
#include <vendor_types.h>
inherit LIB_ITEM;
inherit LIB_CLIMB;


int ClimbMe();
void create(){
    ::create();
    SetKeyName("tree");
    SetId( ({"tree"}) );
    SetAdjectives( ({"huge","large","big","old","great"}) );
    SetShort("a large tree");
    SetLong("This is a very large tree. It has been around since long "+
            "before the Virtual Campus, and looks like it will be around long "+
            "after, as well.");
    SetMass(20000);
    SetDollarCost(10);
    SetVendorType(VT_TREASURE);
    SetClimb( (: ClimbMe :) ,1);
}
void init(){
    ::init();
}
int ClimbMe(){
    string omsg,imsg,dest;
    object who;
    //dest="/domains/campus/room/start";
    dest="/domains/campus/room/uptree";
    who=this_player();
    omsg = "$N climbs up " + GetDefiniteShort() + ".";
    imsg = "$N comes climbing in.";
    who->eventMoveLiving(dest, omsg, imsg);
}
mixed CanGet(object ob) { return "The tree does not budge.";}
