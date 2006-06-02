#include "/include/stargate.h"

#ifndef daemon_stargate_h
#define daemon_stargate_h

void eventLoad();
void eventSave();
int SetStargate(string address, string destination);
int RemoveStargate(string address);
mapping GetStargates();
int SetStatus(string address, string status);
string GetStatus(string address);
string GetDestination(string address);
string GetEndpoint(string address);
int eventConnect(string from, string to);
int eventDisconnect(string from);

#endif
