/*    /daemon/domains.c
 *    From the Dead Souls Object Library
 *    A daemon for handling domain stat information
 *    Created by Descartes of Borg 961110
 *    Version: %A%
 *    Last modified: %D%
 */

#include <lib.h>

inherit LIB_DAEMON;

class domain_stats {
    int errors;
    int moves;
    int value;
}

private mapping Stats = ([]);

static void create() {
    daemon::create();
    SetNoClean(1);
}

void AddMove() {
}
