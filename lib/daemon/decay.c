#include <lib.h>

inherit LIB_DAEMON;

void eventDecay();

static void create() {
    daemon::create();
    SetNoClean(1);
    call_out("eventDecay", 30);
}

static void eventDecay() {
    object *limbs = filter(findobs(LIB_LIMB), (: clonep($1) :) );
    object *corpses = filter(findobs(LIB_CORPSE), (: clonep($1) :) );

    corpses += find_inheritors(LIB_CORPSE);
    limbs += find_inheritors(LIB_LIMB);

    catch( limbs->eventDecay() );
    catch( corpses->eventDecay() );

    call_out("eventDecay", 30);
    return;
}
