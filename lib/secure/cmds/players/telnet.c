#include <lib.h>
#include <daemons.h>

inherit LIB_DAEMON;

mapping NamesMap = ([]);

mixed cmd(string args) {
    string ip_string, port_string, ip;
    int d1, d2, d3, d4, port;

    if(sscanf(args,"%s %s",ip_string, port_string) != 2){
        this_object()->help();
        return 1;
    }

    if(sscanf(ip_string,"%d.%d.%d.%d",d1,d2,d3,d4) != 4 && 
            !RESOLV_D->GetResolving()){
        write("This mud is not using name resolution. Use a "+
                "numerical ip address, like 11.22.33.44");
        return 1;
    }

    if(!atoi(port_string)){
        write("The port must be numerical, such as: 6666");
        return 1; 
    }

    if(d4){
        this_object()->eventStartConnection(this_player(), args);
        return 1;
    }

    write("Attempting to resolve \""+ip_string+"\". If this fails, "+
            "try using a numerical ip address, like 1.2.3.4");
    NamesMap[ip_string] = ([ "dude": this_player(), "port" : port_string ]);
    RESOLV_D->eventResolve(ip_string, "resolve_callback");
    return 1;
}

void resolve_callback(string name, string number, int key){
    string cle, port;
    object ob;
    if(NamesMap[number]){
        cle = number;
    }
    else if(NamesMap[name]){
        cle = name;
    }
    else return;

    ob = NamesMap[cle]["dude"];
    port = NamesMap[cle]["port"];
    map_delete(NamesMap, cle);
    if(ob){
        ob->eventPrint(number+" resolves to: "+name);
        this_object()->eventStartConnection(ob, number + " " + port);
    }
}

int eventStartConnection(object who, string where){
    object client = new("/secure/obj/tc");
    if(!client || !who) return 0;

    if(!telnet_privp(who)){
        who->eventPrint("You are not a member of the group of users permitted "
                "to use this mud's telnet facility.");
        return 1;
    }
    client->SetConnection(where);
    client->eventMove(who);
    client->do_connect(where, who);
    return 1;
}

string GetHelp(){
    return ("Syntax: telnet <ip address> <port>\n\n"
            "If you are in the TELNET group, this connects you to another "
            "computer or mud on the ip and port specified.");
}
