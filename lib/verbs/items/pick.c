/*    /verbs/items/pick.c
 *    from the Dead Souls Mud Library
 *    created by Descartes of Borg 951220
 *    Version: @(#) pick.c 1.4@(#)
 *    Last modified: 96/10/15
 */

#include <lib.h>
#include <rounds.h>
#include "include/pick.h"

inherit LIB_VERB;

static void create() {
    verb::create();
    SetVerb("pick");
    SetRules("STR on OBJ", "STR on OBJ with OBJ", "OBJ");
    SetErrorMessage("You might pick a flower or perhaps pick the lock on "
            "something?");
    SetHelp("Syntax: <pick OBJECT>\n"
            "        <pick lock on OBJECT>\n"
            "        <pick lock on OBJECT with TOOL>\n\n"
            "Depending on what you are doing, this command captures two "
            "different senses of the verb \"pick\".  In the first sense, "
            "<pick OBJECT>, pick allows you to pick things like flowers or "
            "strawberries (not your nose).\n\n"
            "The second conext allows you to open locked things without a key.  "
            "Some tools can help you stealthfully pick a lock, while "
            "others may help you pick it through brute force.\n\n"
            "See also: close, lock, open, unlock");
}

mixed can_pick_obj() {
    if( this_player()->GetParalyzed() ) {
        return "You cannot do anything.";
    }
    return this_player()->CanManipulate();
}

mixed can_pick_str_on_obj(string str) {
    if( this_player()->GetParalyzed() ) {
        return "You cannot do anything.";
    }
    if( this_player()->GetStaminaPoints() < 20 ) {
        return "You are too tired.";
    }
    return this_player()->CanManipulate();
}

mixed can_pick_str_on_obj_with_obj(string str) {
    if( this_player()->GetParalyzed() ) {
        return "You cannot do anything.";
    }
    if( this_player()->GetStaminaPoints() < 30 ) {
        return "You are too tired.";
    }
    return this_player()->CanManipulate();
}

mixed do_pick_obj(object ob, string id) {
    return ob->eventPickItem(this_player(), remove_article(lower_case(id)));
}

mixed do_pick_str_on_obj(string wrd, object ob, mixed *args...) {
    wrd = remove_article(lower_case(args[1]));
    this_player()->eventPrint("You eye the lock for weaknesses.");
    if( this_player()->GetInCombat() )
        this_player()->SetAttack(0, (: $(ob)->eventPick(this_player(), $(wrd)):),
                ROUND_OTHER);
    else ob->eventPick(this_player(), wrd);
    return 1;
}

mixed do_pick_str_on_obj_with_obj(string wrd, object ob, object tool,
        mixed *args...) {
    wrd = remove_article(lower_case(args[1]));
    this_player()->eventPrint("You eye the lock for weaknesses.");
    if( this_player()->GetInCombat() )
        this_player()->SetAttack(0, (: $(ob)->eventPick(this_player(), $(wrd),
                        $(tool)) :),ROUND_OTHER);
    else ob->eventPick(this_player(), wrd, tool);
    return 1;
}
