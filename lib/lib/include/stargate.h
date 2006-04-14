/**
 * 
 * $Id: stargate.h,v 1.1 2006/04/05 05:48:39 jam Exp $
 *
 */

#include "/include/stargate.h"

#ifndef lib_stargate_h
#define lib_stargate_h

void setOrigin(string o, string d);
string getOrigin();
void connect(string destination);
int disconnect();
string status();

#endif
