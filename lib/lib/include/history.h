#ifndef l_history_h
#define l_history_h

static string eventHistory(string str);
nomask private static string Push(string cmd);
private string GetHistory(mixed val);

/* pure abstract */ varargs int eventPrint(string msg, int cl);
/* pure abstract */ string GetKeyName();

#endif /* l_history_h */
