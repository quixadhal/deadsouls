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
    SetShort("a pinger");
    SetLong("This thing can be set to periodically output a string, "
            "with the purpose of keeping fickle telnet connections up. "
            "\nTo set the interval: interval <heartbeats> \n"
            "To set the string: vassily <string>"
            "\n\nNote: This object is deprecated. Use the command "
            "'keepalive' instead.");
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
    add_action("SetKeepalive","vassily");
}

void heart_beat(){
    count++;
    if(interval > 0 && count > interval){
        count = 0;
        if(living(environment(this_object())) &&
                true()){
#ifndef __DSLIB__
            tell_object(environment(this_object()),keepalive);
#else
            send_nullbyte(environment(this_object()));
#endif
        }
    }
}
