/* Roommaker by Haderach @ Frontiers
* Sep 21 2005
*/

#include <lib.h>
#include <vendor_types.h>
inherit LIB_ITEM;

int Begin(); // The function that gets called when the object is activated

//===================
//= Reset Functions =
//===================
void reset_room_header ();
void reset_room_properties();
void reset_room_attributes();
void reset_temps ();

//===========================
//= Menu Printing Functions =
//===========================

int print_menu_main ();
int print_menu_room_create();
int print_menu_item_create();
int print_menu_room_climate();
int print_menu_room_properties();
int print_menu_room_exits();
int print_menu_room_enters();
int print_menu_room_inventory();
int print_menu_room_item();
int print_menu_room_smell();
int print_menu_room_listen();

//=============================
//= Menu Processing Functions =
//=============================

int process_default (string str);
int process_menu_main (string str);
int process_room_create(string str);
int process_room_climate(string str);
int process_room_properties(string str);
int process_room_exits(string str);
int process_room_enters(string str);
int process_room_item(string str);
int process_room_inventory(string str);
int process_room_smell(string str);
int process_room_listen(string str);

//=============================
//= Room Setter Functions     =
//=============================
int set_town(string str);
int set_short(string str);
int set_obvious(string str);
int set_light(string str);
int set_long(string str);
int set_room_exit(string str);
int set_room_exit_helper(string str);
int set_room_enter(string str);
int set_room_enter_helper(string str);
int set_room_inventory(string str);
int set_room_inventory_helper(string str);
int set_room_item_single(string str);
int set_room_item_multiple(string str);
int set_room_item_description(string str);
int set_room_smell_single(string str);
int set_room_smell_multiple(string str);
int set_room_smell_description(string str);
int set_room_listen_single(string str);
int set_room_listen_multiple(string str);
int set_room_listen_description(string str);
int set_defaultdir_helper(string str);
int set_defaultdir(string str);
int set_defaultdir_main(string str);
// toggle the room properties
int toggle_attack();
int toggle_bump();
int toggle_steal();
int toggle_magic();
int toggle_paralyze();
int toggle_teleport();

//==============================
//= Display Printing Functions =
//==============================
string print_room_list_for_display (mapping room_map);
string print_properties_for_display ();
string print_room_items_for_display ();
string print_room_inventory_for_display ();
string print_room_exits_for_display ();
string print_room_enters_for_display ();
string print_room_smells_for_display ();
string print_room_listens_for_display ();

//==============================
//= File Printing Functions    =
//==============================
string print_room_for_file_helper (mapping room_map);
string print_properties_for_file ();
string print_room_items_for_file ();
string print_room_inventory_for_file ();
string print_room_smells_for_file ();
string print_room_listens_for_file ();
string print_room_exits_for_file ();
string print_room_enters_for_file ();


//=============================
//= Room Load/Save Functions  =
//=============================
int write_to_file(string str);
//int read_room(string str);

//====================================================
//= Initialize variables to the beginning values     =
//====================================================

mapping climates = (([ "1":"indoors", "2":"temperate", "3":"arid",
    "4":"arctic", "5":"tropical", "6":"sub-tropical" ]));
mapping room_properties_map = (([ "no attack":"0",
    "no bump":"0",
    "no steal":"0",
    "no magic":"0",
    "no paralyze":"0",
    "no teleport":"0"
  ]));

mapping room_attributes_map = (([ "Town":"",
    "Climate":"",
    "Short Desc":"",
    "Long Desc (Day)":"",
    "Long Desc (Night)":"",
    "Obvious Exits":"",
    "Light Level (Day)":"30",
    "Light Level (Night)":"15" ]));

mapping room_items_map = ([]);
mapping room_inventory_map = ([]);
mapping room_exits_map = ([]);
mapping room_enters_map = ([]);
mapping room_smells_map = ([]);
mapping room_listens_map = ([]);

string *multi_item_array = ({});
string objectdescription = "";

string tempdir = "";
string prefix = "";
string defaultdir = "";

string username = ""; // the administrator's lowercase name
string capitalname = ""; // the administrator's proper name

string header = "";
string tail = "";
string extrafuns = "";

string globalstr = "";
string globalstr2 = "";

int check_prop(mixed arg){
    if(!arg || sizeof(arg) < 1) return 0;
    else return 1;
}


//====================================================
//= Main function. Creates the actual item           =
//====================================================


static void create() {
    item::create();
    SetKeyName("Object Maker");
    SetId( ({"om", "room maker", "roommaker", "maker", "tool"}) );
    SetShort("a room maker");
    SetLong("This is a great tool for any creator.\n"
      "Type \"rmake\" to start the roommaker. Be aware that it keeps the "
      "variables that you type in within its memory. If you want to create "
      "a completely new room, simply clear the buffers. Otherwise, this "
      "roommaker can save you a lot of time if you're creating virtually "
      "similar rooms. You can simply keep writing to different file names. "
      "Hope you enjoy, and mail me with comments, questions, or bug reports.\n"
      "Documentation has been written. Just ask Haderach for it.\n");
    SetVendorType(VT_TREASURE);
}


//====================================================
//= Creates the action that starts the roommaker     =
//====================================================


void init() {
    ::init();
    add_action("Begin","rmake");
}

int Begin () {
    // Keep people who are not wizards from using this object
    if (!creatorp(environment(this_object()))) {
	write ("Your mortal mind cannot comprehend how to work this mighty object.");
	return 1;
    }
    if (strlen (username) == 0) {
	username = lower_case(environment(this_object())->GetName());
    }
    if (strlen (capitalname) == 0) {
	capitalname = environment(this_object())->GetCapName();
    }
    // Set everything to its default value.
    if (strlen (defaultdir) == 0) {
	defaultdir = "/realms/" + username + "/area/room/";
    }

    // Print the starting menu
    this_object()->print_menu_main();
    return 1;
}


//===================
//= Reset Functions =
//===================

void reset_room_header ()
{
    header = "/* \nFile created with Roommaker \n"
    "written by Haderach@Frontiers \n"
    "Room made by " + capitalname + "\n"
    "Date: "+ ctime(time()) + "\n"
    "WARNING! WARNING! If you edit this room by hand "
    "the room maker may be unable to read\n"
    "it properly. Hand-edit at your own risk.\n"
    "\n*/\n\n"
    "#include <lib.h>\n"
    "#include \"../../customdefs.h\";\n"
    "inherit LIB_ROOM;\n"
    "\n"
    "static void create() {\n"
    "  room::create();\n";
}

void reset_room_properties() {
    string *propkey = keys (room_properties_map);
    int index = 0;
    while (index < sizeof (propkey)) {
	room_properties_map[propkey[index]] = "0";
	index++;
    }
}

void reset_room_attributes () {
    string *propkey = keys (room_attributes_map);
    int index = 0;
    while (index < sizeof (propkey)) {
	room_attributes_map[propkey[index]] = "";
	index++;
    }
    room_attributes_map["Light Level (Day)"] = "30";
    room_attributes_map["Light Level (Night)"] = "15";
}

void reset_everything_in_room ()
{
    reset_room_header ();
    reset_room_properties ();
    reset_room_attributes ();

    room_items_map = ([]);
    room_inventory_map = ([]);
    room_exits_map = ([]);
    room_enters_map = ([]);
    room_smells_map = ([]);
    room_listens_map = ([]);

    reset_temps ();

    tail = "";
    extrafuns = "";
}

void reset_temps ()
{
    multi_item_array = ({});
    tempdir = "";
    prefix = "";
    objectdescription = "";
}

//==============================
//= Printing/Display Functions =
//==============================

// Utility function that other functions call
string print_room_list_for_display (mapping room_map) {
    string display = "";
    string *roomlist = keys (room_map);
    string scounter = "";
    for (int counter = 0; counter < sizeof (roomlist); counter++) {
	scounter = (string) counter + 1;
	display += "     " + scounter + ". " + roomlist[counter] + " : " + room_map[roomlist[counter]];
	if (counter + 1 < sizeof (roomlist)) {
	    display += "\n";
	}
    }
    return display;
}

string print_properties_for_display () {
    string properties = 
    "       \"No Attack\":" + room_properties_map["no attack"] + ", \n"
    "       \"No Bump\":" + room_properties_map["no bump"] + ", \n"
    "       \"No Steal\":" + room_properties_map["no steal"] + ", \n"
    "       \"No Magic\":" + room_properties_map["no magic"] + ", \n"
    "       \"No Paralyze\":" + room_properties_map["no paralyze"] + ", \n"
    "       \"No Teleport\":" + room_properties_map["no teleport"] + "\n";
    return properties;
}

string print_room_items_for_display () {
    return print_room_list_for_display (room_items_map);
}

string print_room_inventory_for_display () {
    return print_room_list_for_display (room_inventory_map);
}

string print_room_exits_for_display () {
    return print_room_list_for_display (room_exits_map);
}

string print_room_enters_for_display () {
    return print_room_list_for_display (room_enters_map);
}

string print_room_smells_for_display () {
    return print_room_list_for_display (room_smells_map);
}

string print_room_listens_for_display () {
    return print_room_list_for_display (room_listens_map);
}

string print_properties_for_file () {
    string properties = "  SetProperties (([\n";
    properties += 
    "       \"no attack\":" + room_properties_map["no attack"] + ", \n"
    "       \"no bump\":" + room_properties_map["no bump"] + ", \n"
    "       \"no steal\":" + room_properties_map["no steal"] + ", \n"
    "       \"no magic\":" + room_properties_map["no magic"] + ", \n"
    "       \"no paralyze\":" + room_properties_map["no paralyze"] + ", \n"
    "       \"no teleport\":" + room_properties_map["no teleport"];
    properties += "]));\n";
    return properties;
}

string print_room_for_file_helper (mapping room_map) {
    string strresult = "";
    string *roomlist = keys (room_map);
    for (int counter = 0; counter < sizeof (roomlist); counter++) {
	strresult += "  " + roomlist[counter] + " : " + room_map[roomlist[counter]];
	if (counter != (sizeof (roomlist) - 1)) {
	    strresult += " ,\n";
	}
    }
    strresult += "]));\n";
    return strresult;
}

string print_room_items_for_file () {
    string strresult = "  SetItems(([";
    strresult += print_room_for_file_helper (room_items_map);
    return strresult;
}

string print_room_inventory_for_file () {
    string strresult = "  SetInventory(([";
    strresult += print_room_for_file_helper (room_inventory_map);
    return strresult;
}

string print_room_smells_for_file () {
    string strresult = "  SetSmell(([";
    strresult += print_room_for_file_helper (room_smells_map);
    return strresult;
}

string print_room_listens_for_file () {
    string strresult = "  SetListen(([";
    strresult += print_room_for_file_helper (room_listens_map);
    return strresult;
}

string print_room_exits_for_file () {
    string strresult = "";
    string *roomlist = keys (room_exits_map);
    for (int counter = 0; counter < sizeof (roomlist); counter++) {
	strresult += "  AddExit(" + roomlist[counter] + ", " + room_exits_map[roomlist[counter]] + ");\n";
    }
    return strresult;
}

string print_room_enters_for_file () {
    string strresult = "";
    string *roomlist = keys (room_enters_map);
    for (int counter = 0; counter < sizeof (roomlist); counter++) {
	strresult += "  AddEnter(" + roomlist[counter] + ", " + room_enters_map[roomlist[counter]] + ");\n";
    }
    return strresult;
}


//====================================================
//= Prints the Main menu                             =
//====================================================

int print_menu_main() {
    write("   MAIN MENU\n"
      "================\n\n"
      "1. Create Room\n"
      "2. Load Room\n"
      "3. Set Default Directory [ " + defaultdir + " ]\n"
      "q. Quit\n\n"
      "Enter Your Choice: ");
    input_to("process_menu_main");
    return 1;
}


//====================================================
//= Processes the input of the main menu selection   =
//====================================================

int process_default (string str)
{
    // This takes care of the default commands that every menu
    // should support.
    switch (str) {
    case "q":
    case "quit":
    case "exit":
	return 1;
    default:
	return 0;
    }
    return 1;
}

int process_menu_main (string str) {
    switch(str) {
    case "1": // Create Room
	this_object()->print_menu_room_create();
	return 1;
    case "2": // Load Room
	write("Enter file name within " + defaultdir + " : ");
	this_object()->reset_everything_in_room ();
	input_to("read_room");
	return 1;
    case "3": // Set Default Directory
	write("Enter default directory: ( " + defaultdir + 
	  " ) type c to cancel [default: " + 
	  (string)this_player()->query_cwd() + " ]");
	input_to("set_defaultdir_main");
	return 1;    
    default:
	if (process_default (str) == 0) {
	    write("Invalid choice. Try again.");
	    input_to("process_menu_main");
	}
	return 1;
    }
    return 1;
}


//====================================================
//= Prints the Create Room menu                      =
//====================================================

int print_menu_room_create () {
    write("   ROOM EDITOR\n"
      "================\n\n"
      "1. Town [ " + room_attributes_map["Town"] + " ]\n"
      "2. Climate [ " + room_attributes_map["Climate"] + " ]\n"
      "3. Properties \n" + print_properties_for_display () +
      "4. Short Desc[ " + room_attributes_map["Short Desc"] + " ]\n"
      "5. Long Desc (Day) [ " + room_attributes_map["Long Desc (Day)"] + " ]\n"
      "6. Long Desc (Night) [ " + room_attributes_map["Long Desc (Night)"] + " ]\n"
      "7. Items [ " + print_room_items_for_display () + " ]\n"
      "8. Inventory [ " + print_room_inventory_for_display () + " ]\n"
      "9. Smell [ " + print_room_smells_for_display () + " ]\n"
      "10. Listen [ " + print_room_listens_for_display () + " ]\n"
      "11. Obvious Exits [ " + room_attributes_map["Obvious Exits"] + " ]\n"
      "12. Exits [ " + print_room_exits_for_display () + " ]\n"
      "13. Light Level (Day) [ " + room_attributes_map["Light Level (Day)"] + " ]\n"
      "14. Light Level (Night) [ " + room_attributes_map["Light Level (Night)"] + " ]\n"
      "15. Enters [ " + print_room_enters_for_display () + " ]\n"
      "w. Write to File\n"
      "c. Clear Buffers\n"
      "d. Default Directory [" + defaultdir + "]\n"
      "p. Previous Menu\n\n"
      "Enter Your Choice: ");
    input_to("process_room_create");
    return 1;
}

//====================================================
//= Processes the input from the Create Room menu    =
//====================================================

int process_room_create(string str) {
    switch(str) {
    case "1":
	write("Enter name of Town: ");
	input_to("set_town");
	return 1;
    case "2":
	this_object()->print_menu_room_climate();
	return 1;
    case "3":
	this_object()->print_menu_room_properties();
	return 1;
    case "4":
	write("Enter Short Description: ");
	input_to("set_short");
	return 1;
    case "5":
	write("Enter Long Description (Day). End with q on a single line\n"
	  "type load <filename> to load file as text.");
	room_attributes_map["Long Desc (Day)"] = "";
	input_to("set_long");
	return 1;
    case "6":
	write("Enter Long Description (Night). End with q on a single line.\n"
	  "Type s to have same as Long Desc (Day).\n"
	  "type load <filename> to load file as text.");
	room_attributes_map["Long Desc (Night)"] = "";
	input_to("set_nightlong");
	return 1;
    case "7":
	this_object()->print_menu_room_item();
	return 1;
    case "8":
	this_object()->print_menu_room_inventory();
	return 1;
    case "9":
	this_object()->print_menu_room_smell();
	return 1;
    case "10":
	this_object()->print_menu_room_listen();
	return 1;
    case "11":
	write("Enter obvious exits, separated by commas: (eg. s, w)"); 
	input_to("set_obvious");
	return 1;
    case "12":
	this_object()->print_menu_room_exits();
	return 1;
    case "13":
	write("Enter a new light level for the daytime: ");
	input_to("set_light");
	return 1;
    case "14":
	write("Enter a new light level for the nighttime: ");
	input_to("set_nightlight");
	return 1;
    case "15":
	this_object()->print_menu_room_enters();
	return 1;

    case "w":
	write("Enter filename within default directory: ( " + defaultdir + 
	  " ) type c to cancel [default: " + (string)this_player()->query_cwd() + " ]");
	input_to("write_to_file");
	return 1;
    case "c": // Clear
	// Reset Everything
	this_object()->reset_everything_in_room ();
	// Print out the main menu again
	this_object()->print_menu_room_create();
	return 1;
    case "d":
	write("Enter default directory: ( " + defaultdir + " ) type q to cancel");
	input_to("set_defaultdir");
	return 1;
    case "p":
	this_object()->print_menu_main();
	return 1;
    default:
	if (process_default (str) == 0) {
	    write("Invalid Choice. Try again.");
	    input_to("process_room_create");
	}
	return 1;
    }
}

//====================================================
//= Prints the Available Climates menu               =
//====================================================

int print_menu_room_climate() {
    write(" Available Climates\n"
      "===================\n\n"
      "1. Indoors\n"
      "2. Temperate\n"
      "3. Arid\n"
      "4. Arctic\n"
      "5. Tropical\n"
      "6. Sub-tropical\n"
      "p. Previous Menu\n\n"
      "Enter Your Choice: ");
    input_to("process_room_climate");
    return 1;
}

//====================================================
//= Processes the input from the Climate menu        =
//====================================================

int process_room_climate(string str) {
    if(str == "p") {
	this_object()->print_menu_room_create();
	return 1;
    }
    if(climates[str]) {
	room_attributes_map["Climate"] = climates[str];
	this_object()->print_menu_room_create();
	return 1;
    }
    if (process_default(str) == 0) {
	write("Invalid Choice. Try again.");
	input_to("process_room_climate");
    }
    return 1;
}

//====================================================
//= Prints the Available Properties                  =
//====================================================

int print_menu_room_properties() {
    write(" Toggle Room Properties\n"
      "=======================\n\n"
      "1. No Attack [current: " + room_properties_map["no attack"] + "]\n"
      "2. No Bump [current: " + room_properties_map["no bump"] + "]\n"
      "3. No Steal [current: " + room_properties_map["no steal"] + "]\n"
      "4. No Magic [current: " + room_properties_map["no magic"] + "]\n"
      "5. No Paralyze [current: " + room_properties_map["no paralyze"] + "]\n"
      "6. No Teleport [current: " + room_properties_map["no teleport"] + "]\n"
      "p. Previous Menu\n\n"
      "Enter Your Choice: ");
    input_to("process_room_properties");
    return 1;
}

//====================================================
//= Processes the input from the Properties menu     =
//====================================================

int process_room_properties(string str) {
    switch(str) {
    case "1":
	this_object()->toggle_attack();
	return 1;
    case "2":
	this_object()->toggle_bump();
	return 1;
    case "3":
	this_object()->toggle_steal();
	return 1;
    case "4":
	this_object()->toggle_magic();
	return 1;
    case "5":
	this_object()->toggle_paralyze();
	return 1;
    case "6":
	this_object()->toggle_teleport();
	return 1;
    case "p":
	this_object()->print_menu_room_create();
	return 1;
    default:
	if (process_default(str) == 0) {
	    write("Invalid Choice. Try again.");
	    input_to("process_room_properties");
	}
	return 1;
    }
}

//====================================================
//= Prints the Add Exits menu                        =
//====================================================

int print_menu_room_exits() {
    write(" Exits Menu\n"
      "==========\n\n"
      "1. Add to MY_ROOM\n"
      "2. Add to absolute room\n"
      "3. Remove Exit\n"
      "c. Clear Exits\n"
      "p. Return to previous menu.\n\n"
      "Enter your choice: ");
    input_to("process_room_exits");
    return 1;
} 

//====================================================
//= Processes the Add Exits Menu                     =
//====================================================

int process_room_exits(string str) {
    string *exitlist = keys (room_exits_map);
    string stempindex = "";
    int exitsize = 0;
    int tempindex;
    switch(str) {
    case "1":
	write("Enter exit direction: ");
	prefix = "MY_ROOM + \"/\" +";
	input_to("set_room_exit");
	return 1;
    case "2":
	write("Enter exit direction: ");
	prefix = "";
	input_to("set_room_exit");
	return 1;
    case "3":
	exitsize = sizeof(exitlist);
	write("  Exits");
	for(int counter = 0; counter < exitsize; counter++) {
	    tempindex = counter + 1;
	    stempindex = (string)tempindex;
	    write(stempindex + ". " + exitlist[counter] + " == > " + room_exits_map[exitlist[counter]]);
	}
	write("Enter Exit number to remove: ");
	input_to("remove_exit");
	return 1;
    case "c":
	room_exits_map = ([]);
	this_object()->print_menu_room_create();
	return 1;
    case "p":
	this_object()->print_menu_room_create();
	return 1;
    default:
	if (process_default(str) == 0) {
	    write("Invalid Choice. Try again.");
	    input_to("process_room_exits");
	}
	return 1;
    }
}

//====================================================
//= Prints the Add Enters menu                        =
//====================================================

int print_menu_room_enters() {
    write(" Enters Menu\n"
      "==========\n\n"
      "1. Add to MY_ROOM\n"
      "2. Add to absolute room\n"
      "3. Remove Enter\n"
      "c. Clear Enters\n"
      "p. Return to previous menu.\n\n"
      "Your choice: ");
    input_to("process_room_enters");
    return 1;
}

//====================================================
//= Processes the Add Enters Menu                     =
//====================================================

int process_room_enters(string str) {
    string *enterlist = keys (room_enters_map);
    string stempindex = "";
    int entersize = 0;
    int tempindex;
    switch(str) {
    case "1":
	write("What is entered? Please remember that if what you enter here ");
	write("does not have a matching item in SetItems or AddItem, your ");
	write("room will not load until you include that item in ");
	write("the room's code.");
	prefix = "MY_ROOM + \"/\" +";
	input_to("set_room_enter");
	return 1;
    case "2":
	write("What is entered? Please remember that if what you enter here ");
	write("does not have a matching item in SetItems or AddItem, your ");
	write("room will not load until you include that item in ");
	write("the room's code.");
	prefix = "";
	input_to("set_room_enter");
	return 1;
    case "3":
	entersize = sizeof(enterlist);
	write("  Enters");
	for(int counter = 0; counter < entersize; counter++) {
	    tempindex = counter + 1;
	    stempindex = (string)tempindex;
	    write(stempindex + ". " + enterlist[counter] + " == > " + room_enters_map[enterlist[counter]]);
	}
	write("Enter number to remove: ");
	input_to("remove_enter");
	return 1;
    case "c":
	room_enters_map = ([]);
	this_object()->print_menu_room_create();
	return 1;
    case "p":
	this_object()->print_menu_room_create();
	return 1;
    default:
	if (process_default(str) == 0) {
	    write("Invalid Choice. Try again.");
	    input_to("process_room_enters");
	}
	return 1;
    }
}

//====================================================
//= Prints the Add Inventory menu                    =
//====================================================

int print_menu_room_inventory() {
    write(" Inventory Menu\n"
      "=====================\n\n"
      "1. Add an object from MY_NPC\n"
      "2. Add an object from MY_AREA\n"
      "3. Enter absolute path for object\n"
      "4. Remove Inventory Item\n"
      "c. Clear Inventory\n"
      "p. Return to previous menu.\n\n"
      "Enter your choice: ");
    input_to("process_room_inventory");
    return 1;
}

//====================================================
//= Processes the Add Inventory Menu                 =
//====================================================

int process_room_inventory(string str) {
    string *invlist = keys (room_inventory_map);
    int tempindex = 0;
    int invsize = 0;
    string stempindex;
    switch(str) {
    case "1":
	write("Enter filename: ");
	prefix = "MY_NPC + \"/\" +";
	input_to("set_room_inventory");
	return 1;
    case "2":
	write("Enter filename: ");
	prefix = "MY_AREA + \"/\" +";
	input_to("set_room_inventory");
	return 1;
    case "3":
	write("Enter absolute filename: ");
	prefix = "";
	input_to("set_room_inventory");
	return 1;
    case "4":
	invsize = sizeof(invlist);
	write("  Inventory Items");
	for(int counter = 0; counter < invsize; counter++) {
	    tempindex = counter + 1;
	    stempindex = (string)tempindex;
	    write(stempindex + ". " + invlist[counter]);
	}
	write("Enter Inventory number to remove: ");
	input_to("remove_inventory");
	return 1;
    case "c":
	room_inventory_map = ([]);
	this_object()->print_menu_room_create();
	return 1;
    case "p":
	this_object()->print_menu_room_create();
	return 1;
    default:
	if (process_default(str) == 0) {
	    write("Invalid choice. Try again.");
	    input_to("process_room_inventory");
	}
	return 1;
    }
}

//====================================================
//= Prints the Add Item menu                         =
//====================================================

int print_menu_room_item() {
    write(" Items Menu\n"
      "===========\n\n"
      "1. Add a single item\n"
      "2. Add multiple items with same description\n"
      "3. Remove Item\n"
      "c. Clear Items\n"
      "p. Return to previous menu.\n\n"
      "Enter your choice: ");
    input_to("process_room_item");
    return 1;
}

//====================================================
//= Processes the Add Items Menu                     =
//====================================================

int process_room_item(string str) {
    string *roomitemlist = keys (room_items_map);
    int itemsize = 0;
    int tempindex = 0;
    int counter = -1;
    string stempindex;
    switch(str) {
    case "1":
	write("Enter item name: ");
	input_to("set_room_item_single");
	return 1;
    case "2":
	write("Enter first item: ");
	input_to("set_room_item_multiple");
	return 1;
    case "3":
	itemsize = sizeof(roomitemlist);
	write("  Items\n");
	//for(int counter = 0; counter < itemsize; counter++) {
	//  tempindex = counter + 1;
	//  stempindex = (string)tempindex;
	//  write(stempindex + ". " + roomitemlist[counter]);
	//}

	foreach(string item_key in roomitemlist){
	    counter ++;
	    tempindex = counter +1 ;
	    stempindex = (string)tempindex;
	    write("size of  roomitemlist is "+sizeof(roomitemlist));
	    write("roomitemlist is "+identify(roomitemlist));
	    write(stempindex + ". " + roomitemlist[counter]);
	}

	write("Enter Item number to remove: ");
	input_to("remove_item");
	return 1;
    case "c":
	room_items_map = ([]);
	this_object()->print_menu_room_create();
	return 1;
    case "p":
	this_object()->print_menu_room_create();
	return 1;
    default:
	if (process_default(str) == 0) {
	    write("Invalid Choice. Try again.");
	    input_to("process_room_item");
	}
	return 1;
    }
}

int remove_item(string str) {
    int index;
    string *roomitemlist = ({});
    int itemsize = -1;
    sscanf(str, "%d", index);
    if (index <= 0) {
	write("Invalid choice.");
	this_object()->print_menu_room_item();
	return 1;
    }

    index--;

    roomitemlist = keys (room_items_map);
    itemsize = sizeof(roomitemlist);

    if(index >= itemsize) {
	write("Invalid choice.");
	this_object()->print_menu_room_item();
	return 1;
    }

    map_delete (room_items_map, roomitemlist[index]);

    this_object()->print_menu_room_item();
    return 1;
}

//====================================================
//= Prints the Add Smell menu                         =
//====================================================

int print_menu_room_smell() {
    write(" Smell Menu\n"
      "===========\n\n"
      "1. Add a single smell\n"
      "2. Add multiple smells with same description\n"
      "3. Remove Smell.\n"
      "c. Clear Smells\n"
      "p. Return to previous menu.\n\n"
      "Please rememeber to AddItem or SetItems the thing "
      "to be smelled, or your room may not load. If it's "
      "the \"default\" smell, then adding that isn't necessary.\n\n"
      "Enter your choice: ");
    input_to("process_room_smell");
    return 1;
}

//====================================================
//= Processes the Add Smell Menu                     =
//====================================================

int process_room_smell(string str) {
    string *roomsmelllist = keys (room_smells_map);
    int smellsize = 0;
    int tempindex = 0;
    string stempindex;
    switch(str) {
    case "1":
	write("Enter item name: ");
	input_to("set_room_smell_single");
	return 1;
    case "2":
	write("Enter first item: ");
	input_to("set_room_smell_multiple");
	return 1;
    case "3":
	smellsize = sizeof(roomsmelllist);
	write("  Smells\n");
	for(int counter = 0; counter < smellsize; counter++) {
	    tempindex = counter + 1;
	    stempindex = (string)tempindex;
	    write(stempindex + ". " + roomsmelllist[counter]);
	}
	write("Enter Smell number to remove: ");
	input_to("remove_smell");
	return 1;
    case "c":
	room_smells_map = ([]);
	this_object()->print_menu_room_create();
	return 1;
    case "p":
	this_object()->print_menu_room_create();
	return 1;
    default:
	if (process_default(str) == 0) {
	    write("Invalid Choice. Try again.");
	    input_to("process_room_smell");
	}
	return 1;
    }
}

int remove_smell(string str) {
    int index;
    string *roomsmelllist = ({});
    int smellsize = -1;
    sscanf(str, "%d", index);
    if (index <= 0) {
	write("Invalid choice.");
	this_object()->print_menu_room_smell();
	return 1;
    }

    index--;

    roomsmelllist = keys (room_smells_map);
    smellsize = sizeof(roomsmelllist);

    if(index >= smellsize) {
	write("Invalid choice.");
	this_object()->print_menu_room_smell();
	return 1;
    }

    map_delete (room_smells_map, roomsmelllist[index]);

    this_object()->print_menu_room_smell();
    return 1;
}

//====================================================
//= Prints the Add Listen menu                         =
//====================================================

int print_menu_room_listen() {
    write(" Listen Menu\n"
      "===========\n\n"
      "1. Add a single listen\n"
      "2. Add multiple listens with same description\n"
      "3. Remove Listen\n"
      "c. Clear Listens\n"
      "p. Return to previous menu.\n\n"
      "Please rememeber to AddItem or SetItems the thing "
      "to be listened to, or your room may not load. If it's "
      "the \"default\" listen, then adding that isn't necessary.\n\n"
      "Enter your choice: ");
    input_to("process_room_listen");
    return 1;
}

//====================================================
//= Processes the Add Listen Menu                     =
//====================================================

int process_room_listen(string str) {
    string *roomlistenlist = keys (room_listens_map);
    int listensize = 0;
    int tempindex = 0;
    string stempindex;
    switch(str) {
    case "1":
	write("Enter item name: ");
	input_to("set_room_listen_single");
	return 1;
    case "2":
	write("Enter first item: ");
	input_to("set_room_listen_multiple");
	return 1;
    case "3":
	listensize = sizeof(roomlistenlist);
	write("  Listen List\n");
	for(int counter = 0; counter < listensize; counter++) {
	    tempindex = counter + 1;
	    stempindex = (string)tempindex;
	    write(stempindex + ". " + roomlistenlist[counter]);
	}
	write("Enter Listen number to remove: ");
	input_to("remove_listen");
	return 1;
    case "c":
	room_listens_map = ([]);
	this_object()->print_menu_room_create();
	return 1;
    case "p":
	this_object()->print_menu_room_create();
	return 1;
    default:
	if (process_default(str) == 0) {
	    write("Invalid Choice. Try again.");
	    input_to("process_room_listen");
	}
	return 1;
    }
}

int remove_listen(string str) {
    int index;
    string *roomlistenlist = ({});
    int listensize = -1;
    sscanf(str, "%d", index);
    if (index <= 0) {
	write("Invalid choice.");
	this_object()->print_menu_room_listen();
	return 1;
    }

    index--;

    roomlistenlist = keys (room_listens_map);
    listensize = sizeof(roomlistenlist);

    if(index >= listensize) {
	write("Invalid choice.");
	this_object()->print_menu_room_listen();
	return 1;
    }

    map_delete (room_listens_map, roomlistenlist[index]);

    this_object()->print_menu_room_listen();
    return 1;
}

//====================================================
//= Sets the town variable                           =
//====================================================

int set_attribute_helper (string str, string keyname) {
    if(str == "c") {
	this_object()->print_menu_room_create();
	return 1;
    }
    room_attributes_map[keyname] = str;
    this_object()->print_menu_room_create();
    return 1;
}

int set_town(string str) {
    return set_attribute_helper (str, "Town");
}

//====================================================
//= Sets the short variable                          =
//====================================================

int set_short(string str) {
    return set_attribute_helper (str, "Short Desc");
}

//====================================================
//= Sets the obvious variable                        =
//====================================================

int set_obvious(string str) {
    return set_attribute_helper (str, "Obvious Exits");
}

//====================================================
//= Sets the night light                             =
//====================================================

int set_nightlight(string str) {
    set_attribute_helper (str, "Light Level (Night)");
}

//====================================================
//= Functions to set properties                      =
//====================================================

int set_light(string str) {
    set_attribute_helper (str, "Light Level (Day)");
}

int toggle_helper (string keyname) {
    if( room_properties_map[keyname] == "0" ) {
	room_properties_map[keyname] = "1";
    } else {
	room_properties_map[keyname] = "0";
    }
    this_object()->print_menu_room_create();
    return 1;
}

int toggle_attack() {
    return toggle_helper ("no attack");
}

int toggle_bump() {
    return toggle_helper ("no bump");
}

int toggle_steal() {
    return toggle_helper ("no steal");
}

int toggle_magic() {
    return toggle_helper ("no magic");
}

int toggle_paralyze() {
    return toggle_helper ("no paralyze");
}

int toggle_teleport() {
    return toggle_helper ("no teleport");
}

//====================================================
//= Sets the Long description                        =
//====================================================

int set_long_description_helper (string str, string keyname, string functionname)
{
    string file;
    string contents;
    if(str == "c") {
	this_object()->print_menu_room_create();
	return 1;
    }
    if(sscanf(str, "load %s", file) == 1) {
	if(!(contents = read_file(file))) {
	    write("Invalid text file.");
	    this_object()->print_menu_room_create();
	    return 1;
	}
	room_attributes_map[keyname] = contents;
	this_object()->print_menu_room_create();
	return 1;
    }
    if(str == "q") {
	// remove the trailing endline.
	sscanf (room_attributes_map[keyname], "%s%*(\\\\n$)", contents);
	room_attributes_map[keyname] = contents;
	this_object()->print_menu_room_create();
	return 1;
    }
    room_attributes_map[keyname] = room_attributes_map[keyname] + str + "\\n";
    input_to(functionname);
    return 1;
}

int set_long(string str) {
    return set_long_description_helper (str, "Long Desc (Day)", "set_long");
}

//====================================================
//= Sets the Night Long description                  =
//====================================================

int set_nightlong(string str) {
    return set_long_description_helper (str, "Long Desc (Night)", "set_nightlong");
}

//====================================================
//= Sets the exit                                    =
//====================================================

int set_room_exit(string str) {
    if(str == "c") {
	this_object()->print_menu_room_create();
	return 1;
    }
    tempdir = "\"" + str + "\"";
    write("Enter room filename: ");
    input_to("set_room_exit_helper");
    return 1;
}

int set_room_exit_helper(string str) {
    if(str == "c") {
	this_object()->print_menu_room_create();
	return 1;
    }
    room_exits_map[tempdir] = prefix + "\"" + str + "\"";
    this_object()->print_menu_room_create();
    return 1;
}

int remove_exit(string str) {
    int index = -1;
    string *exitkeys = ({});
    sscanf(str, "%d", index);  
    if (index <= 0) {
	write ("Invalid choice.");
	this_object()->print_menu_room_exits();
	return 1;
    }

    // decrement the index
    index--;

    // We have the input index and the key array
    exitkeys = keys (room_exits_map);

    if (index >= sizeof (exitkeys)) {
	write ("Invalid choice.");
	this_object()->print_menu_room_exits();
	return 1;
    }

    map_delete (room_exits_map, exitkeys[index]);

    this_object()->print_menu_room_exits();
    return 1;
}

//====================================================
//= Sets the enter                                    =
//====================================================

int set_room_enter(string str) {
    if(str == "c") {
	this_object()->print_menu_room_create();
	return 1;
    }
    tempdir = "\"" + str + "\"";
    write("Enter room filename: ");
    input_to("set_room_enter_helper");
    return 1;
}

int set_room_enter_helper(string str) {
    if(str == "c") {
	this_object()->print_menu_room_create();
	return 1;
    }
    room_enters_map[tempdir] = prefix + "\"" + str + "\"";
    this_object()->print_menu_room_create();
    return 1;
}

int remove_enter(string str) {
    int index = -1;
    string *enterkeys = ({});
    sscanf(str, "%d", index);  
    if (index <= 0) {
	write ("Invalid choice.");
	this_object()->print_menu_room_enters();
	return 1;
    }

    // decrement the index
    index--;

    // We have the input index and the key array
    enterkeys = keys (room_enters_map);

    if (index >= sizeof (enterkeys)) {
	write ("Invalid choice.");
	this_object()->print_menu_room_enters();
	return 1;
    }

    map_delete (room_enters_map, enterkeys[index]);

    this_object()->print_menu_room_enters();
    return 1;
}

//====================================================
//= Sets the inventory items                         =
//====================================================

int set_room_inventory(string str) {
    if(str == "c") {
	this_object()->print_menu_room_create();
	return 1;
    }
    tempdir = str;
    write("Enter number of objects to have: ");
    input_to("set_room_inventory_helper");
    return 1;
}

int set_room_inventory_helper(string str) {
    if(str == "c") {
	this_object()->print_menu_room_create();
	return 1;
    }
    room_inventory_map[prefix + "\"" + tempdir + "\""] = str;
    this_object()->print_menu_room_create();
    return 1;
}

int remove_inventory(string str) {
    int index;
    string *invlist = ({});
    int invsize = -1;
    sscanf(str, "%d", index);
    if (index <= 0) {
	write("Invalid choice.");
	this_object()->print_menu_room_inventory();
	return 1;
    }

    index--;

    invlist = keys (room_inventory_map);
    invsize = sizeof(invlist);

    if(index >= invsize) {
	write("Invalid choice.");
	this_object()->print_menu_room_inventory();
	return 1;
    }

    map_delete(room_inventory_map, invlist[index]);

    this_object()->print_menu_room_inventory();
    return 1;
}

//====================================================
//= Sets the Items                                   =
//====================================================

int set_room_description_helper (string str, mapping room_map, string functionname) {
    string finaldescription = "";
    if(str == "c") {
	reset_temps ();
	this_object()->print_menu_room_create();
	return 1;
    }

    if(str == "q") {
	string array_string = "";
	// We need to remove the ending endline.
	sscanf (objectdescription, "%s%*(\\\\n$)", finaldescription);
	finaldescription = "\"" + finaldescription + "\"";
	write(get_stack());
	if(sizeof(multi_item_array)){
	    foreach(string element in multi_item_array){
		if(array_string == "") array_string = "({";
		array_string += "\""+element+"\"";
		if(multi_item_array[sizeof(multi_item_array)-1] == element){
		    array_string += "})";
		}
		else array_string += ",";
	    }
	}
	if(array_string == "") array_string = tempdir;
	room_map[array_string] = finaldescription;

	// clear the array now
	multi_item_array = ({});
	// Clear the temp variables now.
	reset_temps ();
	this_object()->print_menu_room_create();
	return 1;
    }

    objectdescription = objectdescription + str + "\\n";
    input_to(functionname);
    return 1;
}

int set_room_item_single(string str) {
    if(str == "c") {
	reset_temps ();
	this_object()->print_menu_room_create();
	return 1;
    }
    tempdir = "\"" + str + "\"";
    write("Enter item description. End with q on a blank line.: ");
    input_to("set_room_item_description");
    return 1;
}

int set_room_item_description(string str) {
    return set_room_description_helper (str, room_items_map, "set_room_item_description");
}

int set_room_item_multiple(string str) {
    if(str == "c") {
	reset_temps ();
	this_object()->print_menu_room_create();
	return 1;
    }
    if(str == "q") {
	write("Enter item description. End with q on a blank line: ");
	input_to("set_room_item_description");
	return 1;
    }
    multi_item_array += ({ str });
    write("Enter next item or type q to end");
    input_to("set_room_item_multiple");
    return 1;
}

//====================================================
//= Sets the Smells                                   =
//====================================================

int set_room_smell_single(string str) {
    if(str == "c") {
	reset_temps ();
	this_object()->print_menu_room_create();
	return 1;
    }
    tempdir = "\"" + str + "\"";
    write("Enter smell description. End with q on a blank line.: ");
    input_to("set_room_smell_description");
    return 1;
}

int set_room_smell_description(string str) {
    return set_room_description_helper (str, room_smells_map, "set_room_smell_description");
}

int set_room_smell_multiple(string str) {
    if(str == "c") {
	reset_temps ();
	this_object()->print_menu_room_create();
	return 1;
    }

    if(str == "q") {
	write("Enter smell description. End with q on a blank line: ");
	input_to("set_room_smell_description");
	return 1;
    }

    multi_item_array += ({ str });
    write("Enter next item or type q to end");
    input_to("set_room_smell_multiple");
    return 1;
}

//====================================================
//= Sets the Listens                                 =
//====================================================

int set_room_listen_single(string str) {
    if(str == "c") {
	reset_temps ();
	this_object()->print_menu_room_create();
	return 1;
    }
    tempdir = "\"" + str + "\"";
    write("Enter listen description. End with q on a blank line.: ");
    input_to("set_room_listen_description");
    return 1;
}

int set_room_listen_description(string str) {
    return set_room_description_helper (str, room_listens_map, "set_room_listen_description");
}

int set_room_listen_multiple(string str) {
    if(str == "c") {
	reset_temps ();
	this_object()->print_menu_room_create();
	return 1;
    }

    if(str == "q") {
	write("Enter listen description. End with q on a blank line: ");
	input_to("set_room_listen_description");
	return 1;
    }

    multi_item_array += ({ str });
    write("Enter next listen or type q to end");
    input_to("set_room_listen_multiple");
    return 1;
}

//====================================================
//= Sets the default directory                       =
//====================================================

int set_defaultdir_helper(string str) {
    int index;
    index = strlen(str);
    if (index <= 0) {
	// The user just pressed enter. Change it to the
	// current working directory.
	defaultdir = (string)this_player()->query_cwd();
	return 1;
    }
    index--;
    if(str[index] != '/') str = str + "/";
    defaultdir = str;
    return 1;
}

int set_defaultdir(string str) {
    if(str == "c") {
	write("Change cancelled.");
	this_object()->print_menu_room_create();
	return 1;
    }
    set_defaultdir_helper (str);
    this_object()->print_menu_room_create();
    return 1;
}

int set_defaultdir_main(string str) {
    if(str == "c") {
	write("Change cancelled.");
	this_object()->print_menu_main();
	return 1;
    }
    set_defaultdir_helper (str);
    this_object()->print_menu_main();
    return 1;
}



//====================================================
//= Writes all the stuff to the room file            =
//====================================================

int write_to_file(string str) {
    if(str == "q") {
	write("Write cancelled");
	this_object()->print_menu_room_create();
	return 1;
    }
    str = defaultdir+str;
    reset_room_header ();
    globalstr = str;
    globalstr2 = header +
    //unguarded( (: write_file(globalstr, header +
    "  SetTown(\"" + room_attributes_map["Town"] + "\");\n" +
    "  SetClimate(\"" + room_attributes_map["Climate"] + "\");\n";

    if(!check_prop(room_attributes_map["Long Desc (Night)"])) {
	globalstr2 += "  SetAmbientLight(" + room_attributes_map["Light Level (Day)"] + ");\n";
    }
    else {
	globalstr2 += "  SetDayLight(" + room_attributes_map["Light Level (Day)"] + ");\n"+
	"  SetNightLight(" + room_attributes_map["Light Level (Night)"] + ");\n";
    }

    globalstr2 += "  SetShort(\"" + room_attributes_map["Short Desc"] + "\");\n";

    if(!check_prop(room_attributes_map["Long Desc (Night)"])){
	globalstr2 += "  SetLong(\"" + room_attributes_map["Long Desc (Day)"] + "\");\n";
    }
    else {

	globalstr2 +=  "  SetDayLong(\"" + room_attributes_map["Long Desc (Day)"] + "\");\n" +
	"  SetNightLong(\"" + room_attributes_map["Long Desc (Night)"] + "\");\n";
    }

    globalstr2 +=  print_properties_for_file () +
    print_room_items_for_file () +
    print_room_inventory_for_file () +
    "  SetObviousExits(\"" + room_attributes_map["Obvious Exits"] + "\");\n" +
    print_room_exits_for_file () +
    print_room_enters_for_file () +
    "//funs\n" +
    extrafuns +
    "//snuf\n" +
    "}\n" +
    "void init() {\n::init();\n" +
    print_room_smells_for_file () +
    print_room_listens_for_file () +
    "}\n" +
    "//extras\n" +
    tail;
    unguarded( (: write_file(globalstr, globalstr2, 1) :) );

    write("\nRoom written to: " + str + "\n");
    this_object()->print_menu_room_create();
    return 1;
}

//=================================================
//= Room Loading Section                          =
//=================================================

int get_town(string str) {
    sscanf(str, "%*sSetTown(\"%s\");%*s", room_attributes_map["Town"]);
    return 1;
}

int get_climate(string str) {
    sscanf(str, "%*sSetClimate(\"%s\");%*s", room_attributes_map["Climate"]);
    return 1;
}

int get_short(string str) {
    sscanf(str, "%*sSetShort(\"%s\");%*s", room_attributes_map["Short Desc"]);
    return 1;
}

int get_daylong(string str) {
    if(sscanf(str, "%*sSetDayLong(\"%s\");%*s", room_attributes_map["Long Desc (Day)"]) == 0) {
	sscanf(str, "%*sSetLong(\"%s\");%*s", room_attributes_map["Long Desc (Day)"]);
    }
    return 1;
}

int get_nightlong(string str) {
    if(strsrch(str,"SetNightLong")){
	sscanf(str, "%*sSetNightLong(\"%s\");%*s", room_attributes_map["Long Desc (Night)"]);
    }
    return 1;
}

int get_daylight(string str) {
    if(sscanf(str, "%*sSetDayLight(%s);%*s", room_attributes_map["Light Level (Day)"]) == 0){
	sscanf(str, "%*sSetAmbientLight(%s);%*s", room_attributes_map["Light Level (Day)"]);
    }
    return 1;
}

int get_nightlight(string str) {
    if(strsrch(str,"SetNightLight")){
	sscanf(str, "%*sSetNightLight(%s);%*s", room_attributes_map["Light Level (Night)"]);
    }
    return 1;
}

void add_map_to_mapping (mapping room_map, string inputstring) {
    // We need to split up items, and add them to room_map
    string *itemlist = explode (inputstring, ", ");
    string strKey = "";
    string strValue = "";
    for (int counter = 0; counter < sizeof (itemlist); counter++) {
	strKey = "";
	strValue = "";
	sscanf (itemlist[counter], "%s:%s", strKey, strValue);

	//sscanf (itemlist[counter], "%*s\"%s\"%*s:%*s\"%s\"%*s", strKey, strValue);
	if (strlen (strKey) > 0) {
	    room_map[strKey] = strValue;
	}
    }
}

int get_items(string str) {
    string strItems, key, val;
    string *elements;
    if(sscanf(str, "%*sSetItems(([%s]));%*s", strItems) < 1) {
	return 1;
    }
    //elements = explode(strItems," ,\\n");
    elements = explode(strItems," ,");
    if(!elements) elements = ({ strItems });
    if(!elements) {
	return 1;
    }
    foreach(string element in elements){
	if(sscanf(element, "%s : %s", key, val ) < 2) break;
	room_items_map[key] = val;
    }
    return 1;
}

int get_smell(string str) {
    string strItems = "";
    sscanf(str, "%*sSetSmell(([%s]));%*s", strItems);
    add_map_to_mapping (room_smells_map, strItems);
    return 1;
}

int get_listen(string str) {
    string strItems = "";
    sscanf(str, "%*sSetListen(([%s]));%*s", strItems);
    add_map_to_mapping (room_listens_map, strItems);
    return 1;
}

string strip_whitespaces (string str) {
    string temp = "";
    string final = "";
    sscanf (str, "%*(^ *)%s", temp);
    sscanf (temp, "%s%*( *$)", final);
    return final;
}

int get_inventory(string str) {
    string strItems = "";
    string strKey = "";
    string strValue = "";
    string *itemlist;
    sscanf(str, "%*sSetInventory(([%s]));%*s", strItems);
    // We need to split up items, and add them to room_map
    itemlist = explode (strItems, ",");
    for (int counter = 0; counter < sizeof (itemlist); counter++) {
	strKey = "";
	strValue = "";
	sscanf (itemlist[counter], "%s:%s", strKey, strValue);
	strKey = strip_whitespaces (strKey);
	strValue = strip_whitespaces (strValue);
	if (strlen (strKey) > 0) {
	    room_inventory_map[strKey] = strValue;
	}
    }
    return 1;
}

int get_obviousexits(string str) {
    sscanf(str, "%*sSetObviousExits(\"%s\");%*s", room_attributes_map["Obvious Exits"]);
    return 1;
}

int get_properties(string str) {
    string fixedstring = "";
    sscanf(str, 
      "%*s\"no attack\":%s,"
      "%*s\"no bump\":%s,"
      "%*s\"no steal\":%s,"
      "%*s\"no magic\":%s,"
      "%*s\"no paralyze\":%s,"
      "%*s\"no teleport\":%s]", 
      room_properties_map["no attack"], 
      room_properties_map["no bump"],
      room_properties_map["no steal"],
      room_properties_map["no magic"],
      room_properties_map["no paralyze"],
      room_properties_map["no teleport"]);
    if (sscanf (room_properties_map["no teleport"], "%s\n", fixedstring) == 1) {
	room_properties_map["no teleport"] = fixedstring;
    }
    return 1;
}

int get_exits(string str) {
    string *exittemp = explode(str, ";");
    string exitarguments = "";
    string strDirection = "";
    string strFile = "";
    for(int counter = 0; counter < sizeof(exittemp); counter++) {
	strDirection = "";
	strFile = "";
	sscanf(exittemp[counter], "%*sAddExit(%s)%*s", exitarguments);
	// Exit can have more than two arguments. We need to check for that
	if (sscanf(exitarguments, "\"%s\", %s,", strDirection, strFile) != 2) {
	    sscanf (exitarguments, "\"%s\", %s", strDirection, strFile);
	}
	if (strlen (strDirection) > 0) {
	    strDirection = "\"" + strDirection + "\"";
	    room_exits_map[strDirection] = strFile;
	}
    }
    return 1;
}

int get_enters(string str) {
    string *entertemp = explode(str, ";");
    string enterarguments = "";
    string strDirection = "";
    string strFile = "";
    for(int counter = 0; counter < sizeof(entertemp); counter++) {
	strDirection = "";
	strFile = "";
	sscanf(entertemp[counter], "%*sAddEnter(%s)%*s", enterarguments);
	// Enter can have more than two arguments. We need to check for that
	if (sscanf(enterarguments, "\"%s\", %s,", strDirection, strFile) != 2) {
	    sscanf (enterarguments, "\"%s\", %s", strDirection, strFile);
	}
	if (strlen (strDirection) > 0) {
	    strDirection = "\"" + strDirection + "\"";
	    room_enters_map[strDirection] = strFile;
	}
    }
    return 1;
}

int get_header(string str) {
    sscanf(str, "%s::create%*s", header);
    header = header + "::create();\n";
    return 1;
}

int get_tail(string str) {
    if(sscanf(str, "%*s//extras\n%s", tail) != 2) {
	tail = "";
    }
    return 1;
}

int get_extrafuns(string str) {
    if(sscanf(str, "%*s//funs\n%s//snuf%*s", extrafuns) != 3) {
	extrafuns = "";
    }
    return 1;
}

int read_room(string str) {
    string file;
    str = defaultdir+str;

    if(file_size(str) < 0) {
	write("Invalid file. Perhaps you forgot to add \".c\"? Please try again.");
	this_object()->print_menu_main();
	return 1;
    }
    if(!(file = read_file(str))) {
	write("A permissons error occurred. You'll have to start over.");
	this_object()->print_menu_main();
	return 1;
    }
    get_town(file);
    get_climate(file);
    get_short(file);
    get_daylong(file);
    get_nightlong(file);
    get_daylight(file);
    get_nightlight(file);
    get_items(file);
    get_smell(file);
    get_listen(file);
    get_inventory(file);
    get_obviousexits(file);
    get_properties(file);
    get_exits(file);
    get_enters(file);
    get_header(file);
    get_tail(file);
    get_extrafuns(file);
    this_object()->print_menu_room_create();
    return 1;
}
