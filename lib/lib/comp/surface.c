#include <lib.h>

inherit LIB_STORAGE;

void create(){
    storage::create();
}

int eventPutOn(object player, object item){
    return true(player,item);
}
