/*    /lib/net/socket.c
 *    From the Dead Souls Object Library
 *    Handles individual socket I/O
 *    Created by Descartes of Borg 961218
 *    Version: @(#) socket.c 1.1@(#)
 *    Last modified: 96/12/19
 */

#include <lib.h>

inherit LIB_DAEMON;

private static int    Descriptor = -1;
private static object Owner      = 0;

/* *********************** socket.c attributes ********************* */
string GetAddress() {
    return socket_address(Descriptor);
}

int GetDescriptor() {
    return Descriptor;
}

/* ************************* socket.c events *********************** */
static int eventCloseSocket() {
    if(!Owner) return 0;
    Owner->eventClose(this_object());
}

int eventRead(mixed data) {
    if( previous_object() != Owner ) {
        return 0;
    }
    return 1;
}

int eventDestruct(){
    return daemon::eventDestruct();
}

int eventSocketClosed() {
    if(!Owner || previous_object() != Owner ) {
        return 0;
    }
    eventDestruct();
    return 1;
}


varargs static int eventWrite(mixed data, int close) {
    if(!close) close = 0;
    if(!data) data = ({});
    if(Owner && this_object()) return Owner->eventWrite(this_object(), data, close);
    else return 0;
}

/* ******************** socket.c driver applies ****************** */
static void create(int fd, object owner) {
    daemon::create();
    SetNoClean(1);
    Descriptor = fd;
    Owner = owner;
}
