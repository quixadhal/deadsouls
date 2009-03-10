#include <lib.h>

inherit LIB_BASE_TRAINER;
inherit LIB_SENTIENT;

static void create(){
    base_trainer::create();
    sentient::create();
}
