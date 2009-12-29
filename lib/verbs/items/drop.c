/*    /verbs/items/drop.c
 *    from the Dead Souls Mud Library
 *    drop OBJ
 *    drop OBS
 *    drop WRD WRD
 *    created by Descartes of Borg 960113
 */



#include <lib.h>
#include "include/drop.h"

inherit LIB_VERB;

static void create() {
    verb::create();
    SetVerb("drop");
    SetSynonyms("put down");
    SetRules("OBS", "WRD WRD");
    SetErrorMessage("Drop what?");
    SetHelp("Syntax: <drop ITEM>\n"
            "        <drop all>\n"
            "        <drop all ITEM TYPE>\n"
            "        <drop AMOUNT CURRENCY>\n\n"
            "Allows you to drop something you have, or to drop an amount of "
            "some currency you have on you.\n\n"
            "See also: get, put");
}

mixed can_drop_obj(object ob) { return this_player()->CanManipulate(); }

//mixed can_drop_wrd_wrd(string num, string curr) {
mixed can_drop_wrd_wrd(mixed args...) {
    string num, curr;
    int amt;
    object ob;

    num = args[0];
    curr = args[1];

    ob = get_object(num+" "+curr);

    if(ob) return can_drop_obj(ob);

    if( !num || !curr ) return 0;
    if( (amt = to_int(num)) < 1 ) return "You cannot do that!";
    if( this_player()->GetCurrency(curr) < amt )
        return "You don't have that much " + curr + ".";
    if(newbiep(this_player())) return "Newbies can't drop money.";
    return this_player()->CanManipulate();
}

mixed do_drop_obj(object ob) {
    return ob->eventDrop(this_player());
}

mixed do_drop_obs(mixed *res) {
    object *obs, *eligible;
    mixed tmp;

    if( !sizeof(res) ) {
        this_player()->eventPrint("You don't have any to drop!");
        return 1;
    }
    obs = filter(res, (: objectp :));
    if( !sizeof(obs) ) {
        mixed *ua;

        ua = unique_array(res, (: $1 :));
        foreach(string *list in ua) this_player()->eventPrint(list[0]);
        return 1;
    }
    eligible=filter(obs, (: (!($1->GetWorn()) && environment($1) == this_player()) :)); 
    if(!sizeof(eligible)){
        write("Remove or unwield items before trying to drop them.");
        eligible = ({});
        return 1;
    }
    foreach(object ob in eligible) 
        if( (tmp = ob->eventDrop(this_player())) != 1 ) {
            if( stringp(tmp) ) this_player()->eventPrint(tmp);
            else this_player()->eventPrint("You cannot drop " +
                    ob->GetShort() + ".");
        }
    return 1;
}

//mixed do_drop_wrd_wrd(string num, string curr) {
mixed do_drop_wrd_wrd(mixed args...) {
    object ob, pile, env;
    string num, curr;
    int amt;

    num = args[0];
    curr = args[1];

    if(ob = get_object(num+" "+curr)) return do_drop_obj(ob);

    amt = to_int(num);
    env = environment(this_player());
    pile = new(LIB_PILE);
    pile->SetPile(curr, amt);
    if( !(pile->eventMove(env)) ||
            this_player()->AddCurrency(curr, -amt) == -1 ) {
        this_player()->eventPrint("Something prevents your action.");
        pile->eventDestruct();
        return 1;
    }
    this_player()->eventPrint("You drop " + amt + " " + curr + ".");
    environment(this_player())->eventPrint(this_player()->GetName() +
            " drops some " + curr + ".",
            this_player());
    return 1;
}
