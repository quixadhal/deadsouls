#include <lib.h>
#include <vendor_types.h>
inherit LIB_ITEM;

int ringfound;
string SearchCrap(){
    string result;
    if(!ringfound){
        result="Rummaging through the disgusting pile, you "+
            "discover a beautiful gold ring mixed in with the "+
            "crap.";
        say(this_player()->GetName()+" searches the pile of debris "+
                "and seems to have found something of value.");
        if(!new("/domains/campus/armor/ring")->eventMove(this_player()))
            new("/domains/campus/armor/ring")->eventMove(environment(this_object()));
        ringfound=1;
        return result; 
    }

    result="You rummage through the disgusting pile "+
        "and find nothing.";
    say(this_player()->GetName()+" searches the pile of debris "+
            "with no results.");
    return result; 
}
void create(){
    ::create();
    SetKeyName("debris");
    SetId( ({"pile","debris","crap","crud"}) );
    SetAdjectives( ({"disgusting","pile of","smelly"}) );
    SetShort("a pile of debris");
    SetLong("This is a smelly, disgusting pile of crud "+
            "that has accumulated from the running stream of "+
            "sewage here. ");
    SetMass(20);
    SetDollarCost(0);
    SetVendorType(VT_TREASURE);
    SetSearch( (: SearchCrap :) );
    ringfound=0;

}
mixed CanGet(object ob) { return "The pile of debris isn't at all portable.";}
string GetItemCondition() { return "";}
void init(){
    ::init();
}
