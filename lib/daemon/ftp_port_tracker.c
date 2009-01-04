/*
 ** ftp_port_tracker.c
 ** Tracks which passive ports are being used by the FTP daemon.
 **
 ** Created by Zaxan@Haven
 */

#include <lib.h>

#include "include/ftp_port_tracker.h"

inherit LIB_DAEMON;

static int array UsedPorts = ({ });

/*
 ** Adds the specified port to the UsedPorts list. Returns
 ** 1 on success, 0 on error.
 */
int AddUsedPort(int port)
{
    if(member_array(port, UsedPorts) != -1) return 0;


    UsedPorts += ({ port });
    return 1;
}

/*
 ** Removes the specified port from the UsedPorts list. Returns
 ** 1 on success, 0 on error.
 */
int RemoveUsedPort(int port)
{
    if(member_array(port, UsedPorts) == -1) return 0;

    UsedPorts -= ({ port });
    return 1;
}

/*
 ** Clears all ports from the UsedPorts list.
 */
void ClearUsedPorts()
{

    UsedPorts = ({ });
}

/*
 ** Returns a list of used ports.
 */
int array QueryUsedPorts()
{
    return UsedPorts;
}


