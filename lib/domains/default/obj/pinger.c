#include <lib.h>
#include <vendor_types.h>
inherit LIB_ITEM;

int count = 0;
int interval = 0;
string keepalive = "";

void create(){
    ::create();

    SetKeyName("pinger");
    SetId( ({"pinger"}) );
    SetAdjectives( ({"keepalive"}) );
    SetShort("a keepalive pinger");
    SetLong("This thing can be set to periodically output a string, "
      "with the purpose of keeping fickle telnet connections up. "
      "\nTo set the interval: interval <heartbeats> \n"
      "To set the string: keepalive <string>");
    SetNoCondition(1);
    SetMass(20);
    AddSave( ({ "count", "interval", "keepalive" }) );
    set_heart_beat(1);
}

int SetInterval(string str){
    interval = atoi(str);
    if(!intp(interval)){
        write("That is not a valid integer.");
        return 1;
    }

    write("Interval set to "+interval+" heartbeats.");
    return 1;
}

int SetKeepalive(string str){
    if(!str || str == "") keepalive = "\n";
    else keepalive = str;
    write("Keepalive set.");
    return 1;
}

void init(){
    ::init();
    add_action("SetInterval","interval");
    add_action("SetKeepalive","keepalive");
}

void heart_beat(){
    count++;
    if(interval > 0 && count > interval){
        count = 0;
        if(living(environment(this_object())) &&
          true()){
            //query_idle(environment(this_object())) > 240){
            tell_object(environment(this_object()),keepalive);
        }
    }
}
