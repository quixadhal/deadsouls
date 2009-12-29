#include <lib.h>
#include <daemons.h>
#include <position.h>

inherit LIB_VERB;

static void create(){
    verb::create();
    SetVerb("learn");
    SetRules("","STR from LIV","to STR from LIV");
    SetErrorMessage("Syntax: learn <ability> from <person>");
    SetHelp("Syntax: learn <ability> from <person>\n\n"
            "This command allows you to learn from another person "
            "an ability, spell, or skill.\nSee also: teach");
}

mixed can_learn_str_from_liv(string str, object ob){
    int pos = this_player()->GetPosition();
    if( this_player()->GetParalyzed() ) {
        return "You cannot move!";
    }
    if( pos == POSITION_LYING &&
            !RACES_D->GetLimblessCombatRace(this_player()->GetRace()) ){
        return "You cannot learn in that position!";
    }
    return 1;
}

mixed can_learn_to_str_from_liv(string str, object ob){
    return can_learn_str_from_liv(str, ob);
}

mixed can_learn(){
    return "Syntax: learn <%^BOLD%^%^CYAN%^ability%^RESET%^> from <%^BOLD%^%^ORANGE%^person%^RESET%^>\n";
}

mixed do_learn_str_from_liv(string spell, object target){
    this_player()->eventLearn(target,spell);
    return 1;
}

mixed do_learn_to_str_from_liv(string spell, object target){
    return do_learn_str_from_liv(spell,target);
}
