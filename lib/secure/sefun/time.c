/*    /secure/sefun/time.c
 *    from the Dead Souls LPC Library
 *    simuls reguarding MUD time
 *    created by Descartes of Borg 950514
 */

#include <daemons.h>

int query_night() { return ((string)SEASONS_D->GetTimeOfDay() == "night"); }

string query_day_name(int x) { return (string)SEASONS_D->GetDayName(x); }

int query_date(int x) { return (int)SEASONS_D->GetDay(x); }

string query_month(int x) { return (string)SEASONS_D->GetMonth(x); }

int query_year(int x) { return (int)SEASONS_D->GetYear(x); }

int query_minutes(int x) { return (int)SEASONS_D->GetMinutes(x); }

string query_season(int x) { return (string)SEASONS_D->GetSeason(x); }

int query_hour(int x) { return (int)SEASONS_D->GetHour(x); }

function time_event(string tod, function f) {
    if( !functionp(f) ) error("Bad argument 1 to time_event().\n");
    return SEASONS_D->AddTimeEvent(tod, f);
}

