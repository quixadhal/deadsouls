#include <lib.h>
#include <vendor_types.h>
inherit LIB_ITEM;

int count, state, blocked, short, logged;
string message, announce, name, final, cratty, f_global;
object owner;

int read_it(string str);

void create(){
    ::create();
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
    SetDefaultRead("read_it");
    SetProperties(([
        "no steal" : 1,
      ]));
    SetMass(1);
    SetValue(1);
    SetVendorType(VT_TREASURE);
    state=0;
    count = 0;
    set_heart_beat(5);
}

void init(){
    ::init();
    add_action("toggle_answer","answer");
    add_action("check_it","check");
    add_action("set_ann","announce");
    add_action("check_mess","tells");
    add_action("erase","erase");
    add_action("arch_it","archive");
    this_object()->get_ann();
}

void heart_beat(){
    count++;
    if(count > 720){
        count = 0;
        if(creatorp(environment(this_object())) &&
          file_exists("/realms/"+name+"/messages/messages")){
            tell_object(environment(this_object()),"The answering machine %^BOLD%^YELLOW%^BEEPS%^RESET%^.");
        }
    }
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
        write("The answering machine is off.\n");
        return 1;
    }
}

int query_answer(){
    return state;
}

int get_message(string str){
    cratty=str;
    this_object()->final();
    return 1;
}

string send_message(){
    return announce;
}

int log_it(string str){
    f_global=str;
    owner=environment(this_object());
    name=lower_case(owner->GetKeyName());
    if(file_size("/realms/"+name+"/messages") > 0) {
        cp("/realms/"+name+"/messages","/realms/"+name+"/messages.bak");
        rm("/realms/"+name+"/messages");
    }
    if(file_size("/realms/"+name+"/messages") != -2) {
        mkdir("/realms/"+name+"/messages");
    }
    unguarded((: write_file("/realms/"+name+"/messages/messages", f_global+"\n") :));
    return 1;
}

int final(){
    string tempy,a1,a2;
    if(sscanf(cratty,"%s tells you: %s", a1, a2) >1){
        tempy="From "+a1+" at "+local_ctime(time())+":\n"+a2;
    }
    if(!blocked){
        message("info",final+"\n", environment(this_object()));
    }
    if(short){
        //message("info",a1+" telled to you.", environment(this_object()));
    }
    if(logged){
        this_object()->log_it(tempy);
    }
}

int set_ann(string str){
    if(!state){
        write("The machine is not turned on.\n");
        return 1;
    }
    announce=str;
    rm("/realms/"+name+"/messages/annc");
    write_file("/realms/"+name+"/messages/annc", announce);
    return 1;
}

int get_ann(){
    owner=environment(this_object());
    if(creatorp(owner)){
        name=lower_case(owner->GetKeyName());
        if(file_size("/realms/"+name+"/messages/annc") > 0){
            announce=read_file("/realms/"+name+"/messages/annc");
            return 1;
        }
        announce=owner->GetName()+" cannot "
        "answer your tell right now. Your message has been recorded "
        "and "+nominative(owner)+" will get back to you as soon as possible.";
        return 1;
    }
}

int check_mess(string str){
    if(!str){
        this_player()->more("/realms/"+name+"/messages/messages");
        return 1;
    }
}

int read_it(string str){
    write("\n"
      "answer on/off   - on activates the machine. off doesn't.\n"
      "announce <msg>  - '<msg>' is whatever you want the sender to see.\n"
      "tells           - lists messages by date, time and sender.\n"+
      "erase tape      - erases all messages on the machine.\n"
      "archive tape    - copies tape onto backup archive file.\n"
      "\n");
    return 1;
}

int erase(string str){
    if(str=="tape"){
        write("You erase the answering machine tape.\n");
        rm("/realms/"+name+"/messages/messages");
        return 1;
    }
}

int arch_it(string str){
    string temp;
    if(str=="tape"){
        write("You save the contents of the answering machine tape "
          "into /realms/"+name+"/messages/archive.\n");
        temp=read_file("/realms/"+name+"/messages/messages");
        write_file("/realms/"+name+"/messages/archive", temp);
        return 1;
    }
}

int clean_tape(string str){
    if(str=="tape"){
        this_object()->arch_it("tape");
        this_object()->erase("tape");
        return 1;
    }
}
