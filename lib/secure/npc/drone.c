#include <lib.h>
#include <save.h>
#include <privs.h>
#include "/daemon/include/races.h"

int eventReceiveCommand(string butt);
int SetOwner(string hole);
string GetOwner();
int ListenUp(int foo);
int itemcheck,listening;
mixed arguments;
string owner, mm, vv, printvar,owner;
string mensaje, clase_mensaje,desc;
object dude,ww, ownerob;
void doPrint(string str1, string str2);

int doCheckLiving(object ob){
    if(living(ob) && ob->GetInvis() !=1){
	if(file_name(ob) != file_name(this_object()) )
	    desc += ob->GetShort()+" is here.\n";
    }
    return 1;
}

int doCheckItem(object ob){
    string s1;
    if(!living(ob)
      && !sscanf(file_name(ob),"/lib/std/dummy#%s",s1) ){
	if(itemcheck==0){
	    desc+="Here you see:\n";
	    itemcheck=1;
	}
	if(ob->GetInvis() != 1) desc += ob->GetShort()+"\n";
    }
    return 1;
}

int ListenUp(int i){
    listening=i;
    return 1;
    this_object()->set_heart_beat(1);
}
void init(){
    itemcheck=0;
    this_object()->set_heart_beat(1);
    this_object()->SetNoClean(1);
}
void receive_message(string s1, string s2){
    if(ownerob && listening) tell_object(ownerob,"Remote: "+s2);
}
varargs mixed eventHearTalk(object who, object target, int cls, string verb,
  string msg, string lang) {
    ww=who;
    vv=verb;
    mm=msg;
    dude=this_object()->GetShadowedObject();
    this_object()->receive_message("me",ww->GetName()+" "+vv+"s: "+mm) ;
    return;
}
varargs int doPrint(string msg, string msg_class){
    printvar=msg;
    this_object()->receive_message("me again",printvar) ;
    return 1;
}

varargs int eventPrint(string msg, string msg_class){
    mensaje = msg;
    clase_mensaje = msg_class;
    doPrint( mensaje, clase_mensaje) ;
    return 1;
}

int eventDescribeEnvironment(mixed args){
    dude=this_object()->GetShadowedObject();
    arguments = args;
    //dude->eDE(arguments) ;
    unguarded((: tell_object(ownerob,this_object()->eDE()) :));
    return 1;
}

int eventReceiveCommand(string str){
    string thing;
    if(!query_heart_beat(this_object())) this_object()->set_heart_beat(1);
    if(ownerob && str != "look" && str != "l") this_object()->eventForce(str);
    else if(ownerob){
	tell_object(ownerob,"This is where the env dsc comes in.");
	unguarded((: this_object()->eventDescribeEnvironment() :)) ;
	//ownerob->eventPrint(this_object()->eventDescribeEnvironment(this_object())) ;
    }
    else if(!ownerob) return 0;
    return 1;
}

int SetOwner(string str){
    owner=str;
    ownerob=find_player(owner);
    listening=1;
    this_object()->set_heart_beat(1);
    return 1;
}

string GetOwner(){
    if(owner) return owner;
    else return "NONE";
}


string eDE(int brief) {
    object env;
    object *invarr;
    mixed tmp;
    string *shorts;
    string smell, sound, touch;
    int i, maxi;
    dude=this_object()->GetShadowedObject();
    if(!(env = environment(this_object()->GetShadowedObject()))) {
	eventPrint(dude->GetName());
	eventPrint(file_name(dude));
	eventPrint(base_name(environment(find_object(file_name(dude)))));
	eventPrint("You are nowhere.","Room Desc");
	return;
    }
    desc = (string)env->GetObviousExits() || "";
    desc = capitalize(env->GetInternalShort() || env->GetShort() || "")
    + " [" + desc + "]\n";
    desc += (env->GetInternalLong() || env->GetLong() || "");
    smell = env->GetSmell();
    sound = env->GetListen();
    touch = env->GetTouch();
    //if(desc) eventPrint(desc, "Room DEsc");

    if( smell ) desc += "%^GREEN%^\n";
    if( sound ) desc += "%^CYAN%^\n";
    if( touch ) desc += "%^YELLOW%^\n";

    invarr = all_inventory(env);
    filter(invarr, (: doCheckLiving($1) :) );
    filter(invarr, (: doCheckItem($1) :) );
    desc += "\n";
    return desc;
}