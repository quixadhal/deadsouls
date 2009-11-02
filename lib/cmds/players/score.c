/*  /verbs/players/score.c
 *  from the Dead Souls Library
 *  Displays a list of score and related info
 *  created by Blitz@Dead Souls
 */

#include <lib.h>
#include <daemons.h>

inherit LIB_DAEMON;
inherit LIB_HELP;

mixed eventScore();

static void create() {
    daemon::create();
    SetNoClean(1);
    SetHelp("Syntax: score\n\n"
            "Displays information about your character.\n\n"
            "See also: stat, status, env\n");
}

static string *FoodDegree =
({ "could eat a horse right now!", "could eat plenty more.",
 "could eat some more.", "are partially hungry.",
 "are feeling full.", "feel quite full." });

static string *DrunkDegree =
({ "you are sober", "you are tipsy", "you are drunk", "you are blitzed",
 "you are smashed out of your gord", "you are FUBAR" });

static string *DrinkDegree =
({ "parched", "extremely thirsty", "very thirsty", "thirsty",
 "somewhat thirsty", "not thirsty" });

mixed cmd(string unused) {
    eventScore();
    return 1;
}

mixed eventScore() {
    string *str;
    int birth, age, x, y, z, qp, xp;
    string *tmp;
    mapping lev;

    str  = ({ "You are "+this_player()->GetShort() + " (" +
        this_player()->GetMoralityDescription() + ")." });
    str += ({ sprintf("You are a level %d %s%s %s.",
                this_player()->GetLevel(),
                ( this_player()->GetUndead() ? "undead " : ""),
                capitalize(this_player()->GetRace() || "nothing"),
                capitalize(this_player()->GetClass() || "commoner")) });
    str += ({ "Your native town is "+this_player()->GetTown()+", and "
            "you are " + (this_player()->GetReligion() ||
                "agnostic") + " in faith." });
    str += ({ sprintf("You've solved %s, and have %s.",
                consolidate(sizeof(this_player()->GetQuests()),
                    "one quest"),
                consolidate(sizeof(this_player()->GetTitles()),
                    "one title") ) });
    birth = this_player()->GetBirth();
    age = ( query_year(time()) - query_year(birth) );
    str += ({ sprintf("You were born on the %d%s day of %s, year %d. "
                "(%d years old)", query_date(birth), ordinal(query_date(birth)),
                query_month(birth), query_year(birth), age) });
    if( x = this_player()->GetTrainingPoints() < 1 ) {
        y = this_player()->GetLevel() + 1 + (x / -4);
        str += ({ "Training points await you at level " + y + "." });
    }
    else str += ({ "You have " + consolidate(
                this_player()->GetTrainingPoints(),
                "one training point") + "." });
    if( this_player()->GetWimpy() )
        str += ({ "You are feeling wimpy." }); else
            str += ({ "You are feeling brave." });
    if( this_player()->GetPoison() > 0 )
        str += ({ "You are poisoned." });
    x = this_player()->GetFood() / 17;
    if( x > sizeof(FoodDegree) - 1 ) x = (sizeof(FoodDegree) - 1);
    y = this_player()->GetDrink() / 17;
    if( y > sizeof(DrinkDegree) - 1 ) y = (sizeof(DrinkDegree) - 1);
    z = this_player()->GetAlcohol();
    if(z) z = (z/17) + 1;
    if( z > sizeof(DrunkDegree) - 1 ) z = (sizeof(DrunkDegree) - 1);
    str += ({ "You "+FoodDegree[x] });
    str += ({ sprintf("You are %s and %s.", DrinkDegree[y], DrunkDegree[z]) });
    x = this_player()->GetCustomStats();

    tmp = ({});
    qp = this_player()->GetQuestPoints();
    xp = this_player()->GetExperiencePoints();
    lev = PLAYERS_D->GetLevelList()[(this_player()->GetLevel()) + 1];

    if(REQUIRE_QUESTING){
        qp = lev["qp"] - qp;
        if(qp > 0) tmp += ({"You require "+comma(qp)+" more quest points to advance."});
    }

    xp = lev["xp"] - xp;
    if(xp > 0) tmp += ({"You require "+comma(xp)+" more experience points to advance."});
    if(!sizeof(tmp)) tmp = ({"You qualify to advance a level."});
    str += tmp; 

    if(x){
        str += ({ "\nYou have "+x+" customization points left. Type: help customize" });
    }
    this_player()->eventPage(str, "info");
    return 1;
}
