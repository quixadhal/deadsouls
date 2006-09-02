/*
 * Seeks dummy items without an environment and destroys them.
 * Hackey workaround to sloppy code I'm still tracking down.
 * -Crat 21Jan06
 *
 * Started as a hackey workaround to a dummy problem, but
 * it turns out that periodically destructing  cloned
 * objects without an environment is a pretty good idea.
 * see the reaper.c sefun file for how the dests happen
 * and what objects are exempted.
 * -Crat 28Jul06
 */

#include <lib.h>

inherit LIB_DAEMON;

static void eventReap() {

    call_out((: eventReap :), 300);
    reap_dummies();
    reap_other();
}

static void create() {
    daemon::create();
    SetNoClean(1);
    call_out((: eventReap :), 300);
}

