#include <lib.h>
inherit LIB_ROOM;
inherit "/lib/props/ambiance";
static void create() {
    room::create();
    SetClimate("indoors");
    SetAmbientLight(10);
    SetShort("sewer");
    SetLong("You are in the stinking underground tunnels of "+
      "a sewer system. The air here is quite foul, "+
      "and periodic blasts of steam from wall-mounted vents make it "+
      "very hot and very humid. There is a foot-high stream of dark liquid "+
      "covering the bottom of this tunnel, running west to east along "+
      "the tunnel's length. The tunnel narrows dramatically here, and "+
      "becomes so small that further movement east is not possible.");
    SetItems( ([ ({"tunnel","tunnels"}) : "Here the tunnel becomes so "+
        "narrow that you can't go any further east. It looks like "+
        "some debris has accumulated here over time, almost "+
        "clogging the waste flow.", 
        ({"sewer","sewer system"}) : "Though evidently in good repair "+
        "and of modern construction, this is still a sewer tunnel, "+
        "and it's hostile to human comfort.",
        ({"steam","vent","vents","wall-mounted vents"}): "Apparently "+
        "there is industrial machinery nearby that exhausts hot steam "+
        "through vents in the walls.",
        "air" : "It is rank with the reek of decomposing waste.",
        ({"wall","walls"}) : "The walls of the sewer tunnel are made "+
        "of concrete and appear well-built and sturdy.", 
        ({"stream","liquid","stream of liquid","dark liquid"}) : "Whatever "+
        "this stuff is, water is not its main component. It's some kind "+
        "of foul-smelling liquid waste, flowing along the bottom "+
        "of the tunnel.",
        ({"sewage","waste","garbage"}) : "It appears that the fluid on the "+
        "bottom of this tunnel is the result of garbage and waste "+
        "processing. In liquid form, this garbage flows "+
        "east from here.",
      ]) );
    SetSmell( ([ "default" : "The stench of sewage and waste hangs here."]) );
    SetListen("default","You hear faint echoes of dripping water.");
    SetExits( ([ "west" : "/domains/campus/room/sewer1.c"
      ]) );
    SetInventory(([
        "/domains/campus/obj/debris" :1
      ]));
    set_heart_beat(10);
}
int SteamBlast(){
    object *temparr,*stuff,*lstuff;
    int i;
    stuff=all_inventory();
    lstuff = ({});
    for(i=0;i<sizeof(stuff);i++){
        temparr= ({ stuff[i] });
        if(living(stuff[i]) && !sizeof(lstuff)) lstuff = ({stuff[i]});
        if(living(stuff[i]) && sizeof(lstuff) > 0 && member_array(stuff[i],lstuff) == -1) lstuff += ({stuff[i]});
    }
    for(i=0;i<sizeof(lstuff);i++){
        if(sizeof(lstuff) > 0 && !creatorp(lstuff[i]) ) lstuff[i]->eventReceiveDamage(0, 7,random(30)+10, "torso");

    }
    tell_room(this_object(), "You are hit by a blast of scalding-hot steam!");
    return 1;
}
void heart_beat(){
    if(random(10) == 1) SteamBlast();
    return;
}
