#include <lib.h>
#include ROOMS_H
#include <daemons.h>
#include <vendor_types.h>
inherit LIB_ITEM;

string guy, stringy;
int seconds;
object ob;

void create(){
    item::create();

    SetKeyName("snooper object");
    SetId( ({"thing","item","thang","dingus","snooper","object"}) );
    SetAdjectives( ({"invisible","snooper","snoop"}) );
    SetShort("an invisible object");
    SetLong("This is an object of indeterminate nature and proportions. "
            "It is intentionally invisible, and your attempts to "
            "understand it may constitute a security breach. You'd "
            "be well advised to leave it alone.");
    SetInvis(1);
    set_heart_beat(10);
    SetNoClean(1);
    SNOOP_D->RegisterSnooper();
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
    if(!guy || !(dude = find_player(guy))) {
        eventDestruct();
    }
    if(dude && environment(dude) && base_name(environment(dude)) == ROOM_FREEZER) eventDestruct();
    if(time() - seconds > 600 ) stamp_time();
}

void receive_snoop(string str){
    stringy = str;
    unguarded((: write_file("/secure/log/adm/"+guy+".log",guy+" "+timestamp()+": "+stringy) :));
    SNOOP_D->GetSnoop(guy, stringy);
    if( file_size("/secure/log/adm/"+guy+".log") > 200000) {
        write_file("/secure/log/adm/"+guy+".log","\nEND-OF-LOG\n");
        if(!directory_exists("/secure/log/adm/archive/")) mkdir("/secure/log/adm/archive/");
        rename("/secure/log/adm/"+guy+".log", "/secure/log/adm/archive/"+guy+"."+time());
    }
}

int eventStartSnoop(string str){
    string snoopee;
    if(!str || str == "") return 0;
    snoopee = "nobody";
    str = lower_case(str);
    guy = str;

    if(!ob=find_player(str)) { write("Target not found."); return; }
    unguarded((: write_file("/secure/log/adm/snoop.err",snoop(this_object(), ob)?"":guy+": snoop failed.\n") :));
    if(query_snooping(this_object())) snoopee = identify(query_snooping(this_object()));
    SNOOP_D->RegisterSnooper();
    write_file("/secure/log/adm/"+str+".log","\nNEW SESSION: "+timestamp()+"\n");
    return 1;
}

void receive_message(string s1, string s2){
    stringy = s2;
    unguarded((: write_file("/secure/log/adm/"+guy+".log",guy+": "+stringy) :)); 
    SNOOP_D->GetSnoop(guy, stringy);
}

int eventDestruct(){
    if(base_name(previous_object()) != SNOOP_D && !archp(previous_object(2)) &&
            previous_object() != this_object()) return 0;
    SNOOP_D->UnregisterSnooper();
    return item::eventDestruct();
}

string GetSnooped(){
    if( !(master()->valid_apply(({ "PRIV_ASSIST", "PRIV_SECURE", "SNOOP_D" }))) ) return "";
    else return guy;
}
