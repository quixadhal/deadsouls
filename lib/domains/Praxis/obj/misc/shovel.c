//	shovel.c created by Descartes 921004
//	it digs in rooms that look for shovels when digging is done

#include <lib.h>

inherit LIB_ITEM;

void init() {
    ::init();
    add_action("dig", "dig");
}

void create() {
    ::create();
    SetId( ({ "shovel" }) );
    SetKeyName("shovel");
    SetShort( "a shovel");
    SetLong( "You might be able to dig with this in the right soil.");
    SetMass(100);    SetValue( 18);

}

int dig(string str) {
    notify_fail("The shovel can't quite manage the soil here.\n");
    return 0;
}
