private static mapping Inventory = ([]);

static void eventLoadInventory();

static void eventLoadItem(string file, mixed args, int count);

mapping GetInventory() {
    return Inventory;
}

static void eventLoadItem(string file, mixed args, int count) {
    object ob;

    if( intp(args) ) { 
	if( args < 0 ) { 

	    ob = unique(file, -args);
	    if( ob ) {
		ob->eventMove(this_object());
	    }
	}
	else { 
	    args = args - count;
	    while( args > 0 ) {
		int u = file->GetUnique();
		if( u ) {
		    ob = unique(file, u);
		}
		else {
		    ob = new(file);
		}
		if( ob ) {
		    ob->eventMove(this_object());
		}
		args--;
	    }
	}
    }
}

static void eventLoadInventory() {
    int filtersize, i;
    object array stuff,items,tmp;

    stuff=all_inventory(this_object());
    items = ({});

    for(i=0; i<sizeof(stuff);i++){
	if(tmp = ({ base_name(stuff[i]) }) ) items += tmp;
    }
    filtersize=0;
    foreach(string file, mixed args in Inventory) {
	for(i=0; i<sizeof(items); i++) {
	    if (base_name(items[i]) == file ) filtersize++;
	}
	eventLoadItem(file, args, filtersize);
	filtersize=0;
    }
}


mapping SetInventory(mapping mp) {
    foreach(string key, mixed val in mp) {
	if( key[<2..] == ".c" ) {
	    key = key[0..<3];
	}
	if( key[0] != '/' ) {
	    key = "/" + key;
	}
	Inventory[key] = val;
    }
    eventLoadInventory();
    return Inventory;
}

varargs void reset(int count) {
    eventLoadInventory();
}
