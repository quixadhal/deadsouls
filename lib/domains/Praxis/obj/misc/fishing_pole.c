/* fishing pole designed by Descartes for use in rooms that are fishing
   compatible.  The pole is really nothing special except in that
   you can bait it with anything that returns an id of bait and also
   in that it has the following functions called by fishing rooms:
   query_pole_class() returns an integer, the higher the number, the
   better the pole is for fishing.
   query_bait() returns an integer which is the strength of the bait,
   the higher the number, the better the bait is for fishing
   pole class should range from 2 (a horrible pole) to 7 (an absolutely
   awesome pole)
   bait strength, which is found out by the pole by calling a function
   in items which return an id of true for bait, should be a
   number between 0 (items fish wont eat) to 4 (yummy fish food)
 */

#include <lib.h>
inherit LIB_ITEM;

int query_bait();
int query_pole_class();
int query_broken();
int break_pole();
void unbait();
int broken;
int bait;

void init() {
    add_action("bait_it","bait");
}

void create() {
    ::create();
    SetKeyName("stick");
    SetId( ({ "stick", "fishing pole", "hook", "string" }) );    SetValue( 20);
    SetMass( 28);

    broken = 0;
    bait = 0;
}

string GetLong(string str) {
    if(str == "hook") {
        if(!bait) return ("Perhaps you could bait this with something?\n");
        else return ("It has bait on it.\n");
    }
    else if(str == "string") return ("A string with a hook at the end.\n");
    else {
        return ("A bamboo stick with a piece of string connected to the end.\n"
                "At the end of the string is a hook.  What a primitive looking fishing tool!\n");
    }
}

string GetShort() {
    if(!broken) {
        if(!bait) return "A bamboo stick";
        else return "A bamboo stick (baited)";
    }
    else return "A bamboo stick (broken)";
}

int query_pole_class() {
    return 2;
}

int query_bait() {
    return bait;
}

int bait_it(string str) {
    string pole, the_bait;
    if(!str) return 0;
    if((sscanf(str, "%s with %s", pole, the_bait)) !=2) return 0;
    if(pole != "stick" && pole != "hook") return 0;
    if(!present(the_bait,this_player())) {
        write("You do not have any "+the_bait+" to bait your stick!\n");
        return 1;
    }
    if(!(present(the_bait,this_player())->id("bait"))) {
        write("You can't use that as bait!\n");
        return 1;
    }
    if(!bait) {
        bait = (100-present(the_bait, this_player())->query_strength())/7;
        write("You bait your hook with "+the_bait+".\n");

        say(this_player()->query_cap_name()+" baits "+this_player()->query_possessive()+" stick.\n");
        find_object(the_bait)->remove();
        return 1;
    }
    else {
        write("You already have bait on your hook!");
    }
    return 1;
}

void unbait() {
    bait = 0;
}

int query_broken(){
    return broken;} int break_pole() {    SetValue( 10);

        true();
        broken = 1;
    }

