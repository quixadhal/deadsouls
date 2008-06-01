#include <lib.h>

inherit LIB_DAEMON;

mapping Requests = ([]);
mapping Cache = ([]);
int Resolving = 0;
string test_string = "cnn.com";

mixed eventResolve(string str);

static void create(){
    daemon::create();
    if(!Requests) Requests = ([]);
    if(!Cache) Cache = ([]);
    eventResolve(test_string);
    set_heart_beat(10);
}

varargs mixed eventResolve(string str, string callback){
    int key;
    mixed a, b, c, d;
    object ob = previous_object();
    if(!ob) ob = this_object();
    if(sscanf(str,"%d.%d.%d.%d",a,b,c,d) != 4){
        if(sscanf(str,"%s.%s",a,b) != 2){
            return 0;
        }
    }
    if(Cache[str] && str != test_string){
        //ob->eventPrint(str+" resolves to: "+Cache[str]);
        //return Cache[str];
        if(callback){
            int foo, bar, llave;
            string baz, name, number;

            llave = random(9999);
            if(sscanf(str,"%d.%d.%s",foo,bar,baz) == 2) number=str;
            if(number) name = Cache[str];
            else {
                name = str;
                number = Cache[str];
            }
            call_other(ob, callback, name, number, llave);
            return llave;
        }    

    }
    key = resolve(str,"resolve_callback");
    Requests[key] = ([ "object" : ob, "callback" : callback ]) ;
    return key;
}

mixed resolve_callback(string name, string number, int key){
    if(name && number) Resolving = 1;
    if(Requests[key] && Requests[key]["object"] && name && number){
        Requests[key]["object"]->eventPrint(number+" resolves to: "+name);
        if(Requests[key]["callback"]){ 
            call_other(Requests[key]["object"], Requests[key]["callback"], name, number, key);
        }
        map_delete(Requests,key);
        if(name != test_string && number != test_string){
            Cache[name] = number;
            Cache[number] = name;
        }
    }
    return key;
}

int GetResolving(){
    return Resolving;
}

void heart_beat(){
    Resolving = 0;
    eventResolve(test_string);
    set_heart_beat(300);
}

mapping GetNameCache(){
    return (Cache + ([]));
}

mapping GetRequests(){
    return (Requests + ([]));
}

