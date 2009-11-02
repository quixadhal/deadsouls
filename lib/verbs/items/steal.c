/*    /verbs/items/steal.c
 *    from the Dead Souls Mud Library
 *    steal WRD from LIV
 *    steal OBJ from LIV
 *    created by Descartes of Borg 951218
 */



#include <lib.h>
#include <rounds.h>
#include "include/steal.h"

inherit LIB_VERB;

static void create() {
    verb::create();
    SetVerb("steal");
    SetRules("OBJ from LIV", "WRD from LIV", "OBS from LIV");
    SetErrorMessage("Steal what from whom?");
    SetHelp("Syntax: <steal money from LIVING>\n"
            "        <steal ITEM from LIVING>\n"
            "        <steal all [of ITEMS] from LIVING>\n\n"
            "Uses your stealing abilities to rob another of items or money.  "
            "Items are much harder to steal than money, and trying to steal "
            "multiple items in one shot is dangerous.");
}

mixed can_steal_wrd_from_liv(string wrd) {
    if( wrd != "money" ) return 0;
    if( this_player()->GetSkillLevel("stealing") < 1 )
        return "You are not skillful enough at stealing.";
    if( environment(this_player())->GetProperty("no steal") )
        return "Mystical forces prevent your thievery.";
    if( this_player()->GetStaminaPoints() < 10 )
        return "You are too tired for such skullduggery.";
    if(intp(check_light())) return this_player()->CanManipulate();
    else return check_light();
}

mixed can_steal_obj_from_liv() {
    if( this_player()->GetSkillLevel("stealing") < 1 )
        return "You are not skillful enough at stealing.";
    if( environment(this_player())->GetProperty("no steal") )
        return "Mystical forces prevent your thievery.";
    if( this_player()->GetStaminaPoints() < 20 )
        return "You are too tired for such skullduggery.";
    if(intp(check_light())) return this_player()->CanManipulate();
    else return check_light();
}

mixed do_steal_wrd_from_liv(string wrd, object liv) {
    this_player()->eventPrint("You eye " + liv->GetName() +
            " with thoughts on " + possessive(liv) +
            " pockets.");
    if( this_player()->GetInCombat() )
        this_player()->SetAttack(0, (: eventSteal,this_player(), "money", liv :),
                ROUND_OTHER);
    else eventSteal(this_player(), "money", liv);
    return 1;
}

mixed do_steal_obj_from_liv(object item, object liv) {
    if(!item) return "That's not here.";
    if( environment(item) != liv ) {
        this_player()->eventPrint(liv->GetName() + " does not have that.");
        return 1;
    }
    if(item->GetProperty("no steal")){
        this_player()->eventPrint("that item cannot be stolen.");
        return 1;
    }
    this_player()->eventPrint("You eye " + liv->GetName() +
            " with thoughts on " + possessive(liv) + " " +
            remove_article(item->GetShort()) + ".");
    if( this_player()->GetInCombat() )
        this_player()->SetAttack(0, (: eventSteal, this_player(), ({ item }),
                    liv :), ROUND_OTHER);
    else eventSteal(this_player(), ({ item }), liv);
    return 1;
}

mixed do_steal_obs_from_liv(mixed *res, object liv) {
    object *obs;

    obs = filter(res, (: objectp :));
    if( !sizeof(obs) ) {
        mixed *ua;

        ua = unique_array(res, (: $1 :));
        foreach(mixed *lines in ua) this_player()->eventPrint(lines[0]);
        return 1;
    }

    foreach(mixed thing in res){
        if(objectp(thing) && thing->GetProperty("no steal")){
            this_player()->eventPrint("One of those items cannot be stolen, causing "+
                    "you to be distracted from stealing any of the others.");
            return 1;
        }
    }
    this_player()->eventPrint("You eye " + liv->GetName() +
            " with thoughts on " + possessive(liv) +
            " possessions.");
    if( this_player()->GetInCombat() )
        this_player()->SetAttack(0, (: eventSteal, this_player(), obs, liv :),
                ROUND_OTHER);
    else eventSteal(this_player(), obs, liv);
    return 1;
}

static void eventSteal(object who, mixed what, object target) {
    if(objectp(what) && what->GetProperty("no steal")){
        write("That item cannot be stolen.");
        return;
    }
    who->eventSteal(who, what, target);
}
