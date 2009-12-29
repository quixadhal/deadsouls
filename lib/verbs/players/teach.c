#include <lib.h>
#include <daemons.h>
#include <position.h>

inherit LIB_VERB;

static void create(){
    verb::create();
    SetVerb("teach");
    SetRules("","STR to LIV","LIV to STR");
    SetErrorMessage("Syntax: teach <ability> to <person>");
    SetHelp("Syntax: teach <ability> to <person>\n\n"
            "This command allows you to teach another person "
            "an ability, spell, or skill.\nSee also: learn");
}

mixed can_teach_str_to_liv(string str, object ob){
    int pos = this_player()->GetPosition();
    if( this_player()->GetParalyzed() ) {
        return "You cannot move!";
    }
    if( pos == POSITION_SITTING || pos == POSITION_LYING &&
            !RACES_D->GetLimblessCombatRace(this_player()->GetRace()) ){
        return "You cannot teach in that position!";
    }
    return 1;
}

mixed can_teach_liv_to_str(object ob, string str){
    return can_teach_str_to_liv(str, ob);
}

mixed can_teach(){
    return "Syntax: teach <%^BOLD%^%^CYAN%^ability%^RESET%^> to "
        "<%^BOLD%^%^ORANGE%^person%^RESET%^>";
}

mixed do_teach_str_to_liv(string spell, object target){
    this_player()->eventOfferTeaching(target,spell);
    return 1;
}

mixed do_teach_liv_to_str(object target, string spell){
    return do_teach_str_to_liv(spell,target);
}
