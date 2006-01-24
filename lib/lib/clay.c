#include <lib.h>
inherit LIB_STORAGE;
inherit LIB_SURFACE;

void create(){
    ::create();
    SetKeyName("lump");
    SetId( ({"lump","clay","mass"}) );
    SetAdjectives( ({"lump of","clay","mass of","heavy", "shapeless"}) );
    SetShort("a lump of clay");
    SetLong("This is a heavy, shapeless mass of clay.");
    SetNoCondition(1);
    SetCanClose(0);
    SetMaxCarry(200);
    SetMass(200);
}

void init(){
    ::init();
}
