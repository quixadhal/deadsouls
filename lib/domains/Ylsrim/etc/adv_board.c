/*    /domains/Ylsrim/etc/adv_board.c
 *    from the Dead Souls Mud Library
 *    created by Rush@Dead Souls
 */

#include <lib.h>

inherit "/lib/bboard";

static void create()
{
    ::create();
    SetKeyName("bulletin board");
    SetId( ({"board", "bulletin board"}) );
    SetAdjectives("stupid");
    SetShort("a bulletin board");
    SetLong("Adventurers passing through Ylsrim post their thoughts here.");
    set_board_id("ylsrim");
}


