#include <lib.h>
#include <vendor_types.h>
inherit LIB_ITEM;
inherit LIB_INSTALL;

varargs int eventCloak(mixed arg);
varargs int eventDecloak(mixed arg);

int power = 0;
function f1 = 0;
function f2 = 0;
mapping SendMap = ([]);

void create(){
    item::create();

    SetKeyName("data module");
    SetId( ({"module", "cartridge"}) );
    SetAdjectives( ({"data","yautja","stealth"}) );
    SetShort("a Yautja data module");
    SetLong("This is a small cartridge that contains programming intended "
            "for a Yautja wrist computer. It appears to have writing on it.");
    SetNoCondition(1);
    SetMass(1);
    SetReads( ([
                "default" : "Stealth module: provides cloaking and decloaking.",
                "writing": "Stealth module: provides cloaking and decloaking.",
                ]) );
    SetLanguage("Yautja");
    SetBaseCost("silver",10);
    SetVendorType(VT_TREASURE);
}

varargs int eventDecloak(mixed arg){
    int ret;
    string s1,s2;
    string *hist = environment(this_object())->GetHist();
    if(!arg) arg = this_player();
    if(!environment() || !(environment()->GetWorn())){
        write("You are not wearing the wrist computer.");
        return 1;
    }
    if(!power){
        write("The computer is not active.");
        return 1;
    }
    //write("%^GREEN%^last command:%^RESET%^ "+arg->GetCurrentCommand());
    arg = arg->GetCurrentCommand();
    if(sscanf(arg,"%s %s",s1,s2) == 2){
        if(s2 == "on" || s2 == "disable"){
            eventCloak(this_player());
            return 1;
        }
    }
    if(!(this_player()->GetInvis())){
        write("Your wrist computer chirps, and nothing happens.");
        say(this_player()->GetName()+"'s wrist computer chirps.");
        return 1;
    }
    write("Your wrist computer chirps, and you become visible.");
    say(this_player()->GetName()+"'s wrist computer chirps, and "+
            capitalize(this_player()->GetKeyName())+" fades into view.");
    this_player()->SetInvis(0);
    return 1;
}

varargs int eventCloak(mixed arg){
    int ret;
    string s1,s2;
    string *hist = environment(this_object())->GetHist();
    if(!arg) arg = this_player();
    if(!environment() || !(environment()->GetWorn())){
        write("You are not wearing the wrist computer.");
        return 1;
    }
    if(!power){
        write("The computer is not active.");
        return 1;
    }
    //write("%^YELLOW%^last command:%^RESET%^ "+arg->GetCurrentCommand());
    arg = arg->GetCurrentCommand();
    if(sscanf(arg,"%s %s",s1,s2) == 2){
        if(s2 == "off" || s2 == "disable"){
            eventDecloak(this_player());
            return 1;
        }
    }
    if(this_player()->GetInvis()){
        write("Your wrist computer chirps, and nothing happens.");
        say(this_player()->GetName()+"'s wrist computer chirps.");
        return 1;
    }
    write("Your wrist computer chirps, and you become transparent.");
    say(this_player()->GetName()+"'s wrist computer chirps, and "+nominative(this_player())+
            " fades from view.");
    this_player()->SetInvis(1);
    return 1;
}

varargs mixed eventInstall(object what, object where, int auto){
    f1 = (: eventCloak(this_player()) :);
    f2 = (: eventDecloak(this_player()) :);
    if(!where){
        write("Install it where?");
        return 1;
    }
    SendMap = ([
            "cloaking" : ([ "function" : f1, "hook" : "cloak" ]),
            "decloaking" : ([ "function" : f2, "hook" : "decloak" ])
            ]);
    where->eventInstallModule(SendMap,auto ||0);
    power = 1;
    return 1;
}

varargs mixed eventUninstall(object what, mixed auto){
    int success;
    what->eventUninstallModule(this_object(), auto || 0);
    if(this_player() && environment() && environment() == what)
        success = this_object()->eventMove(this_player());
    if(success || auto) return 1;
    else {
        //if(!success) write("The uninstall failed.");
        return 0;
    }
}

string Report(){
    string ret = "";
    if(!sizeof(SendMap)) return ret;
    foreach(mixed key, mixed val in SendMap){
        ret += "Functionality: "+key+", command: "+val["hook"]+"\n";
    }
    return ret;
}

int eventPowerOff(){
    object whom = this_player();
    object env = environment();
    power = 0;
    if(env) env->eventUninstallModule(this_object(),1);
    else return 0;
    if(!whom) whom = environment(env);
    if(!whom) return 0;
    else {
        if(living(whom)){
            if(whom->GetInvis() && !creatorp(whom)){
                whom->SetInvis(0);
                tell_object(whom, "Your wrist computer makes a croaking noise, and you become visible.");
                tell_room(environment(whom),whom->GetName()+"'s wrist computer makes a croaking noise, and "+
                        capitalize(whom->GetKeyName())+" fades into view.", whom);
            }
        }
    }
    return 1;
}

int eventPowerOn(){
    object env = environment();
    power = 1;
    if(env) env->eventInstallModule(SendMap,1);    
    return 1;
}

void init(){
    ::init();
}
