#include <lib.h>
#include <vendor_types.h>
inherit LIB_ITEM;
inherit LIB_READ;

void create(){
    ::create();
    SetKeyName("sign");
    SetId( ({"thing","item","thang","dingus"}) );
    SetAdjectives( ({"generic","sample","template"}) );
    SetShort("a sign");
    SetLong("This is an object of indeterminate nature and proportions.");
    SetItems(([
                ({ "words", "word", "this is the first read" }) : "These are the smallest units of vocabulary of a language.",
                ]));
    SetRead(([
                ({ "word", "words", "this is the first read" }) : "this is the first read",
                ]));
    SetLanguage("farsi");
    SetDefaultRead("Hopefully this is not garbled.");
    SetMass(20);
    SetBaseCost("silver",10);
    SetVendorType(VT_TREASURE);
}
void init(){
    ::init();
}
