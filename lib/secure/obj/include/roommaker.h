#ifndef l_roommaker_h
#define l_roommaker_h

static void create();
void eventAbort();
varargs mixed eventCreateRoom(string file, string climate);
static void eventPrintTitle(string title);
static void MainMenu();
static void MainMenuCommand(string str);
static void ClimateMenu();
static void ClimateMenuCommand(string cmd);
static void TownMenu();
static void TownMenuCommand(string cmd);
static void PropertiesMenu();
static void PropertiesMenuCommand(string cmd, string *props);
static void ShortMenu();
static void ShortMenuCommand(string str);
static void LongMenu();
static void LongMenuCommand(string cmd);
static void InventoryMenu();
static void InventoryMenuCommand(string cmd, string *items);
static void ItemsMenu();
static void ItemsMenuCommand(string cmd, string *items);
void CompleteItem(string *items);
static void PropertiesMenu();
static void PropertiesMenuCommand(string cmd, string *props);
static void DirsMenu(string type);
static void DirsMenuCommand(string cmd, string type, string *sorties);
static void ObviousExitsMenu();
static void ObviousExitsMenuCommand(string cmd);
static void SaveRoom();

varargs static string GetValue(mixed val, string str);

#endif /* l_roommaker_h */
