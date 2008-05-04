#include <lib.h>

inherit LIB_ITEM;

static void create(){
    item::create();
    AddSave( ({"KeyName", "ExternalDesc","Id","Short","Value","Cost","DestroyOnSell" }) );
}

