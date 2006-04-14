#include "/include/stargate.h"

#ifndef daemon_stargate_h
#define daemon_stargate_h

void load();
void save();
int setStargate(string address, string destination);
class stargate getStargate(string address);
int delStargate(string address);
mapping getStargates();
int setStatus(string address, string status);
string getStatus(string address);
string getDestination(string address);
string getEndpoint(string address);
int connect(string from, string to);
int disconnect(string from);

#endif
