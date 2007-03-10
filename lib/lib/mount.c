#include <lib.h>
#include <daemons.h>
#include <position.h>
#include <armor_types.h>
#include <message_class.h>
#include <vision.h>
#include "include/npc.h"

private int Mount;
//string DismountedShort = this_object()->GetShort();
//string MountedShort = "";
//string DismountedLong = this_object()->GetLong();
//string MountedLong = "";

object *Riders = ({});

mixed direct_ride_str(){
    return this_object()->GetMount();
}

mixed direct_ride_word_str(){
    return this_object()->GetMount();
}

mixed direct_mount_liv(){
    return this_object()->GetMount();
}

mixed direct_dismount_liv(){
    return this_object()->GetMount();
}

mixed direct_dismount_from_liv(){
    return this_object()->GetMount();
}
int eventRide(string direction){
    string travel_cmd, s1, s2;
    object *guys = get_livings(this_object());
    if(interactive(this_object())) return 0;
    if(sscanf(direction,"%s %s",s1,s2) == 2){
	if(s1 == "enter" || s1 == "into") travel_cmd = "enter";
    }

    if(!s1 || s1 == "") switch(this_object()->GetPosition()){
    case POSITION_STANDING : travel_cmd = "go";break;
    case POSITION_SITTING : travel_cmd = "crawl";break;
    case POSITION_LYING : travel_cmd = "crawl";break;
    case POSITION_FLYING : travel_cmd = "fly";break;
    default : travel_cmd = "go";
    }
    else direction = s2;
    this_object()->eventForce(travel_cmd+" "+direction);
    return 1;
}

object *AddRider(object ob){
    Riders += ({ ob });
}

object *RemoveRider(object ob){
    Riders -= ({ ob });
}

object *GetRiders(){
    object *ret = ({});
    foreach(mixed rider in Riders){
	if(!rider) RemoveRider(rider);
	if(environment(rider) == this_object()) ret += ({ rider });
	else RemoveRider(rider);
    }
    return ret;
}

int SetMount(int x) {
    Mount = x;
    return Mount;
}

int GetMount(){ return Mount; }

mixed eventMount(object who){
    int rider_weight;
    if(!who) return 0;
    rider_weight = (who->GetCarriedMass()) + (who->GetMass() || 2000);
    if(!environment(this_object())) return 0;
    if(environment(who) && environment(who) == this_object()){
	return write("You are already mounted.");
    }
    if(rider_weight + this_object()->GetCarriedMass() > this_object()->GetMaxCarry()){
	return write("This mount cannot handle that much weight.");
    }
    if(this_object()->GetMountOwner() != who){
	write(this_object()->GetName()+" doesn't know you well enough to let "+
	  "you ride "+objective(this_object())+".");
	return 0;
    }
    else {
	this_object()->SetNoClean(1);
	write("You mount "+this_object()->GetShort()+".");
	say(who->GetName()+" mounts "+this_object()->GetShort()+".");
	who->SetProperty("mount", this_object());
	if(who->eventMove(this_object())) return AddRider(who);
	else return 0;
    }
}

mixed eventDismount(object who){
    int rider_weight;
    rider_weight = (who->GetCarriedMass()) + (who->GetMass() || 2000);
    if(!environment(this_object())) return 0;
    if(environment(who) && environment(who) != this_object()){
	return write("You are already dismounted.");
    }
    else {
	write("You dismount from "+this_object()->GetPlainShort()+".");
	tell_room(environment(this_object()),who->GetName()+" dismounts from " +this_object()->GetPlainShort()+".", ({ this_object() }));
	who->RemoveProperty("mount");
	if(who->eventMove(environment(this_object()))) return RemoveRider(who);
	else return 0;
    }
}

mixed eventBuck(object who){
    int rider_weight;
    rider_weight = (who->GetCarriedMass()) + (who->GetMass() || 2000);
    if(!environment(this_object())) return 0;
    if(environment(who) && environment(who) != this_object()){
	return 0;
    }
    else {
	tell_player(who,"You are thrown from"+this_object()->GetPlainShort()+"!");
	tell_room(environment(this_object()),who->GetName()+" is thrown from " +this_object()->GetPlainShort()+"!", ({ this_object() }));
	who->RemoveProperty("mount");
	who->SetPosition(POSITION_LYING);
	if(who->eventMove(environment(this_object()))) return RemoveRider(who);
	else return 0;
    }
}
