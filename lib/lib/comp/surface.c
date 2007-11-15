#include <lib.h>

inherit LIB_STORAGE;
inherit LIB_ROLL;

void create(){
    storage::create();
}

int eventPutOn(object player, object item){
    return 1;
}
