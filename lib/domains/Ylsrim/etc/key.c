/*    /domains/Ylsrim/etc/key.c
 *    from the Dead Souls Object Library
 *    created by Descartes of Borg 960512
 */

#include <lib.h>

inherit LIB_ITEM;

static void create() {
    item::create();
    SetKeyName("key");
    SetId( ({ "key", "special_key_id" }) );
    SetAdjectives( ({ "brilliant", "silver" }) );
    SetShort("a silver key");
    SetLong("It is a brilliant silver key with no markings.");
    SetMass(50);
    SetValue(15);
    SetDisableChance(90);
}

