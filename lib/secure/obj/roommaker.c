/*    /secure/obj/roommaker.c
 *    from the Dead Souls  Object Library
 *    a room building object
 *    created by Descartes of Borg 951202
 */

#include <lib.h>
#include <message_class.h>
#include "include/roommaker.h"

inherit LIB_ROOM;

private int Columns;
private string RoomFile;
private object Author;

static void create() {
    room::create();
    SetNoClean(1);
    SetProperties( ([ "light" : 2 ]) );
}

void eventAbort() {
    if( previous_object() != Author ) return;
    rm(RoomFile);
    Author->eventPrint("Room creation aborted.", MSG_SYSTEM);
    Destruct();
}

varargs mixed eventCreateRoom(string file, string climate) {
    if( file_exists(file) ) {
	Destruct();
	return "You cannot overwrite an existing file.";
    }
    if( !write_file(file, "") ) {
	Destruct();
	return "Permission denied.";
    }
    if( !climate || climate == "" ) SetClimate("temperate");
    SetClimate(climate);
    RoomFile = file;
    Author = this_player();
    Columns = ((int *)Author->GetScreen())[0];
    MainMenu();
    return 1;
}

static void eventPrintTitle(string title) {
    Author->eventPrint(center("Dead Souls Object Library Room Maker", Columns) +
		       "\n", MSG_SYSTEM);
    Author->eventPrint(center(title, Columns) +"\n\n", MSG_SYSTEM);
}

static void MainMenu() {
    string str;
    
    eventPrintTitle("Main Menu");
    str = "1) Climate (" + GetClimate() + ")\n";
    str += "2) Town (" + GetTown() + ")\n";
    str += "3) Properties (" + GetValue(GetProperties()) + ")\n";
    str += "4) Short (" + GetValue(GetShort()) + ")\n";
    str += "5) Long (" + GetValue(GetLong()) + ")\n";
    str += "6) Inventory (" + GetValue(GetInventory()) + ")\n";
    str += "7) Items (" + GetValue(GetId()) + ")\n";
    str += "8) Enters (" + GetValue(GetEnters()) + ")\n";
    str += "9) Exits (" + GetValue(GetExits()) + ")\n";
    str += "10) Obvious Exits (" + GetObviousExits() + ")\n";
    str += "\n";
    str += "s) Save the room\n";
    str += "q) Quit\n";
    str += "x) Save then quit\n\n";
    Author->eventPrint(str, MSG_SYSTEM);
    Author->eventPrint("Choice: ", MSG_PROMPT);
    input_to((: MainMenuCommand :));
}

static void MainMenuCommand(string str) {
    if( !str || str == "" ) {
	Author->eventPrint("Invalid command, try again: ", MSG_PROMPT);
	input_to((: MainMenuCommand :));
	return;
    }
    switch(lower_case(str)) {
	case "1":
	ClimateMenu();
	return;

	case "2":
	TownMenu();
	return;
	
	case "3":
	PropertiesMenu();
	return;

	case "4":
	ShortMenu();
	return;

	case "5":
	LongMenu();
	return;

	case "6":
	InventoryMenu();
	return;

	case "7":
	ItemsMenu();
	return;

	case "8":
	DirsMenu("enter");
	return;

	case "9":
	DirsMenu("exit");
	return;

	case "10":
	ObviousExitsMenu();
	return;

	case "s":
	SaveRoom();
	MainMenu();
	return;

	case "q":
	Author->eventPrint("Exiting the room maker.", MSG_SYSTEM);
	Destruct();
	return;

	case "x":
	SaveRoom();
	Author->eventPrint("Exiting the room maker.", MSG_SYSTEM);
	Destruct();
	return;
	
	default:
	Author->eventPrint("Invalid command, please try again: ", MSG_PROMPT);
	input_to((: MainMenuCommand :));
	return;
    }	
}

static void ClimateMenu() {
    string str;
    
    eventPrintTitle("Climate Painter");
    Author->eventPrint("Current climate: " + GetClimate() +"\n\n", MSG_SYSTEM);
    str = ("1) indoors\n"
	   "2) arctic\n"
	   "3) sub-arctic\n"
	   "4) temperate\n"
	   "5) tropical\n"
	   "6) sub-tropical\n"
	   "7) arid\n\n"
	   "k) Keep current climate\n\n");
    Author->eventPrint(str, MSG_SYSTEM);
    Author->eventPrint("Choice: ", MSG_PROMPT);
    input_to((: ClimateMenuCommand :));
}

static void ClimateMenuCommand(string cmd) {
    if( !cmd ) cmd = "";
    switch(lower_case(cmd)) {
	case "1":
	cmd = "indoors";
	break;

	case "2":
	cmd = "arctic";
	break;

	case "3":
	cmd = "sub-arctic";
	break;
	
	case "4":
	cmd = "temperate";
	break;

	case "5":
	cmd = "tropical";
	break;
	
	case "6":
	cmd = "sub-tropical";
	break;

	case "7":
	cmd = "arid";
	break;

	case "k":
	cmd = 0;
	break;

	default:
	eventPrint("Invalid command, please try again: ", MSG_PROMPT);
	input_to((: ClimateMenuCommand :));
	return;
    }
    if( cmd ) SetClimate(cmd);
    MainMenu();
}

static void TownMenu() {
    eventPrintTitle("Town Painter");
    Author->eventPrint("Current town: " + GetTown() + "\n\n", MSG_SYSTEM);
    Author->eventPrint("New town (<return> to keep current town): ",
		       MSG_PROMPT);
    input_to((: TownMenuCommand :));
}

static void TownMenuCommand(string cmd) {
    if( cmd && cmd != "" ) SetTown(cmd);
    MainMenu();
}

static void PropertiesMenu() {
    string *props;
    mapping mp;
    string str;
    int i;
    
    eventPrintTitle("Property Painter");
    props = keys(mp = GetProperties());
    for(str = "", i=0; i<sizeof(props); i++) 
      str += (i+1) + ") " + props[i] + ": " + identify(mp[props[i]]) + "\n";
    str += "\nn) New property\nr) Return to main menu\n\n";
    Author->eventPrint(str, MSG_SYSTEM);
    Author->eventPrint("Choice: ", MSG_PROMPT);
    input_to((: PropertiesMenuCommand :), props);
}

static void PropertiesMenuCommand(string cmd, string *props) {
    int x;

    if( x = to_int(cmd) ) x--;
    else {
	switch(lower_case(cmd)) {
	    case "n":
	    Author->eventPrint("Property name: ", MSG_PROMPT);
	    input_to(function(string str) {
		         if( !str || str == "" ) {
			     PropertiesMenu();
			     return;
			 }
		         Author->eventPrint("Value for " + str + ": ",
					    MSG_PROMPT);
			 input_to(function(string str, string prop) {
			              int x;
				      
			              if( !str || str == "" ) {
					  PropertiesMenu();
					  return;
				      }
				      if( (x = to_int(str)) || str == "0" )
					 SetProperty(prop, x);
				      else SetProperty(prop, str);
				      PropertiesMenu();
				  }, str);
		     });
	    return;

	    case "r":
	    MainMenu();
	    return;

	    default:
	    Author->eventPrint("Invalid command, please try again: ",
			       MSG_PROMPT);
	    input_to((: PropertiesMenuCommand :), props);
	    return;
	}
    }
    if( x >= sizeof(props) || x < 0 ) {
	Author->eventPrint("Invalid command, please try again: ", MSG_PROMPT);
	input_to((: PropertiesMenuCommand :), props);
	return;
    }
    Author->eventPrint("New value for " + props[x] + " (<return> to delete): ",
		       MSG_PROMPT);
    input_to(function(string str, string prop) {
	         if( !str || str == "" ) RemoveProperty(prop);
		 else {
		     int x;

		     if( (x = to_int(str)) || str == "0" )
		       SetProperty(prop, x);
		     else SetProperty(prop, str);
		 }
		 PropertiesMenu();
		 return;
	     }, props[x]);
}
    
static void ShortMenu() {
    eventPrintTitle("Short Description Painter");
    Author->eventPrint("Current short: " + GetShort() + "\n\n", MSG_SYSTEM);
    Author->eventPrint("New short (<return> to keep current short): ",
		       MSG_PROMPT);
    input_to((: ShortMenuCommand :));
}

static void ShortMenuCommand(string cmd) {
    if( cmd && cmd != "" ) SetShort(cmd);
    MainMenu();
}

static void LongMenu() {
    string str;
    
    eventPrintTitle("Long Description Painter");
    Author->eventPrint(GetLong() + "\n", MSG_SYSTEM);
    str = ("e) Edit current long\n"
	   "k) Keep current long\n"
	   "n) New long\n\n");
    Author->eventPrint(str, MSG_SYSTEM);
    Author->eventPrint("Choice: ", MSG_PROMPT);
    input_to((: LongMenuCommand :));
}

static void LongMenuCommand(string cmd) {
    string str;
    
    if( !cmd ) cmd = "";
    switch(cmd) {
	case "e":
	str = GetLong();
	break;

	case "k":
	MainMenu();
	return;

	case "n":
	str = "";
	break;

	default:
	Author->eventPrint("Invalid command, please try again: ", MSG_PROMPT);
	input_to((: LongMenuCommand :));
	return;
    }
    if( file_exists( DIR_TMP + "/" + (string)Author->GetKeyName()) )
      rm(DIR_TMP + "/" + (string)Author->GetKeyName());
    write_file(DIR_TMP + "/" + (string)Author->GetKeyName(), str);
    Author->eventEdit(DIR_TMP + "/" + (string)Author->GetKeyName(),
		      function() {
			  string str;

			  if( !(str = read_file(DIR_TMP + "/" +
						(string)Author->GetKeyName())))
			    MainMenu();
			  else SetLong(replace_string(str, "\n", "  "));
			  MainMenu();
		      });
}

static void InventoryMenu() {
    string *items;
    mapping mp;
    string str;
    int i;
    
    eventPrintTitle("Inventory Painter");
    items = keys(mp = GetInventory());
    for(str = "", i=0; i<sizeof(items); i++) 
      str += (i+1) + ") " + items[i] + ":  " + mp[items[i]] + "\n";
    str += "\nn) New inventory item\nr) Return to main menu\n\n";
    Author->eventPrint(str, MSG_SYSTEM);
    Author->eventPrint("Choice: ", MSG_PROMPT);
    input_to((: InventoryMenuCommand :), items);
}

static void InventoryMenuCommand(string cmd, string *items) {
    int x;

    if( x = to_int(cmd) ) x--;
    else {
	switch(lower_case(cmd)) {
	    case "n":
	    Author->eventPrint("Inventory name: ", MSG_PROMPT);
	    input_to(function(string str) {
		         if( !str || str == "" ) {
			     InventoryMenu();
			     return;
			 }
		         Author->eventPrint("Value for " + str + ": ",
					    MSG_PROMPT);
			 input_to(function(string str, string prop) {
			              mapping mp;
			              int x;
				      
			              if( !str || str=="" || !(x=to_int(str))){
					  InventoryMenu();
					  return;
				      }
				      mp = GetInventory();
				      mp[prop] = x;
				      SetInventory(mp);
				      InventoryMenu();
				  }, str);
		     });
	    return;

	    case "r":
	    MainMenu();
	    return;

	    default:
	    Author->eventPrint("Invalid command, please try again: ",
			       MSG_PROMPT);
	    input_to((: InventoryMenuCommand :), items);
	    return;
	}
    }
    if( x >= sizeof(items) || x < 0 ) {
	Author->eventPrint("Invalid command, please try again: ", MSG_PROMPT);
	input_to((: InventoryMenuCommand :), items);
	return;
    }
    Author->eventPrint("New value for " + items[x] + " (<return> to delete): ",
		       MSG_PROMPT);
    input_to(function(string str, string item) {
	         mapping mp;
		 
	         if( !str || str == "" ) {
		     mp = GetInventory();
		     map_delete(mp, item);
		     SetInventory(mp);
		 }
		 else {
		     mp = GetInventory();
		     mp[item] = to_int(str);
		     SetInventory(mp);
		 }
		 InventoryMenu();
		 return;
	     }, items[x]);
}

static void ItemsMenu() {
    string *items;
    string str = "";
    int i;
    
    eventPrintTitle("Items Painter");
    for(i=0, items = GetId(); i<sizeof(items); i++) 
      str += (i+1) + ") " + items[i] + " (" +
	GetValue(GetItemDescription(items[i])) + ")\n";
    str += "\nn) New items\n";
    str += "r) Return to main menu\n";
    Author->eventPrint(str, MSG_SYSTEM);
    Author->eventPrint("Choice: ", MSG_PROMPT);
    input_to((: ItemsMenuCommand :), items);
}

static void ItemsMenuCommand(string cmd, string *items) {
    int x;
    
    if( !cmd ) cmd = "";
    if( !(x = to_int(cmd)) ) {
	switch(lower_case(cmd)) {
	    case "n":
	    Author->eventPrint("List of new items, comma separated: ",
			       MSG_PROMPT);
	    input_to(function(string str) {
		         string *items;
		
			 items = explode(str, ",");
			 if( !sizeof(items) ) {
			     ItemsMenu();
			     return;
			 }
			 Author->eventPrint("Description:", MSG_SYSTEM);
			 if( file_exists(DIR_TMP + "/" +
					 (string)Author->GetKeyName()) )
			   rm(DIR_TMP + "/" + (string)Author->GetKeyName());
			 Author->eventEdit(DIR_TMP + "/" +
					   (string)Author->GetKeyName(),
					   (: CompleteItem, items :));
		     });
	    return;

	    case "r":
	    MainMenu();
	    return;
	    
	    default:
	    Author->eventPrint("Invalid command, please try again: ",
			       MSG_PROMPT);
	    input_to((: ItemsMenuCommand :), items);
	    return;
	}
    }
    else if( x >= sizeof(items) || x < 0 ) {
	Author->eventPrint("Invalid command, please try again: ", MSG_PROMPT);
	input_to((: ItemsMenuCommand :), items);
	return;
    }
    Author->eventPrint("New value for " + items[x]+" (empty desc to delete): ",
		       MSG_SYSTEM);
    if( file_exists(DIR_TMP + "/" + (string)Author->GetKeyName()) )
      rm(DIR_TMP + "/" + (string)Author->GetKeyName());
    write_file(DIR_TMP + "/" + (string)Author->GetKeyName(),
	       GetItemDescription(items[x]));
    Author->eventEdit(DIR_TMP + "/" + (string)Author->GetKeyName(),
		      (: CompleteItem, ({ items[x] }) :));
}

void CompleteItem(string *items) {
    string str, item;

    if( previous_object() != Author ) return;
    str = read_file(DIR_TMP + "/" + (string)Author->GetKeyName());
    rm(DIR_TMP + "/" + (string)Author->GetKeyName());
    if( !str || trim(str) == "" ) foreach(item in items) RemoveItem(item);
    else foreach(item in items) AddItem(item, replace_string(str, "\n", "  "));
    ItemsMenu();
}

static void DirsMenu(string type) {
    string *sorties;
    function f;
    string str;
    int i;
    
    if( type == "enter" ) {
	eventPrintTitle("Enters Painter");
	sorties = GetEnters();
	f = (: GetEnter :);
    }
    else {
	eventPrintTitle("Exits Painter");
	sorties = GetExits();
	f = (: GetExit :);
    }
    for(str = "", i=0; i<sizeof(sorties); i++) 
      str += (i+1) + ") " + sorties[i] + ":  " + evaluate(f, sorties[i]) +"\n";
    str += "\nn) New " + type + "\nr) Return to main menu\n\n";
    Author->eventPrint(str, MSG_SYSTEM);
    Author->eventPrint("Choice: ", MSG_PROMPT);
    input_to((: DirsMenuCommand :), type, sorties);
}

static void DirsMenuCommand(string cmd, string type, string *sorties) {
    int x;

    if( x = to_int(cmd) ) x--;
    else {
	switch(lower_case(cmd)) {
	    case "n":
	    Author->eventPrint(capitalize(type) + " name: ", MSG_PROMPT);
	    input_to(function(string str, string type) {
		         if( !str || str == "" ) {
			     DirsMenu(type);
			     return;
			 }
		         Author->eventPrint("Destination for " + str + ": ",
					    MSG_PROMPT);
			 input_to(function(string str,string type,string dir){
			              int x;
				      
			              if( !str || str == ""  ) {
					  DirsMenu(type);
					  return;
				      }
				      if( type == "enter" ) AddEnter(dir, str);
				      else AddExit(dir, str);
				      DirsMenu(type);
				  }, type, str);
		     }, type);
	    return;

	    case "r":
	    MainMenu();
	    return;

	    default:
	    Author->eventPrint("Invalid command, please try again: ",
			       MSG_PROMPT);
	    input_to((: InventoryMenuCommand :), sorties);
	    return;
	}
    }
    if( x >= sizeof(sorties) || x < 0 ) {
	Author->eventPrint("Invalid command, please try again: ", MSG_PROMPT);
	input_to((: DirsMenuCommand :), type, sorties);
	return;
    }
    Author->eventPrint("New value for " + sorties[x]+" (<return> to delete): ",
		       MSG_PROMPT);
    input_to(function(string str, string type, string dir) {
	         if( !str || str == "" ) {
		     if( type == "enter" ) RemoveEnter(dir);
		     else RemoveExit(dir);
		 }
		 else {
		     if( type == "enter" ) AddEnter(dir, str);
		     else AddExit(dir, str);
		 }
		 DirsMenu(type);
		 return;
	     }, type, sorties[x]);
}

static void ObviousExitsMenu() {
    eventPrintTitle("Obvious Exits Painter");
    Author->eventPrint("Current obvious exits: " + GetObviousExits() + "\n\n",
		       MSG_SYSTEM);
    Author->eventPrint("New obvious exits (<return> to keep current exits): ",
		       MSG_PROMPT);
    input_to((: ObviousExitsMenuCommand :));
}

static void ObviousExitsMenuCommand(string cmd) {
    if( cmd && cmd != "" ) SetObviousExits(cmd);
    MainMenu();
}

static void LoadInventory() { }

static void SaveRoom() {
    string str;

    str = "/*    " + RoomFile + "\n";
    str += " *    created by " + (string)Author->GetCapName() + "@" +
      mud_name() + "\n";
    str += " *    Date: " + ctime(time()) + "\n */\n\n";
    str += "#include <lib.h>\n\n";
    str += "inherit LIB_ROOM;\n\n";
    str += "static void create() {\n    room::create();\n";
    str += "    SetTown(" + save_variable(GetTown()) + ");\n";
    str += "    SetClimate(" +save_variable(GetClimate()) + ");\n";
    str += "    SetProperties(" + save_variable(GetProperties()) + ");\n";
    str += "    SetShort(" + save_variable(GetShort()) + ");\n";
    str += "    SetLong(\"" + GetLong() + "\");\n";
    str += "    SetInventory(" + save_variable(GetInventory()) + ");\n";
    str += "    SetItems( ([ ";
    foreach(string item in GetId()) 
      str += "\"" + item + "\" : \"" + GetItemDescription(item) + "\",\n";
    str += "    ]));\n";
    foreach(string dir in GetEnters())
      str += "    AddEnter(\"" + dir + "\", \"" + GetEnter(dir) + "\");\n";
    foreach(string dir2 in GetExits())
      str += "    AddExit(\"" + dir2 + "\", \"" + GetExit(dir2) + "\");\n";    
    str += "    SetObviousExits(" + save_variable(GetObviousExits()) + ");\n";
    str += "}\n";
    write_file(RoomFile, str);
}
						
varargs static string GetValue(mixed val, string str) {
    str = save_variable(val);
    if( strlen(str) >= (Columns - 25) ) str = str[0..(Columns-26)] + "...";
    /*
    if( !str ) str = "";
    if( stringp(val) || intp(val) ) {
	str += val;

	return replace_string(str, "\n", "\\n");
    }
    if( arrayp(val) ) {
	str += "({";
	foreach(mixed thing in val) str = GetValue(thing, str) + ",";
	str += "})";
	if( strlen(str) >= (Columns - 25) ) str = str[0..(Columns-26)];
	return str;
    }
    if( mapp(val) ) {
	str += "([";
	foreach(string key, mixed v in val) {
	    str += key + " : ";
	    str = GetValue(v, str);
	    str += ",";
	}
	str += "])";
	if( strlen(str) >= (Columns - 25) ) str = str[0..(Columns-26)];
	return str;
    } */
    return str;
}
