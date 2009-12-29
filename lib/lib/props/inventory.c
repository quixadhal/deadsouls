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
    int u;

    if(functionp(args)) args = evaluate(args);

    if( intp(args) ){ 
        if(args < 0){ 
            args = 1;
            u = 1;
        }
        args = args - count;
        while( args > 0 ){
            if(!u) u = file->GetUnique();
            if(!u) u = file->GetMaxClones();
            if( u ){
                object *clones = filter(findobs(file),(: clonep($1) &&
                            environment($1) :));
                if(sizeof(clones) >= u) return;
            }
            ob = new(file);
            if( ob ){
                ob->eventMove(this_object());
            }
            args--;
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
            mixed howmany = 1;
            if(!sizeof(val)) val = ({ 60, 1 });
            if(sizeof(val) < 2) val += ({ 1 });
            howmany = ( val[1] || 1 );
            Inventory[key] = howmany;
            if(intp(val[0])) howlong = (val[0] || 1);
            else continue;
            if(!query_heart_beat()) set_heart_beat(1);
            SetInventoryCheck( ([ key : ([ "howlong" : howlong, "howmany" : howmany ]) ]) );
        }
        else Inventory[key] = val;
    }
    eventLoadInventory();
    return Inventory;
}

varargs void reset(){
    if(origin() == "driver") return;
    counter = 0;
    eventLoadInventory();
}

void heart_beat(){
    int i,howmany;
    counter++;
    if(!InventoryCheck || !sizeof(InventoryCheck)) return;
    foreach(mixed key, mixed val in InventoryCheck){
        if(!InventoryCheck[key] || !InventoryCheck[key]["howlong"]) continue;
        if(!(counter % InventoryCheck[key]["howlong"])){
            string *obs;
            gkey = key;
            obs = filter(all_inventory(), (: base_name($1) == gkey :) );

            if(intp(InventoryCheck[key]["howmany"])){
                howmany = InventoryCheck[key]["howmany"];
            }
            else {
                howmany = evaluate( InventoryCheck[key]["howmany"] );
            }
            if(sizeof(obs) < howmany){
                int count = howmany - sizeof(obs);
                eventLoadItem(key, count, 0);
            }
        }
    }
} 
