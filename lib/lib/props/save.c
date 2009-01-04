#include <message_class.h>
#include <config.h>
#include <dirs.h>
#include <privs.h>

private mixed array PersistentInventory = 0;
private static int  LastSave  = time();
private int PersistentInventoryEnabled = 0;
private static string savename, nom = "";

string GetSaveName(){
    mixed tmp = directory_exists(domain(this_object(),1)+"/save");
    string ret;

    if(tmp){
        //tmp = query_unique_name(this_object(), ( clonep(this_object()) ) );
        tmp = query_unique_name(this_object());
        ret = domain(this_object(),1)+"/save/"+tmp;
    }
    return ret;
}

int GetPersistent(){
    return PersistentInventoryEnabled;
}

int GetLastSave(){
    return LastSave;
}

void SaveObject(){
    if(!PersistentInventoryEnabled) return;
    savename = GetSaveName();
    //tc("savename: "+savename,"red");
    PersistentInventory = filter(map(all_inventory(),
                (: $1->GetSaveString() :)), (: $1 :));
    unguarded((: save_object, savename :));
    LastSave = time();
    //tc(identify(this_object())+" bing! "+identify(PersistentInventory),"cyan");
    //PersistentInventory = 0;
}

int SetPersistent(int x){
    if(x > 0) x = 1;
    else x = 0;
    PersistentInventoryEnabled = x;
    //tc("PersistentInventoryEnabled: "+PersistentInventoryEnabled);
    return PersistentInventoryEnabled;
}

mixed GetPersistentInventory(){
    return copy(PersistentInventory);
}

void RestoreObject(){
    object env;
    //tc("!","red");
    if(!PersistentInventoryEnabled) return;
    //if(env && env->GetPersistent()) return;
    //tc(identify(this_object())+" bing! "+identify(PersistentInventory),"blue");
    savename = GetSaveName();
    //tc("savename: "+savename,"red");
    unguarded((: restore_object, savename :));
    //tc(identify(this_object())+" bang! "+identify(PersistentInventory),"blue");
    if(GetPersistent() && sizeof(PersistentInventory)){
        //tc("walla","cyan");
        foreach(string obdata in PersistentInventory){
            object ob;
            mixed tmp;
            //tc("walla","yellow");
            tmp = restore_variable(obdata);
            if( arrayp(tmp) ){
                //tc("bing","white");
                ob = new(tmp[0]);
                if( ob ) ob->eventConvertObject(tmp, 1);
            }
            else {
                //tc("bang");
                if( !catch(ob = new(tmp["#base_name#"])) ){
                    //tc("oo");
                    if( ob ){
                        //tc("ee");
                        ob->eventLoadObject(tmp, 1);
                    }
                }
            }
        }
    }
    PersistentInventory = 0;
}

varargs void create(){
    //tc("ting!");
    RestoreObject();
}

int eventDestruct(){
    if(PersistentInventoryEnabled){
        //tc(identify(this_object())+" tang!");
        SaveObject();
    }
    return 1;
}
