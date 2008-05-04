private static mapping Inventory = ([]);
private static mapping InventoryCheck = ([]);
private static string gkey = "";
private static int counter = 0;

static void eventLoadInventory();

static void eventLoadItem(string file, mixed args, int count);

mapping GetInventory(){
    return Inventory;
}

static void eventLoadItem(string file, mixed args, int count){
    object ob;

    if( intp(args) ){ 
        if( args < 0 ){ 
            foreach(object guy in findobs(file)){
                if(environment(guy)) return;
            }
            ob = new(file);
            if( ob ){
                ob->eventMove(this_object());
            }
        }
        else { 
            args = args - count;
            while( args > 0 ){
                int u = file->GetUnique();
                if( u ){
                    foreach(object guy in findobs(file)){
                        if(environment(guy)) return;
                    }
                    ob = new(file);
                }
                else {
                    ob = new(file);
                }
                if( ob ){
                    ob->eventMove(this_object());
                }
                args--;
            }
        }
    }
}

static void eventLoadInventory(){
    int filtersize, i;
    object array stuff,items,tmp;

    stuff=all_inventory(this_object());
    items = ({});

    for(i=0; i<sizeof(stuff);i++){
        if(tmp = ({ base_name(stuff[i]) }) ) items += tmp;
    }
    filtersize=0;
    foreach(string file, mixed args in Inventory){
        for(i=0; i<sizeof(items); i++){
            if (base_name(items[i]) == file ) filtersize++;
        }
        eventLoadItem(file, args, filtersize);
        filtersize=0;
    }
}

mapping SetInventoryCheck(mapping newmap){
    string key = keys(newmap)[0];
    int hb = query_heart_beat();
    int tmpint = 0;
    if(last(key,2) == ".c") key = truncate(key,2);
    if(!hb){
        hb = 1;
        set_heart_beat(1);
    }
    if(hb > 1) tmpint =  (newmap["howlong"] / hb || 1);
    else tmpint =  newmap["howlong"];
    if(!InventoryCheck) InventoryCheck = ([]);
    InventoryCheck[key] = newmap[key];
    return copy(InventoryCheck);
}

mapping SetInventory(mapping mp){
    foreach(string key, mixed val in mp){
        if( key[<2..] == ".c" ){
            key = key[0..<3];
        }
        if( key[0] != '/' ){
            key = "/" + key;
        }
        if(arrayp(val)){
            int howlong = 60;
            int howmany = 1;
            if(val[1] && intp(val[1])) Inventory[key] = val[1];
            else Inventory[key] = 1;
            if(val[0] && intp(val[0])) howlong = val[0];
            else continue;
            if(val[1] && intp(val[1])) howmany = val[1];
            if(!query_heart_beat()) set_heart_beat(1);
            SetInventoryCheck( ([ key : ([ "howlong" : howlong, "howmany" : howmany ]) ]) );
        }
        else Inventory[key] = val;
    }
    eventLoadInventory();
    return Inventory;
}

varargs void reset(){
    counter = 0;
    eventLoadInventory();
}

void heart_beat(){
    int i;
    counter++;
    if(!InventoryCheck || !sizeof(InventoryCheck)) return;
    foreach(mixed key, mixed val in InventoryCheck){
        if(!InventoryCheck[key] || !InventoryCheck[key]["howlong"]) continue;
        if(!(counter % InventoryCheck[key]["howlong"])){
            string *obs;
            gkey = key;
            obs = filter(all_inventory(), (: base_name($1) == gkey :) );
            if(!sizeof(obs)){
                object ob;
                for(i=InventoryCheck[key]["howmany"]; i > 0; i--){
                    ob = new(key);
                    if(ob) ob->eventMove(this_object());
                }
            }
        }
    }
} 
