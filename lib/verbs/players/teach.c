#include <lib.h>
#include <daemons.h>
#include <position.h>

inherit LIB_VERB;

static void create()
{
    verb::create();
    SetVerb("teach");
    SetRules("","STR to LIV","LIV to STR");
    SetErrorMessage("Syntax: teach <%^BOLD%^%^CYAN%^ability%^RESET%^> to (%^BOLD%^%^ORANGE%^player%^RESET%^)");
    SetHelp("Syntax: teach <%^BOLD%^%^CYAN%^ability%^RESET%^> to (%^BOLD%^%^ORANGE%^player%^RESET%^)\n"
            "This command allows you to teach another player an ability, spell, or skill.\n\n");
}

mixed can_teach_str_to_liv(string str, object ob)
{
    int pos = this_player()->GetPosition();
    if( (int)this_player()->GetParalyzed() ) {
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

mixed can_teach()
{
    return "Syntax: teach <%^BOLD%^%^CYAN%^ability%^RESET%^> to (%^BOLD%^%^ORANGE%^player%^RESET%^)\n";
}

mixed do_teach_str_to_liv(string spell, object target){
    this_player()->eventOfferTeaching(target,spell);
    //write("You offer to teach "+spell+" to "+target->GetName()+".");
    //tell_player(target,this_player()->GetName()+" offers to teach you "+spell+".");
    return 1;
}

mixed do_teach_liv_to_str(object target, string spell){
    return do_teach_str_to_liv(spell,target);
}
