#ifndef __INTERFACE_H__
#define __INTERFACE_H__

static void create();
static string process_input(string str);
static void terminal_type(string str);
static void window_size(int width, int height);
void receive_message(string msg_class, string msg);
static void receive_snoop(string msg);

varargs int eventPrint(string msg, mixed arg2, mixed arg3);

static string cache_commands(string str);
int Setup();

varargs int SetBlocked(string type, int flag);
int GetBlocked(string type);
int SetClient(int x);
int GetClient();
int SetLogHarass(int x);
int GetLogHarass();
int *SetScreen(int width, int height);
int *GetScreen();
string SetTerminal(string terminal);
string GetTerminal();
string GetKeyName();

#endif /* __TERMINAL_H__ */
