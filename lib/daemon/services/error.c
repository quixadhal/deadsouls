/*    /daemon/services/error.c
 *    from the Foundation II LPC Library
 *    error handling for the grand Intermud 3 Protocol
 *    created by Descartes of Borg 950715
 */

#include <logs.h>
#include <daemons.h>

void eventReceiveError(mixed *packet) {
    object ob;
    string error_code, mud, target, msg;

    tn("ERROR RECEIVED: "+identify(packet));
    LOG_D->LogSpecial(LOG_I3ERR ,timestamp()+" "+identify(packet)+"\n");
    if( packet[5] ) {
        tn("exceptio probat regulam");
        target = convert_name(packet[5]);
        if( !(ob = find_player(target)) ) write("Can't find "+packet[5]);;
    }
    mud = packet[2];
    error_code = packet[6];
    //msg = packet[7];
    //packet = packet[8];
    //tn("errorcode: "+error_code);
    switch(error_code) {
        case "unk-src":
            //update("/daemon/intermud");
            return;
        case "unk-dst": case "not-imp": case "unk-type":
            return;
        case "unk-user":
            if( !ob ) return;
            message("system", (msg ? msg : "Unknown user reported from " + mud +
                        "."), ob);
            return;
    }
}
