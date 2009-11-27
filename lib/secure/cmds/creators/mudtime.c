#include <lib.h>
#include <daemons.h>

inherit LIB_DAEMON;

mixed cmd() {
    int *mudtime;
    int hours, minutes;
    string meridiem = " am";
    string nulle = "";
    int daynum = query_date();

    mudtime = SEASONS_D->GetMudTime();
    hours = mudtime[0];
    minutes = mudtime[1];

    // if you want an odd (as in not even) number of hours in
    // a day, this may need fiddling with.

    if(hours >= (DAY_LENGTH / 2))
        meridiem = " pm";
    if(hours > (DAY_LENGTH / 2)  && hours != DAY_LENGTH)
        hours -= (DAY_LENGTH / 2);
    if(!hours) hours = (DAY_LENGTH / 2);

    if(minutes < 10) nulle = "0";

    write("It is "+query_day_name()+", "+daynum+ordinal(daynum)+" day of "+
            query_month()+", in "+query_season()+".");

    write("In the MUD, the time is "+hours+":"+nulle+minutes+meridiem+"."); 

    return 1;

}

string GetHelp() {
    return ("Syntax: mudtime\n\n"
            "Displays the time inside of the MUD. This is entirely "
            "separate from what time it is anywhere in the real world, "
            "should one still exist.");
}
