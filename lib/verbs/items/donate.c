#include <lib.h>

inherit LIB_VERB;

static void create() {
    verb::create();
    SetVerb("donate");
    SetRules("STR STR");
    SetErrorMessage("Donate <AMOUNT> <CURRENCY>");
    SetHelp("Syntax: donate <AMOUNT> <CURRENCY>\n\n"
      "The donate command is used to donate money.");
}

mixed can_donate_str_str(string amount, string currency){
    return 1;
}

mixed do_donate_str_str(string amount, string currency) {
    object *stuff;
    object *donate_boxes;

    donate_boxes = ({});

    stuff = all_inventory(environment(this_player()));

    foreach(object thing in stuff){
	if(inherits(LIB_DONATE,thing)){
	    donate_boxes += ({ thing });
	}
    }

    if(sizeof(donate_boxes)){
	donate_boxes[0]->eventDonate(amount, currency);
    }

    else write("There is nowhere to donate that.");

    return 1;
}

