#include <lib.h>

inherit LIB_DAEMON;

string *deleta;

static void eventUpdate() {
}

static void create() {
    daemon::create();
    call_out((: eventUpdate :), 60);
}

