/*    /std/estate.c
 *    from Nightmare IV
 *    a standard estate entrance
 *    created by Descartes of Borg 940702
 */

#include <std.h>

inherit OBJECT;

string __Exit;

void create() {
    ::create();
    SetId( ({ "estate" }) );
    SetPreventGet("You cannot get an entire estate!");
    SetPreventPut("How could you do that with an estate?");
    SetPreventDrop("One wonders how you got this to begin with.");
    SetNoClean(1);
}

void init() {
    ::init();
    add_action("cmd_enter", "enter");
}

void set_exit(string str) { __Exit = str; }

static int cmd_enter(string str) {
    if(present(str, environment(this_object())) != this_object()) return 0;
    this_player()->move_player(__Exit, "into the estate");
    return 1;
}

void SetShort(string str) {
    SetId(GetId() + ({ lower_case(str) }) );
    ::SetShort(str);
}

