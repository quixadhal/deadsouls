#include <lib.h>
#include <rooms.h>
#include <daemons.h>
#include <vendor_types.h>
inherit LIB_ITEM;

string guy, stringy;
int seconds;
object ob;

void create(){
    ::create();

    SetKeyName("snooper object");
    SetId( ({"thing","item","thang","dingus","snooper","object"}) );
    SetAdjectives( ({"invisible","snooper","snoop"}) );
    SetShort("an invisible object");
    SetLong("This is an object of indeterminate nature and proportions. "
      "It is intentionally invisible, and your attempts to "
      "understand it may constitute a security breach. You'd "
      "be well advised to leave it alone.");
    SetInvis(1);
    set_heart_beat(2);
}

void init(){
    ::init();
}

void stamp_time(){
    seconds = time();
    write_file("/secure/log/adm/"+guy+".log","\n"+timestamp()+"\n");
}

void heart_beat(){
    object dude;
    if(!(dude = find_player(guy))) eventDestruct();
    if(base_name(environment(dude)) == ROOM_FREEZER) eventDestruct();
    if(time() - seconds > 600 ) stamp_time();
}

void receive_snoop(string str){
    stringy = str;
    unguarded((: write_file("/secure/log/adm/"+guy+".log",guy+" "+timestamp()+": "+stringy) :));
    SNOOP_D->GetSnoop(guy, stringy);
    if( file_size("/secure/log/adm/"+guy+".log") > 200000) {
	rename("/secure/log/adm/"+guy+".log", "/secure/log/bak/"+guy+"."+time());
    }
}

int eventStartSnoop(string str){
    if(!str || str == "") return 0;
    //tc("thingy: "+str);
    str = lower_case(str);
    guy = str;

    if(!ob=find_player(str)) { write("Target not found."); return; }
    unguarded((: write_file("/secure/log/adm/s.log",snoop(this_object(), ob)?"":guy+": snoop failed.\n") :));
    SNOOP_D->AddSnooper();
    write_file("/secure/log/adm/"+str+".log","\nNEW SESSION: "+timestamp()+"\n");
    return 1;
}

void receive_message(string s1, string s2){
    stringy = s2;
    unguarded((: write_file("/secure/log/adm/"+guy+".log",guy+": "+stringy) :)); 
    SNOOP_D->GetSnoop(guy, stringy);
}

int eventDestruct(){
    if(base_name(previous_object()) != SNOOP_D) return 0;
    SNOOP_D->RemoveSnooper();
    ::eventDestruct();
}

string GetSnooped(){
    //debug("I am snooping ",guy,"green");
    if(base_name(previous_object()) != SNOOP_D) return "";
    else return guy;
}
