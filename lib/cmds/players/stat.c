/*    /cmds/creator/stat.c
 *    from the Dead Souls LPC Library
 *    a command to view something's stats
 *    created by Descartes of Borg 950409
 */

#include <lib.h>
#include <daemons.h>

inherit LIB_DAEMON;

string gargs;

mixed cmd(string args) {
    string *lines, *arr, *limbs;
    object ob, *candidates;
    string tmp1, tmp2, gold;
    int i, x, y, cols, dbt;

    if( args == "" || !args || args == "me" ) ob = this_player();
    else if(args && !creatorp(this_player())) {
        write("You can only stat yourself.");
        return 1;
    }
    if(!environment(this_player())){
        write("You have no environment. Stats are the least of your worries.");
        return 1;
    }
    gargs = args;
    candidates = filter(get_livings(environment(this_player())),
            (: answers_to(gargs, $1) :) );
    if(!ob && sizeof(candidates)) ob = candidates[0];
    else if( !ob && !(ob = present(args, environment(this_player()))) )
        if( !(ob = find_player(convert_name(args))) &&
                !(ob = find_living(lower_case(args))) &&
                !(ob = find_object(args)) )
            return capitalize(args) + " is nowhere to be found.";
    if(!living(ob)) return capitalize(args) + " is not alive.";
    cols = (this_player()->GetScreen())[0];
    tmp1 = ob->GetCapName() + " aka " + ob->GetShort() +
        ", level " + ob->GetLevel() + " " + ob->GetGender();
    if( !(tmp2 = ob->GetRace()) ) tmp2 = "blob";
    tmp1 += " " + tmp2;
    if( !(tmp2 = ob->GetClass())  || !stringp(tmp2)) tmp2 = "drifter";
    tmp1 += " " + capitalize(tmp2);
    if( tmp2 = ob->GetSpouse() )
        tmp1 += " (spouse: " + tmp2 + ")";
    lines = ({ center(tmp1, cols) });
    if( ob->GetUndead() ) tmp1 = "%^BOLD%^RED%^UNDEAD%^RESET%^";
    else tmp1 = "%^BOLD%^GREEN%^Alive%^RESET%^";
    if( ob->GetSleeping() ) tmp1 += " / Sleeping";
    else tmp1 += " / Awake";
    if( ob->GetParalyzed() ) tmp1 += " / Paralyzed";
    lines += ({ center(tmp1, cols), "" });
    lines += ({ center("Health: " +ob->GetHealthPoints() + "/"+
                ob->GetMaxHealthPoints() + "   Magic: " +
                ob->GetMagicPoints() + "/" +
                ob->GetMaxMagicPoints() + "   Stamina: " +
                ob->GetStaminaPoints() + "/" +
                to_int(ob->GetMaxStaminaPoints()) + "   Carry: " +
                ob->GetCarriedMass() + "/" +
                ob->GetMaxCarry(), cols) });
    lines += ({ center("Food: " + ob->GetFood() + "    " +
                "Drink: " + ob->GetDrink() + "    " +
                "Alcohol: " + ob->GetAlcohol() + "    " +
                "Caffeine: " + ob->GetCaffeine() + "    " +
                "Poison: " + ob->GetPoison() + "    ", cols) });
    lines += ({ "\n" }) ;
    if(dbt = ob->GetExperienceDebt()) {
        lines += ({ center("Training Points: " + ob->GetTrainingPoints() +
                    "    " +
                    "Quest Points: "+ ob->GetQuestPoints(),cols)
                 });
        lines += ({ center("Experience Points: " + ob->GetExperiencePoints() +
                    "    " +
                    "Experience Debt: "+ dbt,cols)
                 });
    } else {
        lines += ({ center("Training Points: " + ob->GetTrainingPoints() +
                    "    " +
                    "Quest Points: "+ ob->GetQuestPoints() +
                    "    " +
                    "Experience Points: "+ ob->GetExperiencePoints(),cols) 
                });
    }
    lines += ({ "", "Limbs:" });
    limbs = ob->GetWieldingLimbs();
    if(ob && !ob->GetGhost()) arr = map(sort_array(ob->GetLimbs(), 1),
            (: sprintf("%:-14s%s (%d) %d/%d", $1,
                       ((member_array($1, $(limbs)) == -1) ? " " : "*"),
                       ($(ob))->GetLimbClass($1),
                       ($(ob))->GetHealthPoints($1),
                       ($(ob))->GetMaxHealthPoints($1)) :));
    i = sizeof(arr);
    while(i--) if( (y = strlen(arr[i])) > x ) x = y;
    x = cols/(x+2);
    lines += explode(format_page2(arr, x), "\n") + ({ "", "Skills:" });
    arr = map(sort_array(ob->GetSkills(), 1),
            function(string skill, object who) {
            mapping mp = who->GetSkill(skill);
            int x, max;
            x = to_int(percent(mp["points"],
                    who->GetMaxSkillPoints(skill, mp["level"])));
            max = who->GetMaxSkillLevel(skill);
            if( max < mp["level"] ) max = mp["level"];
            return sprintf("%:-18s (%d) %:2d%% - %d/%d",
                skill, mp["class"], x, mp["level"], max);
            }, ob);
    i = sizeof(arr);
    while(i--) if( (y = strlen(arr[i])) > x ) x = y;
    x = cols/(x+2);
    lines += explode(format_page2(arr, x), "\n") + ({ "", "Stats:" });
    arr = map(sort_array(ob->GetStats(), 1),
            (: sprintf("%:-12s (%d) %d/%d", $1,
                       ($(ob))->GetStatClass($1),
                       ($(ob))->GetStatLevel($1),
                       ($(ob))->GetBaseStatLevel($1)) :));
    i = sizeof(arr);
    x = 0;
    while(i--) if( (y = strlen(arr[i])) > x ) x = y;
    x =cols/(x+2);
    lines += explode(format_page2(arr, x), "\n");
    if(sizeof(ECONOMY_D->__QueryCurrencies())){
        if(valid_currency("gold")) gold = "gold";
        else gold = ECONOMY_D->__QueryCurrencies()[0];
        lines += ({ "", ob->GetName()+" has amassed a net worth of " +
                ( ob->GetNetWorth(gold) ) + " "+gold+"."});
        arr = filter( map(ob->GetCurrencies(),
                    (: ($(ob))->GetCurrency($1) &&
                     sprintf("%d %s", ($(ob))->GetCurrency($1), $1) :)),
                (: $1 :));
        lines += ({ "Money on hand: "+implode(arr, ", ") });
    }
    this_player()->eventPage(lines, "system");
    return 1;
}

string GetHelp(){
    string ret = "Syntax: stat";
    if(creatorp(this_player())) ret += " [living]";
    ret += "\n\nDisplays statistical information of a living object.\n"+
        "See also: score, status, env.";
    return ret;
}
