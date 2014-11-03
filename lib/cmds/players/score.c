/*  /verbs/players/score.c
 *  from the Dead Souls Library
 *  Displays a list of score and related info
 *  created by Blitz@Dead Souls
 */

#include <lib.h>
#include <daemons.h>

inherit LIB_DAEMON;
inherit LIB_HELP;

varargs mixed eventScore();

static void create() {
    daemon::create();
    SetNoClean(1);
    SetHelp("Syntax: score\n\n"
            "Displays information about your character.\n"
            "See also: stat, status, env");
}

static string *FoodDegree =
({ "starving!", "very hungry.",
 "hungry.", " partially hungry.",
 "feeling full.", "quite full." });

static string *DrunkDegree =
({ "sober", "tipsy", "drunk", "blitzed",
 "three sheets to the wind", "FUBAR" });

static string *DrinkDegree =
({ "parched", "extremely thirsty", "very thirsty", "thirsty",
 "somewhat thirsty", "unthirsty" });

mixed cmd(string arg) {
    eventScore(arg);
    return 1;
}

varargs mixed eventScore(string arg) {
    string *str;
    int birth, age, x, y, z, qp, xp, dbt;
    string *tmp;
    mapping lev;
    object who;
    string prn, haben, be, poss;
    string past, qual, cnj;

    if(!arg || !creatorp(this_player()) || !(who = find_player(arg))){
        who = this_player();
        haben = "have";
        be = "are";
        poss = "your";  
        prn = "you";
        past = "were";
        qual = "qualify";
        cnj = "";
    }

    else {
        haben = "has";
        be = "is";
        poss = possessive(who);
        prn = nominative(who);
        past = "was";
        qual = "qualifies";
        cnj = "s";
    }
    str  = ({ capitalize(prn)+" "+be+" "+who->GetShort() + " (" +
        who->GetMoralityDescription() + ")." });
    str += ({ sprintf(capitalize(prn)+" "+be+" a level %d %s%s %s.",
                who->GetLevel(),
                ( who->GetUndead() ? "undead " : ""),
                capitalize(who->GetRace() || "nothing"),
                capitalize(who->GetClass() || "commoner")) });
    str += ({ capitalize(poss)+" native town is "+who->GetTown()+", and "+
            prn+" "+be+" "+ (who->GetReligion() ||
                "agnostic") + " in faith." });
    str += ({ sprintf(capitalize(prn)+" "+haben+" solved %s, and "+haben+" %s."+
                " (%d Quest Points)",
                consolidate(sizeof(who->GetQuests()),
                    "one quest"),
                consolidate(sizeof(who->GetTitles()),
                    "one title"), who->GetQuestPoints() ) });
    birth = who->GetBirth();
    age = ( query_year(time()) - query_year(birth) );
    str += ({ sprintf(capitalize(prn)+" "+past+
                " born on the %d%s day of %s, year %d. "
                "(%d years old)", query_date(birth), ordinal(query_date(birth)),
                query_month(birth), query_year(birth), age) });
    if( x = who->GetTrainingPoints() < 1 ) {
        y = who->GetLevel() + 1 + (x / -4);
        str += ({ "Training points await "+(who == this_player() ? "you" :
                    objective(who))+" at level " + y + "." });
    }
    else str += ({ capitalize(prn)+" "+haben+" " + consolidate(
                who->GetTrainingPoints(),
                "one training point") + "." });
    if( who->GetWimpy() )
        str += ({ capitalize(prn)+" "+be+" feeling wimpy." }); else
            str += ({ capitalize(prn)+" "+be+" feeling brave." });
    if( who->GetPoison() > 0 )
        str += ({ capitalize(prn)+" "+be+" poisoned." });
    x = who->GetFood() / 17;
    if( x > sizeof(FoodDegree) - 1 ) x = (sizeof(FoodDegree) - 1);
    y = who->GetDrink() / 17;
    if( y > sizeof(DrinkDegree) - 1 ) y = (sizeof(DrinkDegree) - 1);
    z = who->GetAlcohol();
    if(z) z = (z/17) + 1;
    if( z > sizeof(DrunkDegree) - 1 ) z = (sizeof(DrunkDegree) - 1);
    str += ({ capitalize(prn)+" "+be+" "+FoodDegree[x] });
    str += ({ sprintf(capitalize(prn)+" "+be+
                " %s and %s.", DrinkDegree[y], DrunkDegree[z]) });
    x = who->GetCustomStats();

    tmp = ({});
    qp = who->GetQuestPoints();
    xp = who->GetExperiencePoints();
    dbt = who->GetExperienceDebt();
    lev = PLAYERS_D->GetLevelList()[(who->GetLevel()) + 1];

    if(dbt){
        str += ({ capitalize(prn)+" have "+dbt+" points of "+
                "experience debt." });
    }

    if(lev){
        if(REQUIRE_QUESTING){
            qp = lev["qp"] - qp;
            if(qp > 0) tmp += ({capitalize(prn)+" require"+cnj+" "+
                    comma(qp)+" more quest points to advance."});
        }

        xp = lev["xp"] - xp;
        if(xp > 0) tmp += ({capitalize(prn)+" require"+cnj+" "+
                comma(xp)+" more experience points to advance."});
        if(!sizeof(tmp)) tmp = ({capitalize(prn)+" "+qual+
                " to advance a level."});
        str += tmp; 
    }

    if(x){
        str += ({ "\n"+capitalize(prn)+" "+haben+" "+x+
                " customization points left. Type: help customize" });
    }
    this_player()->eventPage(str, "info");
    return 1;
}
