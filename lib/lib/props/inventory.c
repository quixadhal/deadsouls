/*    /lib/props/inventory.c
 *    From the Dead Souls V Object Library
 *    Inheritable for objects that manage inventories
 *    Created by Descartes of Borg 961222
 *    Version: @(#) inventory.c 1.2@(#)
 *    Last modified: 96/12/31
 */

private static mapping Inventory = ([]);

static void eventLoadInventory();
static void eventLoadItem(string file, mixed args, int count);

mapping GetInventory() {
    return Inventory;
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

static void eventLoadInventory() {
    object array items = map(all_inventory(this_object()), (: base_name :));
    
    foreach(string file, mixed args in Inventory) {
	eventLoadItem(file, args, sizeof(filter(items, (: $1 == $(file) :))));
    }
}

static void eventLoadItem(string file, mixed args, int count) {
    if( intp(args) ) { // Number of items to clone or unique
	if( args < 0 ) { // Unique item
	    object ob;

	    ob = unique(file, -args);
	    if( ob ) {
		ob->eventMove(this_object());
	    }
	}
	else { // Clone up to #
	    args = args - count;
	    while( args > 0 ) {
		int u = file->GetUnique();
		object ob;
		
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

varargs void reset(int count) {
    eventLoadInventory();
}
