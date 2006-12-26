/**
 *
 * $Id: stargate.h,v 1.1 2006/04/05 05:48:39 jam Exp $
 *
 */

#ifndef stargate_h
#define stargate_h

#define GATE_TIMEOUT (10+random(5))

// address (i.e. "sgc") is a key in a mapping in the stargate daemon so it is not included here
class stargate
{
    string status;
    string destination;
    string endpoint;
}

#endif
