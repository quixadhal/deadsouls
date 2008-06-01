#include <lib.h>
#include <daemons.h>

inherit LIB_DAEMON;

mapping NamesMap = ([]);

varargs int cmd(string str)
{
    if(!RESOLV_D->GetResolving()){
        write("This mud is not using name resolution.");
        return 1;
    }

    write("Querying for information on "+str+"...");
    RESOLV_D->eventResolve(str, "resolve_callback");
    NamesMap[str] = this_player();
    return 1;
}

void resolve_callback(string name, string number, int key){
    string cle;
    object ob;
    if(NamesMap[number]) cle = number;
    else if(NamesMap[name]) cle = name;
    ob = NamesMap[cle];

    map_delete(NamesMap, cle);

    if(ob){
        ob->eventPrint(number+" resolves to: "+name);
    }
}

int help()
{
    write( @EndText
Syntax: resolve <ip address>
Effect: Provides information on the internet name of an ip address, and vice versa.
See also: wget
EndText
    );
    return 1;
}
