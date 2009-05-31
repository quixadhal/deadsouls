#include <lib.h>
#include <commands.h>
#include <daemons.h>
#include <vendor_types.h>
inherit LIB_ITEM;

int count, state, blocked, short, logged, forwarding;
int lastchecked, lastmessage;
string message, announce, name, final, tempy, cratty, f_global, forwardee;
object owner;

int read_it(string str);

int validate(){
    object prev = previous_object();
    if(prev && prev->GetForced()) return 0;
    if(this_player() && this_player()->GetForced()) return 0;
    if(!name || (owner && this_player() == owner)) return 1;
    return 0;
}

void create(){
    ::create();
    AddSave(({"state","forwarding","name","forwardee","announce","message",
                "lastchecked","lastmessage"}));
    SetKeyName("answering machine");
    SetId( ({"machine","voicemail"}) );
    SetShort("an answering machine");
    SetLong("This is a portable answering machine. There is a label on "
            "it you can read.");
    SetItems(([
                "label" : "A label you can read.",
                ]));
    SetRead(([
                "label" : (: read_it :),
                ]));
    SetDefaultRead("Try: read label on machine");
    SetProperties(([
                "no steal" : 1,
                ]));
    SetMass(1);
    SetVendorType(VT_TREASURE);
    state=0;
    count = 0;
    set_heart_beat(10);
}

void init(){
    ::init();
    add_action("toggle_answer","answer");
    add_action("check_it","check");
    add_action("set_ann","announce");
    add_action("check_mess","tells");
    add_action("erase","erase");
    //add_action("tellforward","tellforward");
    add_action("arch_it","archive");
    if(owner){
        this_object()->get_ann();
    }
    else {
        if(name && owner) return;
        if(environment() && interactive(environment())){
            unguarded( (: owner = environment() :) );
            unguarded( (: name =  owner->GetKeyName() :) );
        }
    }
}

void heart_beat(){
    count++;
    if(!clonep(this_object())) return;
    if(name && !owner){
        if(!(owner = unguarded( (: find_player(name) :) ))){
            this_object()->eventDestruct();
        }
    }
    if(count > 36){
        count = 0;
        if(lastchecked < lastmessage){
            if(environment()){
                environment()->eventPrint("The answering machine %^GREEN%^"+
                        "BEEPS%^RESET%^.");
            }
        }
    }
}

int tellforward(string str){
    object env = environment();
    //This functionality is incomplete
    return 0;
    if(!validate()) return 0;
    if(!env || !this_player() || env != this_player() ){
        return 0;
    }
    if(!str){
        if(forwardee && forwarding){
            write("You are forwarding tells to "+forwardee+".");
        }
        else {
            write("There is no current forwarding recipient.");
        }
        return 1;
    }
    if(str == "off"){
        write("Disabling tell forwarding.");
        forwarding = 0;
        forwardee = 0;
        return 1;
    }
    write("You are now forwarding tells to: "+str);
    forwardee = str;
    forwarding = 1;
    return 1;
}

int toggle_answer(string str){
    if(str=="on"){
        if(state==1){
            write("The answering machine is already on.\n");
            return 1;
        }
        state=1;
        blocked=1;
        short=1;
        logged=1;
        unguarded( (: owner = this_player() :) );
        unguarded( (: name = this_player()->GetKeyName() :) );
        this_object()->log_it("*** Answering machine activated on "+
                local_ctime(time())+".\n");
        write("The answering machine is on.\n");
        return 1;
    }
    if(str=="off"){
        if(state==0){
            write("The machine is already turned off.\n");
            return 1;
        }
        state=0;
        blocked=0;
        short=0;
        logged=0;
        name=0;
        owner=0;
        cratty=0;
        tempy=0;
        forwardee=0;
        forwarding=0;
        write("The answering machine is off.\n");
        return 1;
    }
}

int query_answer(){
    return state;
}

int do_tell(string wut){
    owner->eventForce(wut);
    return 1;
}

int get_message(string str){
    object prev = previous_object();
    if(!prev || 
            (base_name(prev) != CMD_TELL && base_name(prev) != SERVICES_D)){ 
        return 0;
    }
    cratty="";
    if(forwarding && forwardee){
        string localtemp;
        cratty += "(forwarded to "+forwardee+") ";
        localtemp = "tell "+forwardee+" "+str;
        tempy = replace_string(localtemp,"tells you","");
        unguarded( (: do_tell, tempy :) );
    }
    tempy = 0;
    cratty += str;
    this_object()->final();
    lastmessage = time();
    return 1;
}

string send_message(){
    if(!sizeof(announce)) this_object()->get_ann(); 
    return announce;
}

static int log_it(string str){
    f_global=str;
    name=lower_case(owner->GetKeyName());
    unguarded((: write_file(homedir(owner)+"/log/messages", f_global+"\n") :));
    return 1;
}

static int final(){
    tempy = local_ctime(time())+":\n"+cratty+"\n\n";
    if(!blocked){
        message("info",final+"\n", environment(this_object()));
    }
    if(logged){
        unguarded( (: this_object()->log_it(tempy) :) );
    }
    cratty = 0;
    tempy = 0;
    return 1;
}

int set_ann(string str){
    if(!validate()) return 0;
    if(!state){
        write("The machine is not turned on.\n");
        return 1;
    }
    announce=str;
    rm(homedir(owner)+"/log/annc");
    write_file(homedir(this_player())+"/log/annc", announce);
    return 1;
}

int get_ann(){
    if(file_size(homedir(owner)+"/log/annc") > 0){
        unguarded( (: announce=read_file(homedir(owner)+"/log/annc") :) );
    }
    if(!sizeof(announce)){
        announce=capitalize(name)+" cannot "+
            "answer your tell right now. Your message has been recorded "+
            "and "+nominative(owner)+" will get back to you as soon as possible.";
    }
    return 1;
}

int check_mess(string str){
    if(!validate()) return 0;
    if(!str){
        lastchecked = time();
        owner->more(homedir(owner)+"/log/messages");
        return 1;
    }
}

string read_it(string str){
    string ret = "\n"+
        "answer on/off   - on activates the machine. off doesn't.\n"+
        "announce <msg>  - '<msg>' is whatever you want the sender to see.\n"+
        "tells           - lists messages by date, time and sender.\n"+
        "erase tape      - erases all messages on the machine.\n"+
        "archive tape    - copies tape onto backup archive file.\n"+
        //"tellforward <name>  - Forwards any tells you receive to the "+
        //"person specifed. Use with GREAT caution.\n"+
        "\n"+
        "\n";
    return ret;
}

int erase(string str){
    if(!validate()) return 1;
    if(str=="tape"){
        write("You erase the answering machine tape.\n");
        rm(homedir(owner)+"/log/messages");
        return 1;
    }
}

int arch_it(string str){
    string temp;
    if(!validate()) return 0;
    if(str=="tape"){
        write("You save the contents of the answering machine tape "
                "into "+homedir(owner)+"/log/archive.\n");
        temp=read_file(homedir(owner)+"/log/messages");
        write_file(homedir(owner)+"/log/archive", temp);
        return 1;
    }
}

int clean_tape(string str){
    if(!validate()) return 0;
    if(str=="tape"){
        this_object()->arch_it("tape");
        this_object()->erase("tape");
        return 1;
    }
}
