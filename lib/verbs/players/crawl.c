#include <lib.h>
#include <position.h>

inherit LIB_VERB;

static void create() {
    verb::create();
    SetVerb("crawl");
    SetRules("STR", "into STR");
    SetErrorMessage("Crawl in which direction?");
    SetHelp("Syntax: crawl <DIRECTION>\n"
            "        crawl into <PLACE>\n\n"
            "Moves you towards the direction you specify, or into the place "
            "you specify.  The command \"crawl into\" is synonymous with the "
            "\"enter\" command.");
}

mixed can_crawl_str(string str) {
    if( !environment(this_player()) ) return "You are nowhere.";
    if( this_player()->GetStaminaPoints() <3 )
        return "You are too tired to crawl anywhere right now.";
    if(this_player()->GetPosition() != POSITION_LYING &&
            this_player()->GetPosition() != POSITION_SITTING){
        return "You can't crawl in your current position.";
    }
    return 1;
}

mixed can_crawl_into_str(string str) {
    if( !environment(this_player()) ) return "You are nowhere.";
    if( this_player()->GetStaminaPoints() <3 )
        return "You are too tired right now.";
    if(this_player()->GetPosition() != POSITION_LYING &&
            this_player()->GetPosition() != POSITION_SITTING){
        return "You can't crawl in your current position.";
    }
    return 1;
}

mixed do_crawl_str(string str) {
    this_player()->AddStaminaPoints(-5);
    environment(this_player())->eventGo(this_player(), str);
    return 1;
}

mixed do_crawl_into_str(string str) {
    object targ;
    str = remove_article(lower_case(str));
    targ = present(str,environment(this_player()));
    if(!targ) targ = present(str,this_player());
    if(!targ) return "That's not possible.";
    this_player()->AddStaminaPoints(-5);
    return targ->eventEnter(this_player(), str,"crawl");
}
