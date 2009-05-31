#include <message_class.h>
#include <dirs.h>
#include <privs.h>

private mixed array PersistentInventory = 0;
private static int  LastSave  = time();
private int PersistentInventoryEnabled = 0;
private static string wut, savename, nom = "";

string GetSaveName(){
    mixed tmp = directory_exists(domain(this_object(),1)+"/save");
    string ret;
    if(tmp){
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

static varargs int SaveObject(mixed str, int i){
    int ret;
    savename = GetSaveName();
    if(!undefinedp(str) && intp(str)) i = str;
    if(str) savename = str;
#if ENABLE_INSTANCES
    savename = new_savename(savename);
#endif
    if(!sizeof(savename) || savename[0..0] == ".") return 0;
    if(PersistentInventoryEnabled){
        PersistentInventory = filter(map(all_inventory(),
                    (: $1->GetSaveString() :)), (: $1 :));
    }
    ret = unguarded( (: save_object, savename, i :) );
    LastSave = time();
    return ret;
}

int SetPersistent(int x){
    if(x > 0) x = 1;
    else x = 0;
    PersistentInventoryEnabled = x;
    return PersistentInventoryEnabled;
}

mixed GetPersistentInventory(){
    return copy(PersistentInventory);
}

static varargs int RestoreObject(mixed str, int i){
    object env;
    int ret;
    if(!undefinedp(str) && intp(str)) i == str;
    if(!interactive() && env && env->GetPersistent()) return;
    if(!str) savename = GetSaveName();
    else savename = str;
    if(!sizeof(savename)) return 0;
#if ENABLE_INSTANCES
    str = new_savename(savename);
    if(file_exists(str)) savename = str;
#else
    str = old_savename(savename);
    if(file_exists(str)) savename = str;
#endif
    ret = unguarded((: restore_object, savename :));
    if(GetPersistent() && sizeof(PersistentInventory)){
        foreach(string obdata in PersistentInventory){
            object ob;
            mixed tmp;
            tmp = restore_variable(obdata);
            if( arrayp(tmp) ){
                int howmany, mclones;
                wut = tmp[0];
                howmany = sizeof(objects((: base_name($1) == wut &&
                                environment($1) :)));
                mclones = wut->GetMaxClones();
                if(wut->GetUnique() && howmany) continue;
                if(mclones && mclones <= howmany ) continue; 
                ob = new(wut);
                if( ob ) ob->eventConvertObject(tmp, 1);
            }
            else {
                int howmany, mclones;
                wut = tmp["#base_name#"];
                howmany = sizeof(objects((: base_name($1) == wut &&
                                environment($1) :)));
                mclones = wut->GetMaxClones();
                if(wut->GetUnique() && howmany) continue;
                if(mclones && mclones <= howmany ) continue;
                if( !catch(ob = new(wut)) ){
                    if( ob ){
                        ob->eventLoadObject(tmp, 1);
                    }
                }
            }
        }
    }
    PersistentInventory = 0;
    return ret;
}

varargs void create(){
    RestoreObject();
}

int eventDestruct(){
    if(PersistentInventoryEnabled){
        SaveObject();
    }
    return 1;
}
