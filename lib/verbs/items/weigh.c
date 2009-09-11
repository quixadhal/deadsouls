#include <lib.h>
#include <vendor_types.h>
#include <rounds.h>
#include "include/weigh.h"

inherit LIB_VERB;

static void create() {
    verb::create();
    SetVerb("weigh");
    SetRules("OBJ");
    SetErrorMessage("What would you like to weigh?");
    SetHelp("Syntax: weigh OBJ\n\n"
            "A general estimate of how much a thing weighs."
            "");
}

mixed can_weigh_obj() {
    return 1;
}

mixed do_weigh_obj(object obj) {
    string name, verb = "heft";
    if(!obj) return "You must weigh something.";

    /* Check for presence of objects */
    name = obj->GetShort();
    if( environment(obj) != this_player() ) {
        this_player()->eventPrint("You do not have "+name+".");
        return 1;
    }

    if(obj->GetWorn()) verb = "get a feel for";

    this_player()->eventPrint("You stare intently at " + name + " and " +
            verb + " it.");
    environment(this_player())->eventPrint( this_player()->GetName() +
            " looks at " + name + ".", this_player());
    if( this_player()->GetInCombat() )
        this_player()->SetAttack(0,
                (: eventWeigh, this_player(), obj :),
                ROUND_OTHER);
    else eventWeigh(this_player(), obj);
    return 1;
}

int eventWeigh(object who, object obj){
    int ret, max, curr, cost, diff;
    float tmp, error;
    string ret_str, name;
    if( !(obj) ) return 0;
    name = obj->GetShort();
    cost = random(3) + 1;
    max = who->GetMaxStaminaPoints();
    curr = who->GetStaminaPoints();
    diff = max - curr;
    if(diff > 0) error = to_float(random(percent(diff, curr)));
    else diff = 0;
    if((environment(obj) != who)){
        who->eventPrint("You must have the item in your possession "
                "to weigh it.");
        return 0;
    }
    if(cost > who->GetStaminaPoints()){
        who->eventPrint("You are too weary to weigh right now.");
        environment(who)->eventPrint(
                who->GetName() + " looks tired.", who);
        return 0;
    }
    who->AddStaminaPoints(-cost);
    ret = obj->GetWeight();
    tmp = to_float(ret) * (error * 0.01);
    ret += to_int(tmp);
    ret /= 100;
    if(ret < 1) ret_str = "less than one pound.";
    else if(ret < 2) ret_str = "about one pound.";
    else ret_str = "about " + cardinal(ret) + " pounds.";
    write("You make a rough estimate that " + name + " weighs " +
            ret_str);
    return 1;
} 
