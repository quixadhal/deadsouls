#include <lib.h>
#include <vendor_types.h>
inherit LIB_ITEM;
int polefound;
string SearchCrap(){
    string result;
    object *livings;
    livings = get_livings(environment(this_object()));
    foreach(object living in livings){
        if(living->GetRace() == "troll" && !interactive(living)) {
            write("The troll distracts you!");
            return " ";
        }
    }
    if(!polefound){
        result="Rummaging through the rocks, you "+
            "discover a fishing pole hidden underneath.";
        say(this_player()->GetName()+" searches the pile of rocks "+
                "and seems to have found something of value.");
        new("/domains/town/obj/pole")->eventMove(environment(this_player()));
        polefound=1;
        return result; 
    }

    result="You rummage through the rocks "+
        "and find nothing.";
    say(this_player()->GetName()+" searches the pile of rocks "+
            "with no results.");
    return result; 
}


void create(){
    ::create();
    SetKeyName("rocks");
    SetId( ({"pile","rocks","crap","bunch"}) );
    SetAdjectives( ({"pile of"}) );
    SetShort("a pile of rocks");
    SetLong("This is a bunch of rocks piled together.");
    SetMass(200);
    SetDollarCost(0);
    SetNoCondition(1);
    SetVendorType(VT_TREASURE);
    SetSearch( (: SearchCrap :) );
    polefound=0;
    SetItems( ([
                ({"rock","rocks"}) : "A pile of rocks.",
                ]) );

}

mixed CanGet(object ob) { return "The pile of rocks isn't at all portable.";}
