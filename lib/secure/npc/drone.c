#include <lib.h>
#include <save.h>
#include <privs.h>

int eventReceiveCommand(string butt, string munch);
//varargs int SetOwner(string str, mixed arg);
int SetOwner(string str);
string GetOwner();
int ListenUp(int foo);
int itemcheck,listening;
mixed arguments;
string owner, mm, vv, printvar,owner;
string mensaje, clase_mensaje,desc,control_code;
object dude,ww, ownerob;
void doPrint(string str1, string str2);

void validate(){
    if(base_name(previous_object()) != "/secure/obj/control" || 
            previous_object()->GetControlCode() != control_code){
        if(ownerob){
            tell_object(ownerob,"%^RED%^Security violation. Someone is attempting to "+
                    "hijack your drone. Guilty object stack: %^YELLOW%^"+
                    identify(previous_object(-1))+"%^RESET%^");
        }
        error("Illegal control attempt by: "+identify(previous_object(-1))+", "+get_stack());
    }
    return;
}

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
    unguarded((: tell_object(ownerob,this_object()->eDE()+"\n") :));
    return 1;
}

int eventReceiveCommand(string str){
    string thing;

    validate();

    if(!query_heart_beat(this_object())) this_object()->set_heart_beat(1);
    if(ownerob && str != "look" && str != "l") this_object()->eventForce(str);
    else if(ownerob){
        unguarded((: this_object()->eventDescribeEnvironment() :)) ;
    }
    else if(!ownerob) return 0;
    return 1;
}

varargs int SetOwner(string str, mixed arg){
    if(sizeof(owner) && this_object()->GetOwner() != "NONE") {
        validate();
    }
    if(str == "NONE") {
        control_code = "";
        owner = "NONE";
        ownerob = 0;
        listening = 0;
        return 1;
    }
    owner=str;
    if(arg && objectp(arg)) ownerob=arg;
    else ownerob=find_player(owner);
    listening=1;
    this_object()->set_heart_beat(1);
    return 1;
}

string GetOwner(){
    if(owner) return owner;
    else return "NONE";
}


int SetControlCode(string str){
    if(sizeof(control_code) && GetOwner() != "NONE" ){
        if(ownerob){
            tell_object(ownerob,"%^RED%^Security violation. Someone is attempting to "
                    "hijack your drone. Guilty object stack: %^YELLOW%^"+
                    identify(previous_object(-1))+"%^RESET%^");
        }
        error("Illegal control attempt by: "+identify(previous_object(-1))+", "+get_stack());
    }
    else control_code = str;
    return 1;
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
    desc = env->GetObviousExits() || "";
    desc = capitalize(env->GetInternalShort() || env->GetShort() || "")
        + " [" + desc + "]\n";
    desc += (env->GetInternalLong() || env->GetLong() || "");
    smell = env->GetSmell();
    sound = env->GetListen();
    touch = env->GetTouch();

    if( smell ) desc += "%^GREEN%^\n";
    if( sound ) desc += "%^CYAN%^\n";
    if( touch ) desc += "%^YELLOW%^\n";

    invarr = all_inventory(env);
    filter(invarr, (: doCheckLiving($1) :) );
    filter(invarr, (: doCheckItem($1) :) );
    desc += "\n";
    return desc;
}

int GetDrone() { return 1; }
