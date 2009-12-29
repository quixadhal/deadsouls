/*    /verbs/items/give.c
 *    from the Dead Souls Mud Library
 *    give LIV OBJ
 *    give OBJ to LIV
 *    give LIV WRD WRD
 *    give WRD WRD to LIV
 *    created by Descartes of Borg 950113
 */

#include <lib.h>
#include "include/give.h"

inherit LIB_VERB;

string curr2;

static void create() {
    verb::create();
    SetVerb("give");
    SetRules("LIV WRD WRD", "WRD WRD to LIV", "OBS LIV", "LIV OBS", "OBS to LIV" );
    SetErrorMessage("Give what to whom?");
    SetHelp("Syntax: <give LIVING ITEM>\n"
            "        <give LIVING ITEMS>\n"
            "        <give ITEM to LIVING>\n"
            "        <give ITEMS to LIVING>\n"
            "        <give LIVING AMOUNT CURRENCY>\n"
            "        <give AMOUNT CURRENCY to LIVING>\n\n"
            "This command allows you to give something you have to "
            "someone else.\n\n"
            "See also: drop, get, put");
}

mixed can_give_liv_obj(mixed args...) {
    return can_give_obj_to_liv();
}

mixed can_give_obj_liv(mixed args...) {
    return can_give_obj_to_liv();
}

mixed can_give_obj_to_liv(mixed args...) { 
    return this_player()->CanManipulate(); }

    mixed can_give_liv_wrd_wrd(object targ, string num, string curr) {
        return can_give_wrd_wrd_to_liv(num, curr, targ);
    }

mixed can_give_wrd_wrd_to_liv(string num, string curr, object targ) {
    int amt;
    curr2 = curr;
    if(!valid_currency(curr)) curr = truncate(curr,1);
    if(!valid_currency(curr)) curr = truncate(curr,1);
    if(!valid_currency(curr)) curr = curr2 +"s";
    if(!valid_currency(curr)) curr = curr2 +"es";
    if(valid_currency(curr)) curr2 = curr;

    if(sscanf(num,"%d",amt) != 1){
        if(valid_currency(curr)) return "Please use a number to specify the amount.";
        else return "That isn't a valid currency.";
    }
    if( amt < 1 ) return "What sort of amount is that?";
    if( amt > this_player()->GetCurrency(lower_case(curr)) )
        return "You don't have that much " + curr + "."; 
    if(newbiep(this_player())) return "Newbies can't give money.";
    return this_player()->CanManipulate();
}

//mixed do_give_liv_obj(object target, object what) {
mixed do_give_liv_obj(mixed args...) {
    object target, what;
    target = args[0];
    what = args[1];
    return do_give_obj_to_liv(what, target);
}

//mixed do_give_obj_liv(object what, object target) {
mixed do_give_obj_liv(mixed args...) {
    object target, what;
    target = args[1];
    what = args[0];
    return do_give_obj_to_liv(what, target);
}

//mixed do_give_obj_to_liv(object what, object target) {
mixed do_give_obj_to_liv(mixed args...) {
    object target, what;
    target = args[1];
    what = args[0];

    if(!intp(target->CanManipulate())){
        this_player()->eventPrint(target->GetName()+" is incapable "+
                "of holding that.");
        return 1;
    }
    if( !(what->eventMove(target)) ) {
        this_player()->eventPrint("They cannot accept that right now.");
        return 1;
    }
    this_player()->eventPrint("You give " + target->GetName() + " " +
            what->GetShort() + ".");
    target->eventPrint(this_player()->GetName() + " gives you " +
            what->GetShort() + ".");
    environment(this_player())->eventPrint(this_player()->GetName() +
            " gives " +
            target->GetName() +
            " " + what->GetShort() +".",
            ({ this_player(), target }));
    return 1;
}

mixed do_give_liv_wrd_wrd(object target, string num, string curr) {
    return do_give_wrd_wrd_to_liv(num, curr, target);
}

mixed do_give_wrd_wrd_to_liv(string num, string curr, object target) {
    int amt;
    if(curr2) curr = curr2;
    amt = to_int(num);
    if( target->AddCurrency(curr, amt) == -1 ) {
        this_player()->eventPrint("You just can't give that money away.");
        return 1;
    }
    if( this_player()->AddCurrency(curr, -amt) == -1 ) {
        target->AddCurrency(curr, -amt);
        this_player()->eventPrint("The amount of money you have is boggled.");
        return 1;
    }
    this_player()->eventPrint("You give " + target->GetName() + " " +
            amt + " " + curr + ".");
    target->eventPrint(this_player()->GetName() + " gives you " +
            amt + " " + curr + ".");
    environment(this_player())->eventPrint(this_player()->GetName() +
            " gives " + amt + " " + curr +
            " to " + target->GetName() +
            ".", ({ target, this_player() }));
    return 1;
}

mixed do_give_liv_obs(object target, mixed *items) {
    return do_give_obs_to_liv(items, target);
}

mixed do_give_obs_liv(mixed *items, object target) {
    return do_give_obs_to_liv(items, target);
}

mixed do_give_obs_to_liv(mixed *items, object target) {
    object *obs, *eligible;

    if( sizeof(items) < 1 ) {
        this_player()->eventPrint("You don't have any to give.");
        return 1;
    }

    obs = filter(items, (: objectp :));
    if( !sizeof(obs) ) {
        mixed *ua;

        ua = unique_array(items, (: $1 :));
        foreach(string *list in ua) this_player()->eventPrint(list[0]);
        return 1;
    }
    eligible=filter(obs, (: (!($1->GetWorn()) && environment($1) == this_player()) :));
    if(!sizeof(eligible)){
        write("Remove or unwield items before trying to sell them.");
        eligible = ({});
        return 1;
    }

    foreach(object ob in eligible) do_give_obj_to_liv(ob, target);
    return 1;
}
