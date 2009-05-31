//  /domains/Praxis/obj/misc/stone_pile.c
//  A stone pile/stones for stoning criminals.
//  For use with the Nightmare Mortal Law system
//  Created by Manny@Nightmare  940901

#include <lib.h>

inherit LIB_ITEM;

object target;
void set_target(object who);

void create() {
    ::create();
    SetKeyName("pile");
    SetId( ({ "stones", "pile of stones", "pile" }) );
    SetShort("a pile of stones");
    SetLong("A pile of nice round stones, perfect for throwing at "
            "outlaws and criminals.");
    SetPreventGet("You can only get one stone at a time.");
}

void init() {
    ::init();
    add_action("get_stone", "get");
}

int get_stone(string str) {
    object stone;

    if(!str || str != "stone") return 0;
    message("say", "You pick up a stone from the pile.  You feel a "
            "strong desire to try <throw>ing it at "
            +target->query_cap_name()+"...", this_player());
    message("say", this_player()->query_cap_name()+" picks up a stone "
            "from the pile.", environment(this_player()), this_player());
    stone = new("/"+__DIR__+"stone");
    stone->set_target(target);
    stone->move(this_player());
    return 1;
}

void set_target(object who) { target = who; }


