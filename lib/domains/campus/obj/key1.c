/*    /domains/Midian/etc/key.c
 *    from the Dead Souls Object Library
 *    created by Descartes of Borg 960512
 */

#include <lib.h>

inherit LIB_ITEM;


static void create() {
    item::create();
    SetKeyName("key");
    SetId(({"key","locker_key_1"}));
    SetAdjectives(({"locker","small",}));
    SetShort("a small key");
    SetLong("This is a small key, perhaps to a padlock or locker.");
    SetMass(1);
    SetDollarCost(2);
    SetDisableChance(90);
}
void init(){
    ::init();
}
