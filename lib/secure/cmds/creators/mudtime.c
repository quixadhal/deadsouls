#include <lib.h>
#include <daemons.h>

inherit LIB_DAEMON;

mixed cmd() {
    int *mudtime;
    int integer, hours, minutes;
    string meridiem = " am";
    string nulle = "";
    int daynum = query_date();

    mudtime = SEASONS_D->GetMudTime();
    hours = mudtime[0];
    minutes = mudtime[1];

    if(hours >= 12  && hours != 24) {
        if(hours != 12) hours -= 12;
        meridiem = " pm";
    }

    if(!hours || hours == 0) hours = 12;

    if(minutes < 10) nulle = "0";

    write("It is "+query_day_name()+", "+daynum+ordinal(daynum)+" day of "+
      query_month()+", in "+query_season()+".");

    write("In the MUD, the time is "+hours+":"+nulle+minutes+meridiem+"."); 

    return 1;

}

void help() {
    message("help", "Syntax: <mudtime>\n\n"
      "Displays the time inside of the MUD. This is entirely "
      "separate from what time it is anywhere in the real world."
      "\n", this_player());
}
