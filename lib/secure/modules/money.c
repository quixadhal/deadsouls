#include <lib.h>
#include <dirs.h>
#include <daemons.h>
#include <modules.h>
#include <commands.h>
#include ROOMS_H


string globalstr, globalstr2, globalstr3, globaltmp;
mapping TmpMap = ([]);
mapping LoadMap = ([]);

int eventModCost(object ob, string type, mixed val);

int eventModMoney(object ob, string type, mixed val){
    string junk1;
    int amount, npc;
    mapping NewMap;

    TmpMap = ([]);
    NewMap = ([]);
    LoadMap = ([]);
    globalstr = "";
    globalstr2 = "";
    globalstr3 = "";


    if(inherits(LIB_NPC,ob)) npc = 1;

    if(!intp(val) ) sscanf(val,"%s %d",junk1,amount);
    if(amount) val = amount;
    else val = 0;
    amount = 0;

    globalstr3 = base_name(ob)+".c";
    globalstr = generate_tmp(ob);
    unguarded( (: cp(globalstr3, globalstr) :) );
    if(!check_privs(this_player(),globalstr3)){
        write("You do not appear to have write access to this file. Modification aborted.");
        return 1;
    }
    if(npc){
        unguarded( (: TmpMap = this_object()->eventMappifyLine(globalstr,"SetCurrency") :) );
        LoadMap = this_object()->eventParsePair(globalstr,"AddCurrency","string","literal");
        foreach(string foo, mixed bar in LoadMap) if(stringp(bar)) LoadMap[foo] = amount;
        unguarded( (: globaltmp = remove_matching_line(globalstr,"AddCurrency",1) :) );
        unguarded( (: globaltmp = remove_matching_line(globaltmp,"SetCurrency",1) :) );
        globalstr2 = "SetCurrency( ";
        NewMap = add_maps(LoadMap,TmpMap);
    }

    else {
        unguarded( (: TmpMap = this_object()->eventMappifyLine(globalstr,"SetMoney") :) );
        LoadMap = this_object()->eventParsePair(globalstr,"AddMoney","string","literal");
        foreach(string foo, mixed bar in LoadMap) if(stringp(bar)) LoadMap[foo] = amount;
        unguarded( (: globaltmp = remove_matching_line(globalstr,"AddMoney",1) :) );
        unguarded( (: globaltmp = remove_matching_line(globaltmp,"SetMoney",1) :) );
        globalstr2 = "SetMoney( ";
        NewMap = add_maps(copy(LoadMap),copy(TmpMap));
    }

    if(stringp(val)) val = amount;
    NewMap[type] = val;
    globalstr2 += this_object()->eventStringifyMap(NewMap) + " );";
    globaltmp = this_object()->eventAppend(read_file(globaltmp),({"SetItems","SetInventory","SetLong"}),"\n"+globalstr2+"\n");
    unguarded( (: write_file(globalstr,globaltmp,1) :) );
    this_object()->eventGeneralStuff(globalstr);
    unguarded( (: cp(globalstr,globalstr3) :) );
    unguarded( (: rm(globalstr) :) );
    reload(ob);
    return 1;
}

int eventModCost(object ob, string type, mixed val){
    string new_line, junk;
    int amount;

    if(stringp(val)) sscanf(val,"%s %d",junk,amount);
    else amount = val;
    globalstr = base_name(ob)+".c";
    if(!check_privs(this_player(),globalstr)){
        write("You do not appear to have write access to this file. Modification aborted.");
        return 1;
    }

    unguarded( (: globalstr2 = read_file(globalstr) :) );

    globalstr2 = remove_matching_line(globalstr2,"SetBaseCost",1);
    globalstr2 = remove_matching_line(globalstr2,"SetDollarCost",1);
    globalstr2 = remove_matching_line(globalstr2,"SetCost",1);
    globalstr2 = remove_matching_line(globalstr2,"SetValue",1);

    if(!type) type = "silver";

    new_line = "SetBaseCost(\""+type+"\", "+amount+");";
    globalstr2 = this_object()->eventAppend(globalstr2,({"SetMass","SetItems","SetLong","SetInventory","SetVendorType"}), "\n"+new_line+"\n");

    unguarded( (: write_file(globalstr, globalstr2, 1) :) );

    this_object()->eventGeneralStuff(globalstr);
    reload(ob);


    return 1;
}
