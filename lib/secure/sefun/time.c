/*    /secure/sefun/time.c
 *    from the Dead Souls LPC Library
 *    simuls reguarding MUD time
 *    created by Descartes of Borg 950514
 */

#include <lib.h>
#include <daemons.h>

#define MIN 60
#define HOUR (60 * MIN)
#define DAY  (24 * HOUR)
#define WEEK ( 7 * DAY)
#define YEAR ( 52 * WEEK)


int query_night() { return (SEASONS_D->GetTimeOfDay() == "night"); }

string query_day_name(int x) { return SEASONS_D->GetDayName(x); }

int query_date(int x) { return SEASONS_D->GetDay(x); }

string query_month(int x) { return SEASONS_D->GetMonth(x); }

int query_year(int x) { return SEASONS_D->GetYear(x); }

int query_minutes(int x) { return SEASONS_D->GetMinutes(x); }

string query_season(int x) { return SEASONS_D->GetSeason(x); }

int query_hour(int x) { return SEASONS_D->GetHour(x); }

function time_event(string tod, function f) {
    if( !functionp(f) ) error("Bad argument 1 to time_event().\n");
    return SEASONS_D->AddTimeEvent(tod, f);
}

varargs string time_elapsed(int arg1, int arg2){
    int tm, x;
    string str;
    if(arg2) arg1 = arg2 - arg1;
    tm = arg1;

    str = "";
    if (x = (tm / YEAR)) {
        str += x + "y ";
        tm -= x * YEAR;
    }

    if (x = (tm / WEEK)) {
        str += x + "w ";
        tm -= x * WEEK;
    }
    if (x = (tm / DAY)) {
        str += x +"d ";
        tm -= x * DAY;
    }
    if (x = (tm / HOUR)) {
        str += x + "h ";
        tm -= x * HOUR;
    }
    if (x = (tm / MIN)) {
        str += x + "m ";
        tm -= x * MIN;
    }
    if (tm) {
        str += tm + "s";
    }
    //str += str[0..<2] + "%^RESET%^.";
    return str;
}


