// /secure/sefun/query_time_of_day.c
// from the Dead Souls mudlib
// returns the time of day name.
// created by Rush 18 december 1995

#include <daemons.h>

string query_time_of_day() { return SEASONS_D->GetTimeOfDay(); }


