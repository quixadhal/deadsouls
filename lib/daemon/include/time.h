#ifndef __TIME_H__
#define __TIME_H__

class timezone {
    int Offset;
    string Name;
}

static void create();
int GetOffset(string tzone);
string GetName(string tzone);
string *GetTimeZones();

#endif /* __TIME_H__ */
