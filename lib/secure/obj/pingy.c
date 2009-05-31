#include <lib.h>
#include <vendor_types.h>
inherit LIB_ITEM;

int count = 0;
int interval = 0;
string keepalive = "";

void validate(){
    if(!this_player() || !environment() || !(this_player() == environment())){
        error("Pingy violation. Stack: "+get_stack());
    }
}

void create(){
    ::create();

    SetKeyName("pinger");
    SetId( ({"pinger"}) );
    SetAdjectives( ({"pinger"}) );
    SetShort("a pinger");
    SetLong("This thing can be set to periodically output a string, "
            "with the purpose of keeping fickle telnet connections up. "
            "\nTo set the interval: interval <heartbeats> \n"
            "To set the string: vassily <string>\n"
            "\nNote that this object is deprecated in favor of the "
            "keepalive command.");
    SetNoCondition(1);
    SetMass(20);
    SetBaseCost("silver",10);
    set_heart_beat(1);
    SetVendorType(VT_TREASURE);
}

int SetInterval(string str){
    validate();
    interval = atoi(str);
    if(!intp(interval)){
        write("That is not a valid integer.");
        return 1;
    }

    write("Interval set to "+interval+" heartbeats.");
    return 1;
}

int SetKeepalive(string str){
    validate();
    if(!str || str == "") keepalive = "\n";
    else keepalive = str;
    write("Keepalive set.");
    return 1;
}

void init(){
    ::init();
    add_action("SetInterval","interval");
    add_action("SetKeepalive","vassily");
}

void heart_beat(){
    count++;
    if(interval > 0 && count > interval){
        count = 0;
        if(living(environment(this_object())) &&
                true()){
            if(query_idle(environment(this_object())) > 240){
                if(environment(this_object()))
                    environment(this_object())->eventPrint(keepalive);
                //Do not use the below line unless you understand
                //the security hazard it poses
                //environment(this_object())->eventForce(keepalive);
            }
        }
    }
}
