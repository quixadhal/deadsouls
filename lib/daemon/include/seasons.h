#ifndef __SEASONS_H__
#define __SEASONS_H__

class month {
    string Name;
    string Season;
    int Days;
    int DaylightHours;
}

class moon {
    string Name;
    int Phase;
    string Id;
    string Description;
}

static void create();
static void eventConfigure();
static void eventDawn();
static void eventMorning();
static void eventTwilight();
static void eventNight();
static void eventMidnight();
int GetCurrentDay();
string GetCurrentDayName();
string GetCurrentMonth();
string GetCurrentSeason();
int GetCurrentTime();
int GetCurrentYear();
int GetDay(int x);
string GetDayName(int x);
string *GetDays();
int GetDaylightHours(string mon);
int GetHour(int x);
int GetMinutes(int x);
string GetMonth(int x);
private int GetMonthIndex(int x);
string *GetMonths();
string GetSeason(int x);
int GetTime(int x);
string GetTimeOfDay();
function AddTimeEvent(string tod, function f);
mapping GetTimeEvents();
int GetYear(int x);
string GetYearString(int x);
string GetPhaseName(int x);
int GetPhase(string m);
int GetMoonLight();
string GetLong(string arg);

#endif /* __SEASONS_H__ */


