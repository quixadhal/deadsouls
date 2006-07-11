#ifndef l_connect_h
#define l_connect_h

static void create(int binary);
static void logon();
static void InputName(string name, string kill_me);
static void InputPassword(string pass, string cap);
static void eventCreatePlayer(string cap);
static void ConfirmName(string ans, string cap);
static void eventReEnterGame(string cap);
static void ConfirmReconnect(string ans, string cap);
static int CanLogin();
static private void eventEnterGame();
static void CreatePassword(string pass, string cap);
static void ConfirmPassword(string control, string cap, string pass);
static void InputGender(string str, string cap);
static void InputCapName(string name, string cap);
static void InputEmail(string email);
static void InputRealName(string rname);
static void InputRace(string str);
static void cmdHelp(string str);
static void cmdList();
static void cmdPick(string args);
static void eventTimeout();
varargs int eventPrint(string msg, mixed arg2, mixed arg3);
static private void Destruct();
void eventDestruct();
string GetKeyName();
string GetCapName();
static void terminal_type(string str);
static void window_size(int width, int height);
static void CreateGuest();
static void GetGuestName(string nom);
int *GetScreen();
void SetLastError(mapping m);
mapping GetLastError();

#endif /* l_connect_h */
