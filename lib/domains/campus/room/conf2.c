#include <lib.h>
#include ROOMS_H

inherit LIB_ROOM;

int privacy, timer;
string ExtraDesc();
static void create() {
    string privs;
    privs = query_privs();
    if( privs ) privs = capitalize(privs);
    else privs = "a creator";
    room::create();
    SetClimate("indoors");
    SetAmbientLight(40);
    SetShort("Conference Room");
    SetLong((: ExtraDesc :));
    SetExits( ([ 
                ]) );
    SetInventory(([
                "/domains/town/obj/bbucket" : 1,
                "/domains/town/obj/chair" : 4,
                "/domains/campus/obj/podium" : 1,
                ]));

    SetProperties(([
                "no attack" : 1,
                "nopeer" : 1,
                ]));
    privacy=0;
    set_heart_beat(20);
    timer = 0;

}

int AutoDeactivate(){
    message("info","%^RED%^The privacy field shuts off.%^RESET%^", this_object());
    timer = 0;
    privacy = 0;
    return 1;
}

void init(){
    ::init();
    add_action("report_time","timer");
}

void heart_beat(){
    if(timer && (time() - timer ) > 1200) AutoDeactivate();
}

int report_time(){
    int secs = time() - timer;

    if(!timer){
        write("Privacy field is not active.");
        return 1;
    }

    write("Elapsed seconds: "+secs);
    write("Elapsed minutes: "+(secs/60));
    return secs;
}

int CanReceive(object ob) {
    if(privacy){
        if(!interactive(ob)) { 
            message("info","\n\nPRIVACY WARNING: "+ob->GetName()+" has entered the room.\n\n",this_object() );
        }
        else if(!archp(ob)){
            message("info","You bounce off the conference room privacy shield.", ob);
            message("info",ob->GetName()+" bounced off the privacy shield.",this_object());
            if(!environment(ob)) ob->eventMoveLiving(ROOM_START);
            return 0;
        }

    }
    return ::CanReceive(ob);
}

int set_privacy(int i){
    if(environment(this_player()) != this_object() && !archp(this_player())) {
        write("You lack the adequate privileges to do that.");
        say(this_player()->GetName()+" is trying to mess around with the privacy shield system.");
        return 1;
    }
    privacy=i;
    timer = time();
    return 1;
}
/*
   int privacy(string str){
   if(environment(this_player()) != this_object() && !archp(this_player())) {
   write("You lack the adequate privileges to do that.");
   say(this_player()->GetName()+" is trying to muck around with the privacy shield system.");
   return 1;
   }

   if(str=="on" || str == "1"){
   this_object()->set_privacy(1);
   write("You enable the privacy shield.\n");
   say(this_player()->GetName()+" enables a privacy force field around the room.");
   timer = time();
   return 1;
   }
   if(str=="off" || str == "0"){
   this_object()->set_privacy(0);
   write("You disable the privacy shield.\n");
   say(this_player()->GetName()+" disables a privacy force field around the room.");
   timer = 0;
   return 1;
   }
   }
 */
int get_privacy(){
    return privacy;
}

string ExtraDesc(){
    string extra = "%^YELLOW%^A privacy force field is active around this room.%^RESET%^";
    string desc = "This is an enchanted room, with the magical power to prevent uninvited people from entering. It is used for meetings where three or more people need to share information without interruption or privately. To enable privacy, 'privacy on'. To disable it, 'privacy off'. The privacy field automatically deactivates after approximately 20 minutes.\n";
    if(privacy) return desc+extra;
    else return desc+"%^RED%^The privacy field is DISABLED.";
}
