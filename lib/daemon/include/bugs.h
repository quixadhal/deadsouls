#ifndef l_bugs_h
#define l_bugs_h

static void create();
int eventAssign(int bug, string who);
int eventComplete(int bug, string resolution);
int eventDelete(int bug);
int eventReport(string who, string type, string bug, string data);
int GetAssignedBugs(string who);
mapping GetBugs();
string AddComment(int bug, string comment);

#endif l_bugs_h
